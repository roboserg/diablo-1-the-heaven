#include "stdafx.h"

char* FontSpriteMid;
char* TextBoxCel;
int Speech_CurLineY;
char* Speech_FirstDrawingLine;
char Speech_IsPanelVisible;
// 004A7914
int Speech_ScrollSteps[11] = { 0, 2, 4, 6, 8, 0, -1, -2, -3, -4, 0	};
int Speech_ScrollCurStep;
int Speech_ScrollStep;
i64 Speech_PrevTick;

//----- (0042E4F5) --------------------------------------------------------
void FreeSpeechPanelPtrs()
{
	FreeMemZero(FontSpriteMid);
	FreeMemZero(TextBoxCel);
}

//----- (0042E519) --------------------------------------------------------
void LoadSpeechPanelData()
{
	FontSpriteMid = (char*)LoadFile("Data\\MedTextS.CEL");
	TextBoxCel = (char*)LoadFile("Data\\TextBox.CEL");
	Speech_IsPanelVisible = 0;
}

//----- (0042E543) --------------------------------------------------------
void __fastcall StartSpeech( int speechIndex )
{
	Speech& speech = Speeches[ speechIndex ];
	if( speech.HasText ){
		IsQUESTPanelVisible = false;
		Speech_FirstDrawingLine = speech.TextPtr;
		Speech_IsPanelVisible = true;
		Speech_CurLineY = Screen_TopBorder + Dialog_302_Height + (ScreenHeight - Dialog_302_Height - GUI_MainPanelHeight) / 2 + Speech_TopMargin;
		Speech_ScrollStep = Speech_ScrollSteps[ speech.stepIndex ];
		Speech_ScrollCurStep = Speech_ScrollStep;
		Speech_PrevTick = GetTickCountTh(); // это таймер не влияет на гемплей, не перехватываем
	}
	PlayGlobalSound( speech.SoundIndex );
}

//----- (0042E600) --------------------------------------------------------
void __fastcall DrawLetterInSpeech( int xPos, int yPos, void* fontSet, int letterNumber )
{
	int letterIndex = letterNumber - 1;
	int* fontData = (int*)fontSet;
	int* lettersOffsets = &fontData[1];
	int letterOffset = lettersOffsets[letterIndex];
	int nextLetterOffset = lettersOffsets[letterIndex + 1];
	int length = nextLetterOffset - letterOffset;
	char* pictureData = (char*)fontSet + letterOffset;
	int linePixelsLeft = Speech_LetterMaxWidth;

	uchar* curPixelPtr = WorkingSurface + YOffsetHashTable[yPos] + xPos;
	int dialogBottomY = Screen_TopBorder + Dialog_302_Height + (ScreenHeight - Dialog_302_Height - GUI_MainPanelHeight) / 2;
	uchar* speechTop = WorkingSurface + YOffsetHashTable[ dialogBottomY - Dialog_302_Height + Speech_TopMargin ];
	uchar* speechBottom = WorkingSurface + YOffsetHashTable[ dialogBottomY - Speech_TopMargin ];

	for( int row = 0; row < length; ){
		char curCode = pictureData[row++];
		if( curCode >= 0 && ( curPixelPtr < speechTop || curPixelPtr > speechBottom ) ){ // высота запрещенная 
			row += curCode;
			curCode = -curCode;// сразу пропускаем все байты 
		}
		if( curCode < 0 ){ // Пропустить (-curCode) байт 
			curPixelPtr -= curCode;
			linePixelsLeft += curCode;
			if( linePixelsLeft <= 0 ){
				curPixelPtr -= YOffsetHashTable[1] + Speech_LetterMaxWidth; // подъем на строку выше 
				linePixelsLeft = Speech_LetterMaxWidth;
			}
		}else{ // рисуем curCode пикселей
			for( int j = 0; j < curCode; j++ ){
				*curPixelPtr++ = pictureData[row++];
				if( --linePixelsLeft <= 0 ){
					curPixelPtr -= YOffsetHashTable[1] + Speech_LetterMaxWidth; // подъем на строку выше 
					linePixelsLeft = Speech_LetterMaxWidth;
				}
			}
		}
	}
}

