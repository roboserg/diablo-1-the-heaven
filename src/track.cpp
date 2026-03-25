#include "stdafx.h"

// track state on/off
bool IsTracking;
bool MouseDown;

// time tracking last performed
#define TRACK_START_DELAY	250_ms
#define TRACK_LOOP_DELAY	300_ms
i64 TrackTime;

//----- (00477A95) --------------------------------------------------------
void TrackMouse()
{
	const CObjectUnderCursor& top = Cur;
	const Player& player = Players[CurrentPlayerIndex];
	// don't track if the mouse isn't down
	if( MouseDown && In112(top.Row, top.Col) ){
		// don't track if the player isn't in a stand state or late walk		
		if( player.currentActionFrame > 6 || player.CurAction == PCA_0_STAND ){
			// don't track if mouse still on same target
			if( top.Row != player.TargetRow || top.Col != player.TargetCol ){
				// don't track if it hasn't been long enough since we last tracked
				i64 newTick = GetTickCountTh(); // перехватываем вызов
				if( newTick - TrackTime >= 300_ms ){ // TRACK_LOOP_DELAY
					TrackTime = newTick;
					// track!
					NetSendCmdLoc( PRIORITY_LOW, CMD_1_WALKXY, top.Row, top.Col );
					IsTracking = true;
				}
			}
		}
	}
}

//----- (00477B33) -------------------------------------------------------- net
void TrackInit( bool mouseDown)
{
	if( MouseDown != mouseDown ){
		MouseDown = mouseDown;
		if( mouseDown ){
			// indicate we haven't moved from click mode to track mode yet
			IsTracking = false;
			// make next track time occur after TRACK_START_DELAY
			// instead of TRACK_LOOP_DELAY
			TrackTime = GetTickCountTh() - 50_ms; // + TRACK_START_DELAY - TRACK_LOOP_DELAY
			// start tracking immediately by pumping a command into queue
            NetSendCmdLoc( PRIORITY_LOW, CMD_1_WALKXY, Cur.Row, Cur.Col );
		}else{
			// turn off track mode
		    IsTracking = false;
		}
	}
}
