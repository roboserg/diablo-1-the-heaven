#include "stdafx.h"

// ============================================================================
// ChatHistory.cpp — Multiplayer Chat History Overlay
// ============================================================================
// Local-client-only chat history buffer and overlay UI.
// No network packets are added or modified.
// ============================================================================

// ---------------------------------------------------------------------------
// Constants
// ---------------------------------------------------------------------------
constexpr int CHAT_HISTORY_MAX_MESSAGES     = 40;   // 4 players × 10 messages
constexpr int CHAT_HISTORY_LINE_HEIGHT      = 14;   // pixels per text line (small font)
constexpr int CHAT_HISTORY_MAX_DISPLAY_CHARS = 160; // max formatted message length
constexpr int CHAT_HISTORY_OVERLAY_MAX_WIDTH = 600; // max overlay width in pixels
constexpr int CHAT_HISTORY_OVERLAY_MARGIN    = 20;  // margin from screen edges
constexpr int CHAT_HISTORY_OVERLAY_PADDING   = 8;   // internal padding
constexpr int CHAT_HISTORY_OVERLAY_TOP_Y     = 20;  // top margin from screen edge
constexpr int CHAT_HISTORY_MAX_VISIBLE_LINES = 25;
constexpr int CHAT_HISTORY_MAX_WRAP_LINES_PER_MSG = 4; // max wrapped lines per message
constexpr int CHAT_HISTORY_HEADER_HEIGHT     = CHAT_HISTORY_LINE_HEIGHT + 14; // 28px: header band height; text (rendered upward) sits with ~7px margin each side
constexpr int CHAT_HISTORY_BORDER_COLOR      = 197; // palette index for border (grey)
constexpr int CHAT_HISTORY_SCROLLBAR_COLOR   = 241; // palette index for scrollbar indicator
constexpr int CHAT_HISTORY_CONTENT_TOP_EXTRA = 5;   // extra gap from separator to first message (ensures no glyph overlap)
constexpr int CHAT_HISTORY_FOOTER_EXTRA      = 10;  // extra height below content before the bottom border

// ---------------------------------------------------------------------------
// Message types
// ---------------------------------------------------------------------------
enum ChatMessageType : unsigned char {
	CHAT_MSG_PLAYER = 0, // Normal player chat
	CHAT_MSG_SYSTEM = 1, // Join/leave/server messages
};

// ---------------------------------------------------------------------------
// Chat history entry
// ---------------------------------------------------------------------------
struct ChatHistoryEntry {
	char text[CHAT_HISTORY_MAX_DISPLAY_CHARS]; // Pre-formatted display string
	unsigned char senderIndex;                  // Player index (PlayersMax_4 = system)
	ChatMessageType type;
	i64 timestamp;
	bool used;
};

// ---------------------------------------------------------------------------
// Wrapped line reference (points into a ChatHistoryEntry)
// ---------------------------------------------------------------------------
struct ChatWrappedLine {
	int entryIndex;  // index into chatHistoryBuffer
	int startOffset; // byte offset into entry.text where this line starts
	int length;      // number of characters in this wrapped line
};

// ---------------------------------------------------------------------------
// Storage
// ---------------------------------------------------------------------------
static ChatHistoryEntry chatHistoryBuffer[CHAT_HISTORY_MAX_MESSAGES];
static int chatHistoryWriteIndex = 0;
static int chatHistoryCount      = 0;

// ---------------------------------------------------------------------------
// Wrapped lines cache — rebuilt on message add or resolution change
// ---------------------------------------------------------------------------
static constexpr int MAX_TOTAL_WRAPPED_LINES = CHAT_HISTORY_MAX_MESSAGES * CHAT_HISTORY_MAX_WRAP_LINES_PER_MSG;
static ChatWrappedLine wrappedLines[MAX_TOTAL_WRAPPED_LINES];
static int wrappedLineCount = 0;
static int lastWrapWidth = 0; // overlay width when lines were last wrapped

