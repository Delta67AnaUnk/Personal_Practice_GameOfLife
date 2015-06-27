#ifndef GAMEMAIN_H_INCLUDED
#define GAMEMAIN_H_INCLUDED

#define UNICODE

#include <cstddef>
#include <windows.h>
#include "D2Graphic.h"


extern HWND MainWnd;

extern bool Borderless;

extern bool Pause;

extern bool DoublePace;


struct cell{
    bool alive;
    unsigned char nblive;
    cell();
};

extern void Border();
extern void Init(HWND);
extern void Flush();
extern void Clear();
extern void ClearNb();
extern void Update();
extern void Draw(LPGraphicDevice lp);

struct saveseed{
    WORD x;
    WORD y;
};

extern bool SaveSeed();
extern bool LoadSeed();

extern cell *Matrix[MSIZE+2][MSIZE+2];

extern GameSprites *sprites[MSIZE][MSIZE];


#endif // GAMEMAIN_H_INCLUDED
