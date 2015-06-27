#include "GameMain.h"
#include <iostream>

using namespace std;

bool Borderless;

bool Pause;

bool DoublePace;


cell::cell():alive(false),nblive(0){}

cell *Matrix[MSIZE+2][MSIZE+2];

cell *rubbish;

GameSprites *sprites[MSIZE][MSIZE];

void Border(){
    if(Borderless){
        for(int i=1;i<MSIZE+1;i++){
            Matrix[i][0] = Matrix[i][MSIZE];
            Matrix[i][MSIZE+1] = Matrix[i][1];
            Matrix[0][i] = Matrix[MSIZE][i];
            Matrix[MSIZE+1][i] = Matrix[1][i];
        }
        Matrix[0][0] = Matrix[MSIZE][MSIZE];
        Matrix[MSIZE+1][MSIZE+1] = Matrix[1][1];
        Matrix[0][MSIZE+1] = Matrix[MSIZE][1];
        Matrix[MSIZE+1][0] = Matrix[1][MSIZE];
    }else{
        for(int i=1;i<MSIZE+1;i++){
            Matrix[i][0] = rubbish;
            Matrix[i][MSIZE+1] = rubbish;
            Matrix[0][i] = rubbish;
            Matrix[MSIZE+1][i] = rubbish;
        }
        Matrix[0][0] = rubbish;
        Matrix[MSIZE+1][MSIZE+1] = rubbish;
        Matrix[0][MSIZE+1] = rubbish;
        Matrix[MSIZE+1][0] = rubbish;
    }
}


void Init(HWND disp){
    gDevice = new D3D9GraphicDevice();
    gDevice->Init(disp);
    Borderless = false;
    for(int i=0;i<MSIZE;i++){
        for(int j=0;j<MSIZE;j++){
            Matrix[i+1][j+1] = new cell();
            sprites[i][j] = new GameSprites(i*10+1,j*10+1);
            sprites[i][j]->Init(gDevice->dev);
        }
    }
    rubbish = new cell();
    Border();
    Pause = true;
    DoublePace = false;
    for(int i=0;i<MSIZE-1;i++){
        grpt[i*4+0].x = (i+1)*PIX;
        grpt[i*4+0].y = 0;
        grpt[i*4+0].Fill();
        grpt[i*4+1].x = (i+1)*PIX;
        grpt[i*4+1].y = MSIZE*PIX+1;
        grpt[i*4+1].Fill();
        grpt[i*4+2].y = (i+1)*PIX;
        grpt[i*4+2].x = 0;
        grpt[i*4+2].Fill();
        grpt[i*4+3].y = (i+1)*PIX;
        grpt[i*4+3].x = MSIZE*PIX+1;
        grpt[i*4+3].Fill();
    }
}

void Flush(){
    for(int i=1;i<MSIZE+1;i++){
        for(int j=1;j<MSIZE+1;j++){
            delete Matrix[i][j];
            delete sprites[i-1][j-1];
        }
    }
    delete rubbish;
    tex->Release();
}

void Clear(){
    for(int i=1;i<MSIZE+1;i++){
        for(int j=1;j<MSIZE+1;j++){
            Matrix[i][j]->alive = false;
            Matrix[i][j]->nblive = 0;
        }
    }
    rubbish->nblive = 0;
}

void ClearNb(){
    for(int i=1;i<MSIZE+1;i++){
        for(int j=1;j<MSIZE+1;j++){
            Matrix[i][j]->nblive = 0;
        }
    }
    rubbish->nblive = 0;
}