// ---------------------------------------------------------------------------
// UI state
// ---------------------------------------------------------------------------
bool IsChatHistoryVisible = false;
static int chatHistoryScrollOffset = 0; // 0 = newest at top
DisplayObject ChatHistoryRect;          // updated each frame in ChatHistory_Draw(); used for CursorIntoDisplayObject checks

// ---------------------------------------------------------------------------
// Forward declarations
// ---------------------------------------------------------------------------
static void ChatHistory_RebuildWrappedLines();
static int  ChatHistory_CalcOverlayWidth();
static int  ChatHistory_CalcVisibleLines();
static bool ChatHistory_IsAnyPanelOpen();
static int  ChatHistory_GetEntryIndexFromNewest(int n);

// ---------------------------------------------------------------------------
// ChatHistory_Clear — wipe all stored messages
// ---------------------------------------------------------------------------
void ChatHistory_Clear()
{
	memset(chatHistoryBuffer, 0, sizeof(chatHistoryBuffer));
	chatHistoryWriteIndex = 0;
	chatHistoryCount      = 0;
	chatHistoryScrollOffset = 0;
	wrappedLineCount = 0;
	lastWrapWidth    = 0;
	IsChatHistoryVisible = false;
}

// ---------------------------------------------------------------------------
// ChatHistory_AddMessage — internal: store a message into the ring buffer
// ---------------------------------------------------------------------------
static void ChatHistory_AddMessage(const char* formattedText, unsigned char senderIndex, ChatMessageType type)
{
	if (!formattedText || !formattedText[0]) return;

	ChatHistoryEntry& entry = chatHistoryBuffer[chatHistoryWriteIndex];
	entry.senderIndex = senderIndex;
	entry.type        = type;
	entry.timestamp   = GetTickCountTh();
	entry.used        = true;

	strncpy(entry.text, formattedText, sizeof(entry.text) - 1);
	entry.text[sizeof(entry.text) - 1] = '\0';

	chatHistoryWriteIndex = (chatHistoryWriteIndex + 1) % CHAT_HISTORY_MAX_MESSAGES;
	if (chatHistoryCount < CHAT_HISTORY_MAX_MESSAGES)
		++chatHistoryCount;

	// Rebuild wrapped lines only when the overlay is visible; Toggle() rebuilds on open.
	// When visible and the user has scrolled back to read history, shift the viewport so the
	// same content remains in view (prevents the scroll position jumping on new arrivals).
	if (IsChatHistoryVisible) {
		int prevWrapCount = wrappedLineCount;
		ChatHistory_RebuildWrappedLines();
		if (chatHistoryScrollOffset > 0) {
			chatHistoryScrollOffset += wrappedLineCount - prevWrapCount;
			if (chatHistoryScrollOffset < 0) chatHistoryScrollOffset = 0;
		}
	}
}

// ---------------------------------------------------------------------------
// ChatHistory_AddPlayerMessage — called when a player chat message is displayed
// ---------------------------------------------------------------------------
void ChatHistory_AddPlayerMessage(int playerIndex, const char* formattedStr)
{
	if (MaxCountOfPlayersInGame <= 1) return; // multiplayer only
	if (!formattedStr || !formattedStr[0]) return;

	// Filter out self-sent version info (tilde key)
	// Version messages contain THE_HELL_VERSION_HUMAN_STRING prefix (e.g. "Diablo: The Hell 4")
	// and are self-sent: the formatted string starts with own player name
	if (playerIndex == CurrentPlayerIndex) {
		// Check if the raw message part (after "name (lvl N): ") starts with "Diablo: The Hell"
		const char* colonPos = strstr(formattedStr, "): ");
		if (colonPos) {
			const char* msgPart = colonPos + 3;
			if (strncmp(msgPart, "Diablo: The Hell", 16) == 0) {
				return; // Skip version info message
			}
		}
	}

	ChatHistory_AddMessage(formattedStr, (unsigned char)playerIndex, CHAT_MSG_PLAYER);
}

// ---------------------------------------------------------------------------
// ChatHistory_AddSystemMessage — called for join/leave/server messages
// ---------------------------------------------------------------------------
void ChatHistory_AddSystemMessage(const char* formattedStr)
{
	if (MaxCountOfPlayersInGame <= 1) return; // multiplayer only
	if (!formattedStr || !formattedStr[0]) return;

	ChatHistory_AddMessage(formattedStr, PlayersMax_4, CHAT_MSG_SYSTEM);
}

