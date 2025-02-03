//заголовочный файл для библиотеки рисования и анимации
//инклудить в каждом файле, где будете использовать функции
#define GET_ALPHA(x) (((unsigned int)x>>24)&0xFF)
#define GET_BLUE(x) ((unsigned int)x&0xFF)
#define GET_GREEN(x) (((unsigned int)x>>8)&0xFF)
#define GET_RED(x) (((unsigned int)x>>16)&0xFF)
extern int mod(int i);
extern int lmod(int i);
extern void AnimSpeedMove(int*src,int dest, int speed);
extern void AnimCicleMove(int*src,int from, int to, int speed, int*st);
extern void DrawGradient(int x, int y, int x2, int y2, int upc, int loc);
extern void DrawGradientVert(int x, int y, int x2, int y2, int upc, int loc);
extern void DrawS(int Font,int text,int CEN, int XPos, int YPos, int MaxXPos, int MaxYPos, int BordColor,int NormalColor, int type);

