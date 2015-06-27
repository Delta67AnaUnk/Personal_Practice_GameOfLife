#ifndef D2GRAPHIC_H
#define D2GRAPHIC_H

#include <d3d9.h>
#include <d3dx9.h>

#define PIX   10
#define MSIZE 65

class D3D9GraphicDevice{
    public:
        D3D9GraphicDevice();
        ~D3D9GraphicDevice();

        void Init(HWND hWnd);
        void Clear(D3DCOLOR clr);
        void Grid();
        inline void Begin(){dev->BeginScene();dev->SetFVF(D3DFVF_XYZRHW|D3DFVF_DIFFUSE);}
        inline void End(){dev->EndScene();}
        inline void Present(){dev->Present(NULL,NULL,NULL,NULL);}

        LPDIRECT3DDEVICE9 dev;
    private:
        LPDIRECT3D9 direct3d;

};

typedef D3D9GraphicDevice* LPGraphicDevice;

struct gridpoint{
    float x;
    float y;
    float z;
    float rhw;
    DWORD color;
    inline void Fill(){z=1.0f;rhw=1.0f;color=D3DCOLOR_XRGB(245,150,170);}
};

extern gridpoint grpt[MSIZE*4-4];

extern LPGraphicDevice gDevice;

extern LPDIRECT3DTEXTURE9 tex;

class  GameSprites{
    public:
        GameSprites();
        GameSprites(int x,int y);
        virtual ~GameSprites();

        bool Init(LPDIRECT3DDEVICE9 dev);
        inline bool IsInit(){return inited;}

        void Draw();
    private:
        LPD3DXSPRITE sprite;
        D3DXVECTOR3 pos;
        D3DCOLOR color;
        bool inited;
};


#endif // D2GRAPHIC_H