// ---------------------------------------------------------------------------
// ChatHistory_Toggle — toggle overlay visibility
// ---------------------------------------------------------------------------
void ChatHistory_Toggle()
{
	if (MaxCountOfPlayersInGame <= 1) return;
	if (CurrentDialogIndex != 0 || Speech_IsPanelVisible) return; // NPC menu/dialogue takes priority

	IsChatHistoryVisible = !IsChatHistoryVisible;
	if (IsChatHistoryVisible) {
		// Close all conflicting UI panels before showing the overlay.
		// Mirrors the OpenInfoWindow() / ToggleCHARPanel() pattern in Panel.cpp
		// so that the chat history overlay is never drawn on top of open panels.
		if (IsGameMenuActive()) Menu_SetupMenuWindow(0, 0); // close ESC menu, consistent with other panels
		IsSpellBookVisible  = false;
		IsQUESTPanelVisible = false;
		IsCHARPanelVisible  = false;
		IsStashPanelVisible = false;
		IsHELPVisible       = 0;
		InfoWindow_Close();
		VisualTrade_Close();
		Craft_Close();
		Enchant_Close();
		Powerups_Close();
		Quench_Close();
		Transmute_Close();
		CloseInventoryPanel();
		ClosePerksPanel();

		chatHistoryScrollOffset = 0; // reset to newest
		ChatHistory_RebuildWrappedLines();
	}
	PlayGlobalSound(S_75_I_TITLEMOV);
}

// ---------------------------------------------------------------------------
// ChatHistory_ScrollUp — scroll towards newer messages (toward top, offset 0)
// ---------------------------------------------------------------------------
void ChatHistory_ScrollUp()
{
	if (!IsChatHistoryVisible) return;
	if (chatHistoryScrollOffset > 0)
		--chatHistoryScrollOffset;
}

// ---------------------------------------------------------------------------
// ChatHistory_ScrollDown — scroll towards older messages (deeper into history)
// ---------------------------------------------------------------------------
void ChatHistory_ScrollDown()
{
	if (!IsChatHistoryVisible) return;
	int visibleLines = ChatHistory_CalcVisibleLines();
	int maxScroll = wrappedLineCount > visibleLines ? wrappedLineCount - visibleLines : 0;
	if (chatHistoryScrollOffset < maxScroll)
		++chatHistoryScrollOffset;
}

// ---------------------------------------------------------------------------
// Ring buffer index helpers
// ---------------------------------------------------------------------------
// Get the index of the n-th newest entry (0 = most recent)
static int ChatHistory_GetEntryIndexFromNewest(int n)
{
	if (n < 0 || n >= chatHistoryCount) return -1;
	int idx = (chatHistoryWriteIndex - 1 - n + CHAT_HISTORY_MAX_MESSAGES * 2) % CHAT_HISTORY_MAX_MESSAGES;
	return idx;
}

// ---------------------------------------------------------------------------
// ChatHistory_CalcOverlayWidth
// ---------------------------------------------------------------------------
static int ChatHistory_CalcOverlayWidth()
{
	int maxWidth = ScreenWidth - CHAT_HISTORY_OVERLAY_MARGIN * 2;
	if (maxWidth > CHAT_HISTORY_OVERLAY_MAX_WIDTH)
		maxWidth = CHAT_HISTORY_OVERLAY_MAX_WIDTH;
	if (maxWidth < 200)
		maxWidth = 200;
	return maxWidth;
}

// ---------------------------------------------------------------------------
// ChatHistory_CalcVisibleLines
// ---------------------------------------------------------------------------
static int ChatHistory_CalcVisibleLines()
{
	// Space-first: compute how many content lines fit in the available vertical space,
	// accounting for all overlay overhead. This guarantees the window never overlaps the
	// bottom panel regardless of resolution (including 320x240).
	int available = ScreenHeight - GUI_MainPanelHeight - CHAT_HISTORY_OVERLAY_TOP_Y
	                - CHAT_HISTORY_HEADER_HEIGHT - CHAT_HISTORY_OVERLAY_PADDING * 2
	                - CHAT_HISTORY_FOOTER_EXTRA;
	int lines = available / CHAT_HISTORY_LINE_HEIGHT;
	if (lines > CHAT_HISTORY_MAX_VISIBLE_LINES) lines = CHAT_HISTORY_MAX_VISIBLE_LINES;
	if (lines < 1) lines = 1;
	return lines;
}

