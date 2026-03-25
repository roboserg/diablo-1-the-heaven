#include "stdafx.h"

PathStep* Dword300Stek[300];
int Steps300StackIndex;

PathStep* PrevStep;
PathStep* CurrentStep;   

int StepsCount; 
int CurStepOrientIndex[MaxPathStep_25];

// A* star
//----- (00455B00) --------------------------------------------------------
int __fastcall FindPath( int (__fastcall *stepFunc)(int, int, int), int monsterIndex, int startRow, int startCol, int targetRow, int targetCol, char* steps )
{
	PathStep* curStep;    // eax@1
	PathStep** prevStep;  // eax@7
	//PathStep* curStep;  // edx@7
	int step;             // edi@8
	int result;           // eax@12
	int distance;              // esi@13
	//char v24;             // cl@1
	char isFirstStep;             // zf@10
	char nextOrient;             // cl@14

	StepsCount = 0;
	CurrentStep = NewStepFromPool();
	PrevStep = NewStepFromPool();
	Steps300StackIndex = 0;
		
	PathStep* nextStep = NewStepFromPool();
	char perimeter = GetPerimeter( startRow, startCol, targetRow, targetCol );
	nextStep->row = startRow;
	nextStep->col = startCol;
	nextStep->value2 = 0;
	nextStep->value0 = perimeter + nextStep->value2;
	nextStep->value1 = perimeter;
	
	CurrentStep->nextPtr = nextStep;

	curStep = GetNextStep();
	while( 1 ){
		if( !curStep ){
			return 0;
		}
		if( curStep->row == targetRow && curStep->col == targetCol ){
			break;
		}
		if( !GenerateSuccessors(stepFunc, monsterIndex, curStep, targetRow, targetCol) ){
			return 0;
		}
		curStep = GetNextStep();
	}
	prevStep = &curStep->prevPtr;
	int maxDistance = 0;
	if( *prevStep ){
		step = 0;//CurStepOrientIndex;
		do{
			if( maxDistance >= 25 ){
				break;
			}
			++maxDistance;
			CurStepOrientIndex[step] = StepOrientationIndex[3 * (curStep->col - (*prevStep)->col) + (curStep->row - (*prevStep)->row) + 4];
			curStep = *prevStep;
			++step;
			isFirstStep = (*prevStep)->prevPtr == 0;
			prevStep = &(*prevStep)->prevPtr;
		}while( !isFirstStep );
	}
	if( maxDistance == 25 ){
		return 0;
	}
	result = 0;
	if( maxDistance > 0 ){
		distance = maxDistance;
		do{
			nextOrient = CurStepOrientIndex[distance - 1];
			--distance;
			steps[result++] = nextOrient;
		}while( result < maxDistance );
	}
	return result;
}

//----- (00455C10) --------------------------------------------------------
int __fastcall GetPerimeter( int aDestX, int aDestY, int X, int Y )
{
	int rowDest_1 = abs(aDestX - X);
	int colDest = abs(aDestY - Y);
	int rowDest = rowDest_1;
	if( rowDest_1 >= colDest ){
		rowDest = colDest;
	}
	if( rowDest_1 > colDest ){
		colDest = rowDest_1;
	}
	return 2 * (rowDest + colDest);
}

//----- (00455C43) --------------------------------------------------------
int __fastcall Step_IsOnLine(PathStep* step, int x, int y)
{
	if( step->row == x || step->col == y ){
		return 2;
	}else{
		return 3;
	}
}

//----- (00455C5B) --------------------------------------------------------
PathStep* GetNextStep()
{
	PathStep* nextPtr = CurrentStep->nextPtr;
	if( nextPtr ){
		CurrentStep->nextPtr = nextPtr->nextPtr;
		nextPtr->nextPtr = PrevStep->nextPtr;
		PrevStep->nextPtr = nextPtr;
	}
	return nextPtr;
}

