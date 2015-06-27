#include "D2Graphic.h"
#include <iostream>
using namespace std;

LPGraphicDevice gDevice;

gridpoint grpt[MSIZE*4-4];

D3D9GraphicDevice::D3D9GraphicDevice(){

}

D3D9GraphicDevice::~D3D9GraphicDevice(){
    if(dev){
        dev->Release();
        dev = NULL;
    }
    if(direct3d){
        direct3d->Release();
        direct3d = NULL;
    }
    if(tex){
        tex->Release();
        tex = NULL;
    }
}


void D3D9GraphicDevice::Init(HWND hWnd){
    direct3d = Direct3DCreate9(D3D_SDK_VERSION);
    D3DPRESENT_PARAMETERS pparam;
    ZeroMemory(&pparam,sizeof(pparam));

    pparam.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pparam.hDeviceWindow = hWnd;
    pparam.Windowed = true;

    direct3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,D3DCREATE_MIXED_VERTEXPROCESSING,&pparam,&dev);
    D3DXCreateTextureFromFileEx(
                dev,"sprt.png",9,9,
                D3DX_DEFAULT,0,D3DFMT_UNKNOWN,
                D3DPOOL_MANAGED,D3DX_DEFAULT,
                D3DX_DEFAULT,0,NULL,NULL,&tex);
//    D3DXCreateTextureFromResourceEx(
//                dev,NULL,MAKEINTRESOURCE(PIC),9,9,
//                D3DX_DEFAULT,0,D3DFMT_UNKNOWN,
//                D3DPOOL_MANAGED,D3DX_DEFAULT,
//                D3DX_DEFAULT,0,NULL,NULL,&tex);
}

void D3D9GraphicDevice::Clear(D3DCOLOR clr){
    dev->Clear(0,NULL,D3DCLEAR_TARGET,clr,1.0f,0);
}

void D3D9GraphicDevice::Grid(){
    dev->DrawPrimitiveUP(D3DPT_LINELIST,MSIZE*2-2,grpt,sizeof(gridpoint));
}

//=================================================================================
LPDIRECT3DTEXTURE9 tex;

GameSprites::GameSprites(){
    pos.x = 0;
    pos.y = 0;
    pos.z = 0;
    color = D3DCOLOR_ARGB(255,255,255,255);
    inited = false;
}

GameSprites::GameSprites(int xx, int yy){
    pos.x = xx;
    pos.y = yy;
    pos.z = 0;
    color = D3DCOLOR_ARGB(255,255,255,255);
    inited = false;
}

bool GameSprites::Init(LPDIRECT3DDEVICE9 dev){
    if(FAILED(  D3DXCreateSprite(dev,&sprite)  )){
        return false;
    }
    inited = true;
    return true;
}



void GameSprites::Draw(){
    if(sprite){
        sprite->Begin(D3DXSPRITE_ALPHABLEND);
        sprite->Draw(tex,NULL,NULL,&pos,color);
        sprite->End();
    }
}

GameSprites::~GameSprites(){
    if(sprite){
        sprite->Release();
        sprite = NULL;
    }
}