// ---------------------------------------------------------------------------
// ChatHistory_RebuildWrappedLines
// Rebuilds the wrapped line array from the ring buffer.
// Lines are stored newest-first: wrappedLines[0] = first line of newest message.
// ---------------------------------------------------------------------------
static void ChatHistory_RebuildWrappedLines()
{
	int overlayWidth = ChatHistory_CalcOverlayWidth() - CHAT_HISTORY_OVERLAY_PADDING * 2 - 10; // -10 for scrollbar
	lastWrapWidth = overlayWidth;
	wrappedLineCount = 0;

	// Iterate from newest to oldest
	for (int msgIdx = 0; msgIdx < chatHistoryCount; ++msgIdx) {
		int bufIdx = ChatHistory_GetEntryIndexFromNewest(msgIdx);
		if (bufIdx < 0) break;

		ChatHistoryEntry& entry = chatHistoryBuffer[bufIdx];
		if (!entry.used || !entry.text[0]) continue;

		const char* str = entry.text;
		int textLen = (int)strlen(str);
		int lineStart = 0;

		while (lineStart < textLen) {
			if (wrappedLineCount >= MAX_TOTAL_WRAPPED_LINES) goto done;

			// Scan forward to find how many chars fit in overlayWidth
			int pixelWidth = 0;
			int lastBreak = -1; // last space/word-break position
			int pos = lineStart;

			while (pos < textLen) {
				char rawChar = str[pos];
				if (rawChar == '\0') break;

				// (th4) CHI: a GB2312 pair is ONE character spanning two bytes. chiStep is
				// how far pos must advance; fontIdx is widened because CJK letterIDs reach 8434.
				int fontIdx, chiPixW;
				int chiStep = ChiNextSmall( (const uchar*)str + pos, &fontIdx, &chiPixW );
				int charWidth = chiPixW + 1;

				if (pixelWidth + charWidth > overlayWidth && pos > lineStart) {
					// Overflow
					if (lastBreak > lineStart) {
						pos = lastBreak + 1; // wrap at last space
					}
					// else: hard break at current position
					break;
				}

				pixelWidth += charWidth;
				if (fontIdx == 0) { // space character (fonttrans maps spaces to 0)
					lastBreak = pos;
				}
				pos += chiStep; // (th4) CHI: retire BOTH bytes of a pair, never split one
			}

			ChatWrappedLine& wl = wrappedLines[wrappedLineCount];
			wl.entryIndex  = bufIdx;
			wl.startOffset = lineStart;
			wl.length      = pos - lineStart;
			++wrappedLineCount;

			lineStart = pos;
		}
	}
done:;
}

// ---------------------------------------------------------------------------
// ChatHistory_IsAnyPanelOpen — check if any UI panel that could conflict is open
// ---------------------------------------------------------------------------
static bool ChatHistory_IsAnyPanelOpen()
{
	return IsINVPanelVisible
		|| IsCHARPanelVisible
		|| IsSpellBookVisible
		|| IsQUESTPanelVisible
		|| IsStashPanelVisible
		|| IsVisualTradePanelVisible
		|| IsCraftPanelVisible
		|| IsEnchantPanelVisible
		|| IsQuenchPanelVisible
		|| IsTransmutePanelVisible
		|| IsPowerupPanelVisible
		|| IsPerksPanelVisible
		|| IsInfoWindowVisible
		|| IsHELPVisible
		|| (CurrentDialogIndex != 0)
		|| SelectCurSpellMode
		|| AutomapMode
		|| TalkPanelMode;
}