// похоже проверка на то, можно ли двигаться по диагонали.
//----- (00455C85) --------------------------------------------------------
bool __fastcall CheckBlockStep( PathStep* step, int row, int col )
{
	int orientation = StepOrientationIndex[3 * (col - step->col) + (row - step->row) + 4] - 5;
	if( orientation == 0 ){
		if( !TileBlockWalking[FineMap[ row ][ col + 1 ]]
			&& !TileBlockWalking[FineMap[ row + 1 ][ col ]] ){
				return true;
		}else{
			return false;
		}
	}
	if( orientation == 1 ){
		if( !TileBlockWalking[FineMap[ row ][ col + 1 ]]
			&& !TileBlockWalking[FineMap[ row - 1 ][ col ]] ){
				return true;
		}else{
			return false;
		}
	}
	if( orientation == 2 ){
		if( !TileBlockWalking[FineMap[ row ][ col - 1 ]]
			&& !TileBlockWalking[FineMap[ row - 1 ][ col ]] ){
				return true;
		}else{
			return false;
		}
	}
	if( orientation == 3 ){
		if( !TileBlockWalking[FineMap[ row + 1 ][ col ]]
			&& !TileBlockWalking[FineMap[ row ][ col - 1 ]] ){
				return true;
		}else{
			return false;
		}
	}
	return true;
}

//----- (00455D2C) --------------------------------------------------------
int __fastcall GenerateSuccessors(int(__fastcall *stepFunc)(int, int, int), int charIndex, PathStep *step, int targetX, int targetY)
{
	int nextY;                                // edi@3
	int nextX;                                // esi@3
	PathStep* nextStep;                       // eax@3
	int result = 1;                           // no path
	for( int curDir = 0; curDir < 8; curDir++ ){
		// get step direction                 🡳  🡷  🡰   🡴  🡱  🡵   🡲  🡶 
		static const int stepDeltaRow[8] = { -1, -1,  1,  1, -1,  0,  1,  0 };
		static const int stepDeltaCol[8] = { -1,  1, -1,  1,  0, -1,  0,  1 };
		nextX = step->row + stepDeltaRow[curDir];
		nextY = step->col + stepDeltaCol[curDir];
		if( nextX >= 0 && nextY >= 0 ){
			nextStep = (PathStep*)stepFunc(charIndex, nextX, nextY);
			if( (nextStep && CheckBlockStep(step, nextX, nextY)) || (!nextStep && nextX == targetX && nextY == targetY) ){
				if( !NextAstarWave(step, nextX, nextY, targetX, targetY) ){
					result = 0; // ok
					break;
				}
			}
		}
	}
	return result;
}

//----- (00455DB5) --------------------------------------------------------
int __fastcall NextAstarWave(PathStep* step, int nextX, int nextY, int targetX, int targetY)
{
	PathStep *step_1; // edi@1
	int stepLine; // ebx@1
	PathStep *curStep; // esi@1
	int stepIndex; // eax@2
	PathStep **steps; // ecx@2
	char v10; // al@7
	PathStep *prevStep; // esi@8
	int prevStepIndex; // eax@9
	PathStep **prevSteps; // ecx@9
	signed __int8 v14; // al@14
	PathStep *newStep; // eax@15
	PathStep *lastStep; // esi@15
	signed __int8 dist; // al@16
	int stepIndex3; // ecx@16
	PathStep **steps3; // eax@16
	int nextX_1; // [sp+Ch] [bp-4h]@1

	nextX_1 = nextX;
	step_1 = step;
	stepLine = step->value2 + Step_IsOnLine( step, nextX, nextY );
	curStep = FindCurrentStepForXY( nextX_1, nextY );
	if( curStep ){
		stepIndex = 0;
		steps = step_1->subSteps;
		do{
			if( !*steps ){
				break;
			}
			++stepIndex;
			++steps;
		}while( stepIndex < 8 );
		step_1->subSteps[ stepIndex ] = curStep;
		if( stepLine < curStep->value2 ){
			if( CheckBlockStep( step_1, nextX_1, nextY ) ){
				v10 = curStep->value1;
				curStep->prevPtr = step_1;
				curStep->value2 = stepLine;
				curStep->value0 = stepLine + v10;
			}
		}
	}else{
		prevStep = FindPrevStepForXY( nextX_1, nextY );
		if( prevStep ){
			prevStepIndex = 0;
			prevSteps = step_1->subSteps;
			do{
				if( !*prevSteps ){
					break;
				}
				++prevStepIndex;
				++prevSteps;
			}while( prevStepIndex < 8 );
			step_1->subSteps[ prevStepIndex ] = prevStep;
			if( stepLine < prevStep->value2 && CheckBlockStep( step_1, nextX_1, nextY ) ){
				v14 = stepLine + prevStep->value1;
				prevStep->prevPtr = step_1;
				prevStep->value2 = stepLine;
				prevStep->value0 = v14;
				LookNextStep( prevStep );
			}
		}else{
			newStep = NewStepFromPool();
			lastStep = newStep;
			if( !newStep ){
				return 0;
			}
			newStep->prevPtr = step_1;
			newStep->value2 = stepLine;
			dist = GetPerimeter( nextX_1, nextY, targetX, targetY );
			lastStep->value1 = dist;
			lastStep->row = nextX_1;
			lastStep->value0 = dist + stepLine;
			lastStep->col = nextY;
			InsertNextPathStep( lastStep );
			stepIndex3 = 0;
			steps3 = step_1->subSteps;
			do{
				if( !*steps3 ){
					break;
				}
				++stepIndex3;
				++steps3;
			}while( stepIndex3 < 8 );
			step_1->subSteps[ stepIndex3 ] = lastStep;
		}
	}
	return 1;
}