// Отрисовка плавного поднимающегося текста речи NPC 
//----- (0042E6A2) --------------------------------------------------------
void DrawSpeech()
{
	int leftX = Screen_LeftBorder + (ScreenWidth - Dialog_591_Width) / 2 + Speech_LeftMargin;
	int bottomY = Screen_TopBorder + Dialog_302_Height + (ScreenHeight - Dialog_302_Height - GUI_MainPanelHeight) / 2 + Speech_TopMargin; // нижняя координата отрисовки речи
	char* curChar = Speech_FirstDrawingLine; // текущая рисуемая буква = первая буква верхней рисуемой строки 
	char* secondDrawingLine = Speech_FirstDrawingLine; // первая буква второй рисуемой строки 
	char curLine[ Speech_LineMaxLen ]; // буфер рисуемой локализованной по кодовой странице строки
	
	int dialogW = Dialog_591_Width;
	int dialogH = Dialog_302_Height;
	int xPos = (ScreenWidth - dialogW) / 2;
	int yPos = dialogH + ((ScreenHeight - dialogH - GUI_MainPanelHeight) / 2) + 8;
	DrawDialogBox( dialogW, dialogH, TextBoxCel, xPos, yPos );
	
	int charY = Speech_CurLineY; // верхний пиксель строки 
	for( char* nextChar = curChar; *nextChar != '|' && charY < bottomY; charY += Speech_LineHeight ){ // рисуем все видимые в окне строки текста до низа 
		int curLineWidth = 0; // ширина выводимой строки в пикселях 
		int lineLen = 0; // длина рисуемой строки 
		for( nextChar = curChar; *nextChar != '|' && *nextChar != '\n' && curLineWidth < Dialog_591_Width - Speech_LeftMargin * 2 && lineLen < Speech_LineMaxLen; nextChar++ ){ // локализуем и заполняем до максимальной ширины заполняемую строку
			if( char localChar = Codepage[ *nextChar ] ){ // нулевые символы пропускаем 
				curLine[ lineLen++ ] = localChar; // заполняем строку и считаем её длину 
				curLineWidth += FontWidthMid[ FontIndexMid[ localChar ] ] + Speech_Spacing; // считаем ширину строки в пикселях 
			}
		}
		if( *nextChar != '|' && *nextChar != '\n' ){ // если текст не закончился и следущий символ не перевод строки 
			for( ; lineLen > 0 && curLine[ lineLen - 1 ] != ' '; lineLen-- ); // обрезаем справа до ближайшего пробела чтобы не было разрыва слова 
		}
		curLine[ lineLen ] = 0; // завершаем выводимую строку нулём 
		int charX = leftX; // левый пиксель строки 
		for( char* localChar = curLine; *localChar != 0; ++localChar ){ // отрисовка строки 
			uchar charImageNum = FontIndexMid[ *localChar ]; // номер изображения буквы 
			if( charImageNum ){ // если не пробельный символ 
				DrawLetterInSpeech( charX, charY, FontSpriteMid, charImageNum ); // отрисовка буквы 
			}
			charX += FontWidthMid[ charImageNum ] + Speech_Spacing; // двигаем пиксельную каретку вправо (включая пробельные символы ) 
			if( *(++curChar) == '\n' ){ // двигаем каретку исходного текста 
				++curChar; // переводы строк были пропущены при локализации 
			}
		}
		if( secondDrawingLine == Speech_FirstDrawingLine ){ // если вывели первую строку 
			secondDrawingLine = curChar; // запоминаем начало второй 
		}
	}
	if( IsMainDraw ){
		i64 curTick = GetTickCountTh(); // тик текущего кадра  // это таймер не влияет на гемплей, не перехватываем
		do{ // плавная прокрутка 
			if( Speech_ScrollStep > 0 ){ // положительные значения для попиксельного скроллинга внтури шага 
				if( Speech_ScrollCurStep-- > 1 ){ // пока не сдвинулись на все пиксели шага 
					Speech_CurLineY--; // попиксельный скроллинг 
				}else{
					Speech_ScrollCurStep = Speech_ScrollStep; // начинаем попиксельный шаг заново 
				}
			}else{ // отрицательные значения для скроллинга на весь шаг в несколько пикселей за кадр 
				Speech_CurLineY -= -Speech_ScrollStep + 1; // пошаговый скроллинг 
			}
		}while( Speech_CurLineY > bottomY - Dialog_302_Height && curTick >= (Speech_PrevTick += MAX_FPS / ENGINE_FPS) ); // пока не верхняя граница текста 
		if( curTick >= Speech_PrevTick ){ // Speech_LineScrollTime (50) миллисекунд на прокрутку одной строку вверх ( с проверкой на случай переполнения ) 
			Speech_CurLineY += Speech_LineHeight; // переходим на отслеживание верха следущей строки 
			Speech_FirstDrawingLine = secondDrawingLine; // отбрасываем верхнюю строку уползшую за верхний край 
			if( *Speech_FirstDrawingLine == '|' ){ // если текст закончился 
				Speech_IsPanelVisible = 0; // закрываем речевое окно 
			}
		}
	}
}