// ---------------------------------------------------------------------------
// ChatHistory_DrawBorder — draw a thin 1px border around a rectangle
// ---------------------------------------------------------------------------
static void ChatHistory_DrawBorder(int xPos, int yPos, int width, int height, unsigned char color)
{
	// Horizontal lines (top and bottom)
	for (int edge = 0; edge < 2; ++edge) {
		int surfY = yPos + Screen_TopBorder + (edge == 0 ? 0 : height - 1);
		if (surfY < 0 || surfY >= WorkingHeight) continue;
		int baseOffset = YOffsetHashTable[surfY] + xPos + Screen_LeftBorder;

		for (int col = 0; col < width; ++col) {
			unsigned char* pixel = &WorkingSurface[baseOffset + col];
			if ((size_t)pixel >= (size_t)WorkingSurfaceDrawBegin && (size_t)pixel < (size_t)WorkingSurfaceDrawEnd) {
				*pixel = color;
				if (LightSurface) LightSurface[pixel - WorkingSurface] = 0;
				// (th4) Stage 2: same for the alpha overlay — the border is UI
				// ink and must never be ghosted (Rev 2.33).
				AlphaOverlayClearPixel(pixel);
				// (th4) Stage 3 (2026-09-05): same for the colored-light tint —
				// the border must not be multiplied by the world's light color.
				TintClearPixel(pixel);
			}
		}
	}
	// Vertical lines (left and right)
	for (int row = 0; row < height; ++row) {
		int surfY = yPos + Screen_TopBorder + row;
		if (surfY < 0 || surfY >= WorkingHeight) continue;
		int baseOffset = YOffsetHashTable[surfY] + xPos + Screen_LeftBorder;

		for (int edge = 0; edge < 2; ++edge) {
			int col = (edge == 0) ? 0 : width - 1;
			unsigned char* pixel = &WorkingSurface[baseOffset + col];
			if ((size_t)pixel >= (size_t)WorkingSurfaceDrawBegin && (size_t)pixel < (size_t)WorkingSurfaceDrawEnd) {
				*pixel = color;
				if (LightSurface) LightSurface[pixel - WorkingSurface] = 0;
				// (th4) Stage 2: same for the alpha overlay (Rev 2.33).
				AlphaOverlayClearPixel(pixel);
				// (th4) Stage 3 (2026-09-05): same for the colored-light tint.
				TintClearPixel(pixel);
			}
		}
	}
}

// ---------------------------------------------------------------------------
// ChatHistory_DrawText — draw a single line of text with color
// Uses DrawLevelInfoText which adds Screen_LeftBorder/Screen_TopBorder internally
// ---------------------------------------------------------------------------
static void ChatHistory_DrawTextLine(int x, int y, const char* text, int len, char color)
{
	// DrawLevelInfoText expects a null-terminated string.
	// We need to draw a substring, so copy to a temp buffer.
	char buf[CHAT_HISTORY_MAX_DISPLAY_CHARS];
	if (len <= 0) return;
	if (len >= (int)sizeof(buf)) len = (int)sizeof(buf) - 1;
	memcpy(buf, text, len);
	buf[len] = '\0';

	DrawLevelInfoText(x, y, buf, color);
}