void Update(){
    int i,j;

    ClearNb();
    for(i=1;i<MSIZE+1;i++){
        for(j=1;j<MSIZE+1;j++){
            if(Matrix[i][j]->alive){
                ++(Matrix[i-1][j-1]->nblive);
                ++(Matrix[i-1][j]->nblive);
                ++(Matrix[i-1][j+1]->nblive);
                ++(Matrix[i][j-1]->nblive);
                ++(Matrix[i][j+1]->nblive);
                ++(Matrix[i+1][j-1]->nblive);
                ++(Matrix[i+1][j]->nblive);
                ++(Matrix[i+1][j+1]->nblive);
            }
        }
    }
    for(i=1;i<MSIZE+1;i++){
        for(j=1;j<MSIZE+1;j++){
            if(Matrix[i][j]->alive){
                if(!(Matrix[i][j]->nblive==2)&&!(Matrix[i][j]->nblive==3))Matrix[i][j]->alive = false;
            }else{
                if(Matrix[i][j]->nblive==3)Matrix[i][j]->alive = true;
            }
        }
    }
}

void Draw(LPGraphicDevice lpgDevice){
    lpgDevice->Clear(D3DCOLOR_XRGB(129,199,212));
    lpgDevice->Begin();
    //lpgDevice->Grid();
    for(int i=0;i<MSIZE;i++){
        for(int j=0;j<MSIZE;j++){
            if(Matrix[i+1][j+1]->alive)
                sprites[i][j]->Draw();
        }
    }

    lpgDevice->End();
    lpgDevice->Present();
}

bool SaveSeed(){
    wchar_t namepath[1024] = {0};
    OPENFILENAME opf;
    ZeroMemory(&opf,sizeof(OPENFILENAME));
    opf.lStructSize = sizeof(OPENFILENAME);
    opf.lpstrFilter = L"GameSeed (*.gls)\0*.gls\0";
    opf.hwndOwner = MainWnd;
    opf.lpstrFile = namepath;
    opf.nMaxFile = 1023;
    opf.lpstrDefExt = L"gls";
    opf.lpstrInitialDir = L"seed\\";
    opf.Flags = OFN_EXPLORER|OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY;
    GetSaveFileName(&opf);
    if(namepath[0] == 0) return true;
    HANDLE fp = CreateFile(namepath,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
    DWORD checkbit;
    saveseed sd;
    for(int i = 1;i<MSIZE+1;i++){
        for(int j = 1;j<MSIZE+1;j++){
            if(Matrix[i][j]->alive){
                sd.x = i;
                sd.y = j;
                if(  !WriteFile(fp,&sd,sizeof(saveseed),&checkbit,NULL)||(checkbit!=sizeof(saveseed))  ){
                    CloseHandle(fp);
                    return false;
                }
            }
        }
    }

    sd.x = 999;
    sd.y = 999;
    if(  !WriteFile(fp,&sd,sizeof(saveseed),&checkbit,NULL)||(checkbit!=sizeof(saveseed))  ){
        CloseHandle(fp);
        return false;
    }
    CloseHandle(fp);
    return true;
}

bool LoadSeed(){
    wchar_t namepath[1024] = {0};
    OPENFILENAME opf;
    ZeroMemory(&opf,sizeof(OPENFILENAME));
    opf.lStructSize = sizeof(OPENFILENAME);
    opf.lpstrFilter = L"GameSeed (*.gls)\0*.gls\0";
    opf.hwndOwner = MainWnd;
    opf.lpstrFile = namepath;
    opf.nMaxFile = 1023;
    opf.lpstrDefExt = L"gls";
    opf.lpstrInitialDir = L"seed\\";
    opf.Flags = OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
    GetOpenFileName(&opf);
    if(namepath[0] == 0) return true;
    HANDLE fp = CreateFile(namepath,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    DWORD checkbit;
    saveseed sd;
    sd.x=1;
    sd.y=1;
    Clear();
    while(true){
        if(  !ReadFile(fp,&sd,sizeof(saveseed),&checkbit,NULL)||(checkbit!=sizeof(saveseed))  ){
            CloseHandle(fp);
            return false;
        }else{
            if(sd.x!=999)
                Matrix[sd.x][sd.y]->alive = true;
            else
                break;
        }
    }
    CloseHandle(fp);
    return true;
}
