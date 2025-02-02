#ifndef _GAME_H
#define _GAME_H

#define _TOP_HEIGHT_ 34  //Высота табло (где отображаются очки) в пикселах
#define _CELL_SIZE_ 26   //Размер стороны ячейки в пикселах (они все квадратные)

#define _NUMBER_H_ 24
#define _NUMBER_W_ 14
#define _POINTS_W_ 3

#define BEST_RESULTTS_NUM 5
#define MAX_NAME_LEN      29

bool isGameActivated();
bool isGameNotEnded();
void SetNewGameState();
void SetContinueGameState();
void SetEndGameState();

void InitScreen();
void LoadResources();
void FreeResources();

TEXTID BestResults();

void ShowGame(void *gc, RECT *rc);
void InitGame();
void FreeGame();
void WriteGame();
void GetGame();

void WriteInfo();
void GetInfo();

void UpdateCursor(int dx, int dy);
void onClickCursor();

void onEndGame();

#endif