// ---------------------------------------------------------------------------
// ChatHistory_Draw — main draw function, called from Draw.cpp
// ---------------------------------------------------------------------------
void __fastcall ChatHistory_Draw()
{
	if (!IsChatHistoryVisible) return;
	if (MaxCountOfPlayersInGame <= 1) {
		IsChatHistoryVisible = false;
		return;
	}
	if (!WorkingSurface) return;

	int overlayWidth  = ChatHistory_CalcOverlayWidth();
	int visibleLines  = ChatHistory_CalcVisibleLines();
	int contentWidth  = overlayWidth - CHAT_HISTORY_OVERLAY_PADDING * 2;

	// Check if wrapped lines need rebuild (resolution changed, or messages arrived while invisible)
	int expectedWrapWidth = contentWidth - 10;
	if (lastWrapWidth != expectedWrapWidth) {
		ChatHistory_RebuildWrappedLines();
	}

	// Calculate overlay height (space-first: CalcVisibleLines accounts for all overhead,
	// so overlayHeight is guaranteed to fit above the bottom panel at any supported resolution).
	int headerHeight = CHAT_HISTORY_HEADER_HEIGHT;
	int contentHeight = visibleLines * CHAT_HISTORY_LINE_HEIGHT;
	int overlayHeight = headerHeight + contentHeight + CHAT_HISTORY_OVERLAY_PADDING * 2 + CHAT_HISTORY_FOOTER_EXTRA;

	// Position: centered horizontally, near top
	int overlayX = (ScreenWidth - overlayWidth) / 2;
	int overlayY = CHAT_HISTORY_OVERLAY_TOP_Y;

	// Keep the overlay rect in sync so CursorIntoDisplayObject(ChatHistoryRect) works correctly.
	ChatHistoryRect.Left   = overlayX;
	ChatHistoryRect.Top    = overlayY;
	ChatHistoryRect.Width  = overlayWidth;
	ChatHistoryRect.Heigth = overlayHeight;
	ChatHistoryRect.Right  = overlayX + overlayWidth;
	ChatHistoryRect.Down   = overlayY + overlayHeight;

	// Clamp scroll offset
	int maxScroll = wrappedLineCount > visibleLines ? wrappedLineCount - visibleLines : 0;
	if (chatHistoryScrollOffset > maxScroll) chatHistoryScrollOffset = maxScroll;
	if (chatHistoryScrollOffset < 0) chatHistoryScrollOffset = 0;

	// Draw background (LUT-optimised: branch-free inner loop, see DrawBackground_LUT in InfoWindow.cpp).
	DrawBackground_LUT(overlayX + Screen_LeftBorder, overlayY + Screen_TopBorder, overlayWidth, overlayHeight);

	// Draw border
	ChatHistory_DrawBorder(overlayX, overlayY, overlayWidth, overlayHeight, CHAT_HISTORY_BORDER_COLOR);

	// Draw header text: "Chat History"
	// DrawLevelInfoText Y is the BOTTOM row of the glyph (font renders upward).
	// Center text vertically: set Y so glyph center aligns with header center.
	// Glyph ~13px tall, header center at overlayY + headerHeight/2.
	// Y_bottom = center + 13/2 = overlayY + headerHeight/2 + 6
	{
		const char* headerText = "Chat History";
		int textW = GetTextWidth(headerText);
		int headerX = overlayX + (overlayWidth - textW) / 2;
		int headerY = overlayY + headerHeight / 2 + 6;
		DrawLevelInfoText(headerX, headerY, headerText, C_3_Gold);
	}

	// Draw separator line below header
	{
		int sepY = overlayY + headerHeight - 1;
		int surfY = sepY + Screen_TopBorder;
		if (surfY >= 0 && surfY < WorkingHeight) {
			int baseOffset = YOffsetHashTable[surfY] + overlayX + Screen_LeftBorder;
			for (int col = 1; col < overlayWidth - 1; ++col) {
				unsigned char* pixel = &WorkingSurface[baseOffset + col];
				if ((size_t)pixel >= (size_t)WorkingSurfaceDrawBegin && (size_t)pixel < (size_t)WorkingSurfaceDrawEnd) {
					*pixel = CHAT_HISTORY_BORDER_COLOR;
					if (LightSurface) LightSurface[pixel - WorkingSurface] = 0;
					// (th4) Stage 3 (2026-09-05): the separator is UI ink — it
					// must not be multiplied by the world's light color.
					TintClearPixel(pixel);
				}
			}
		}
	}

	// Draw messages
	// wrappedLines[0] is first line of newest message.
	// With scrollOffset=0, we show wrappedLines[0..visibleLines-1].
	// With scrollOffset=N, we show wrappedLines[N..N+visibleLines-1].
	if (chatHistoryCount == 0 || wrappedLineCount == 0) {
		// Empty state
		const char* emptyText = "No messages yet.";
		int textW = GetTextWidth(emptyText);
		int textX = overlayX + (overlayWidth - textW) / 2;
		int textY = overlayY + headerHeight + CHAT_HISTORY_OVERLAY_PADDING + CHAT_HISTORY_CONTENT_TOP_EXTRA;
		DrawLevelInfoText(textX, textY, emptyText, C_0_White);
	} else {
		int textX = overlayX + CHAT_HISTORY_OVERLAY_PADDING;
		int textY = overlayY + headerHeight + CHAT_HISTORY_OVERLAY_PADDING + CHAT_HISTORY_CONTENT_TOP_EXTRA;

		for (int i = 0; i < visibleLines; ++i) {
			int lineIdx = chatHistoryScrollOffset + i;
			if (lineIdx >= wrappedLineCount) break;

			ChatWrappedLine& wl = wrappedLines[lineIdx];
			ChatHistoryEntry& entry = chatHistoryBuffer[wl.entryIndex];

			// Determine text color based on message type and sender
			char color;
			if (entry.type == CHAT_MSG_SYSTEM) {
				color = C_3_Gold; // System messages in gold
			} else if (entry.senderIndex == CurrentPlayerIndex) {
				color = C_1_Blue; // Own messages in blue
			} else {
				color = C_0_White; // Other players in white
			}

			ChatHistory_DrawTextLine(textX, textY + i * CHAT_HISTORY_LINE_HEIGHT,
				entry.text + wl.startOffset, wl.length, color);
		}
	}

	// Draw scrollbar indicator
	if (wrappedLineCount > visibleLines) {
		int scrollbarX = overlayX + overlayWidth - 6;
		int scrollbarTop = overlayY + headerHeight;
		int scrollbarHeight = contentHeight;

		// Draw track line (1px wide, faint)
		for (int row = 0; row < scrollbarHeight; ++row) {
			int surfY = scrollbarTop + row + Screen_TopBorder;
			if (surfY < 0 || surfY >= WorkingHeight) continue;
			unsigned char* pixel = &WorkingSurface[YOffsetHashTable[surfY] + scrollbarX + Screen_LeftBorder + 2];
			if ((size_t)pixel >= (size_t)WorkingSurfaceDrawBegin && (size_t)pixel < (size_t)WorkingSurfaceDrawEnd) {
				*pixel = CHAT_HISTORY_BORDER_COLOR;
				if (LightSurface) LightSurface[pixel - WorkingSurface] = 0;
				// (th4) Stage 3 (2026-09-05): scrollbar track is UI ink — no tint.
				TintClearPixel(pixel);
			}
		}

		// Draw thumb (5px wide, 1px tall)
		int thumbPos = maxScroll > 0
			? (chatHistoryScrollOffset * (scrollbarHeight - 1)) / maxScroll
			: 0;
		int thumbY = scrollbarTop + thumbPos + Screen_TopBorder;
		if (thumbY >= 0 && thumbY < WorkingHeight) {
			int baseOffset = YOffsetHashTable[thumbY] + scrollbarX + Screen_LeftBorder;
			for (int col = 0; col < 5; ++col) {
				unsigned char* pixel = &WorkingSurface[baseOffset + col];
				if ((size_t)pixel >= (size_t)WorkingSurfaceDrawBegin && (size_t)pixel < (size_t)WorkingSurfaceDrawEnd) {
					*pixel = CHAT_HISTORY_SCROLLBAR_COLOR;
					if (LightSurface) LightSurface[pixel - WorkingSurface] = 0;
					// (th4) Stage 3 (2026-09-05): scrollbar thumb is UI ink — no tint.
					TintClearPixel(pixel);
				}
			}
		}
	}

	// Draw scroll hint text at bottom if there are more messages.
	// Y is the bottom row of the glyph. The bottom border is at overlayY+overlayHeight-1.
	// Place hint 8px above the border so there is a visible gap (footer is CHAT_HISTORY_FOOTER_EXTRA
	// taller than pure content, giving breathing room between last message and hint).
	if (wrappedLineCount > visibleLines) {
		const char* hintText = "[Up/Down to scroll]";
		int textW = GetTextWidth(hintText);
		int hintX = overlayX + (overlayWidth - textW) / 2;
		int hintY = overlayY + overlayHeight - 8;
		DrawLevelInfoText(hintX, hintY, hintText, C_3_Gold);
	}
}