//----- (00455EE0) --------------------------------------------------------
PathStep* __fastcall FindCurrentStepForXY( int row, int col )
{
	PathStep* curPtr = CurrentStep;
	while( 1 ){
		curPtr = curPtr->nextPtr;
		if( !curPtr ){
			break;
		}
		if( curPtr->row == row && curPtr->col == col ){
			return curPtr;
		}
	}
	return 0;
}

//----- (00455EFA) --------------------------------------------------------
PathStep* __fastcall FindPrevStepForXY( int row, int col )
{
	PathStep* curPtr = PrevStep;
	while( 1 ){
		curPtr = curPtr->nextPtr;
		if( !curPtr ){
			break;
		}
		if( curPtr->row == row && curPtr->col == col ){
			return curPtr;
		}
	}
	return 0;
}

//----- (00455F14) --------------------------------------------------------
void __fastcall InsertNextPathStep(PathStep* lastPtr)
{
	PathStep* nextPtr = CurrentStep->nextPtr;
	PathStep* curPtr = CurrentStep;
	if( nextPtr ){
		while( nextPtr && nextPtr->value0 < lastPtr->value0 ){
			curPtr = nextPtr;
			nextPtr = nextPtr->nextPtr;
		}
		lastPtr->nextPtr = nextPtr;
		curPtr->nextPtr = lastPtr;
	}else{
		CurrentStep->nextPtr = lastPtr;
	}
}

//----- (00455F45) --------------------------------------------------------
void __fastcall LookNextStep( PathStep* step )
{
	AddToDword300Stack(step);
	while( Steps300StackIndex ){

		PathStep* prevPtr = DeleteFromDword300Stek();
		
		for( int orientation = 0; orientation < 8; orientation++ ){
			PathStep* curStep = prevPtr->subSteps[orientation];
			if( !curStep ){
				break;
			}
			int result2 = prevPtr->value2 + Step_IsOnLine(prevPtr, curStep->row, curStep->col);// TODO: что за нафиг? с обоих сторон выражения какое то value
			if( result2 >= curStep->value2 ){
				continue;
			}
			if( !CheckBlockStep(prevPtr, curStep->row, curStep->col) ){
				continue;
			}
			int row = curStep->row;
			int col = curStep->col;
			curStep->prevPtr = prevPtr;
			char v7 = prevPtr->value2 + Step_IsOnLine(prevPtr, row, col);
			curStep->value2 = v7;
			curStep->value0 = v7 + curStep->value1;
			AddToDword300Stack(curStep);
		}

	}
}

//----- (00455FD8) --------------------------------------------------------
int __fastcall AddToDword300Stack( PathStep* a1 )
{
	Dword300Stek[Steps300StackIndex++] = a1;
	return Steps300StackIndex - 1;
}

//----- (00455FEB) --------------------------------------------------------
PathStep* DeleteFromDword300Stek()
{
	return Dword300Stek[--Steps300StackIndex];
}

//----- (00455FFE) --------------------------------------------------------
PathStep* NewStepFromPool()
{
	PathStep* newStep;
	if( StepsCount == 300 ){
		newStep = 0;
	}else{
		newStep = &StepsPool[StepsCount++];
		memset(newStep, 0, sizeof( PathStep ));
	}
	return newStep;
}
