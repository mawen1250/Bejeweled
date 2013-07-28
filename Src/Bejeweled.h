#ifndef  __BEJEWELED_H
#define  __BEJEWELED_H


#define sint8    signed char
#define sint16   signed short
#define sint32   signed long
#define uint8  unsigned char
#define uint16 unsigned short
#define uint32 unsigned long
/*
#define blX  978
#define blY 1100
#define brX 7614
#define brY 1100
#define tlX  996
#define tlY 7200
#define trX 7554
#define trY 7200
#define Xul 7660
#define Xbl  880
#define Yul 8000
#define Ybl  380
*/
#define blX 1320
#define blY  400
#define brX 7400
#define brY  400
#define tlX 1320
#define tlY 3030
#define trX 7400
#define trY 3030
#define Xul 8000
#define Xbl  880
#define Yul 4000
#define Ybl  380

#define pXr  240
#define pYr  320


extern sint16 s16max(sint16 a, sint16 b);
extern sint16 s16min(sint16 a, sint16 b);
extern sint16 ClippX(sint16 pX);
extern sint16 ClippY(sint16 pY);
extern sint32 s32RoundDiv(sint32 a, sint32 b);
extern uint8 IsTouch(uint16 tX, uint16 tY);
extern sint16 GetpX(uint16 tX, uint16 tY, uint8 isT);
extern sint16 GetpY(uint16 tX, uint16 tY, uint8 isT);
extern uint8 GetpXY(sint16 *pX, sint16 *pY);
extern uint8 pX2BjH(sint16 pX);
extern uint8 pY2BjV(sint16 pY);

extern void Bootup(unsigned int *count_seed);
extern void Bejeweled(unsigned int *count_seed);


#endif