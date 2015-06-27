
#include "GameMain.h"
#include <iostream>
#include <windowsx.h>
#include "resource.h"
using namespace std;

#define fps 50

#define PAUSEBT 18800
#define CLEARBT 13400
#define LOADBT  13600
#define SAVEBT  12200
#define BORDBT  17000

LRESULT CALLBACK WindowProc (HWND, UINT, WPARAM, LPARAM);

wchar_t szClassName[ ] = L"CW";
HWND MainWnd;
HWND disp,b_pause,b_border;

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    MSG msg;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProc;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    wincl.hIconSm = LoadIcon(hThisInstance,MAKEINTRESOURCE(ICO32));
    wincl.hIcon = LoadIcon(hThisInstance,MAKEINTRESOURCE(ICO16));
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    if (!RegisterClassEx (&wincl))
        return 0;
    MainWnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           L"Conway Game Of Life",       /* Title Text */
           WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           900,                 /* The programs width */
           720,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );
    ShowWindow (MainWnd, nCmdShow);
    b_pause = CreateWindowEx(0,L"Button",L"Start",WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON,700,30,120,25,MainWnd,(HMENU)PAUSEBT,hThisInstance,NULL);
    CreateWindowEx(0,L"Button",L"Clear",WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON,700,60,120,25,MainWnd,(HMENU)CLEARBT,hThisInstance,NULL);
    b_border = CreateWindowEx(0,L"Button",L"Borderless",WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON,700,90,120,25,MainWnd,(HMENU)BORDBT,hThisInstance,NULL);
    CreateWindowEx(0,L"Button",L"Load",WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON,700,120,120,25,MainWnd,(HMENU)LOADBT,hThisInstance,NULL);
    CreateWindowEx(0,L"Button",L"Save",WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON,700,150,120,25,MainWnd,(HMENU)SAVEBT,hThisInstance,NULL);
    disp = CreateWindowEx(WS_EX_CLIENTEDGE,szClassName,NULL,WS_CHILD|WS_VISIBLE|WS_BORDER,20,20,655,655,MainWnd,0,hThisInstance,NULL);
    Init(disp);
    //DWORD sttm;
    SetTimer(MainWnd,998,fps,NULL);

        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        //if(msg.message == WM_QUIT)
        //    break;

    Flush();
    delete gDevice;
    //system("pause");
    return msg.wParam;
}


LRESULT CALLBACK WindowProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_TIMER:
            if(!Pause)   {
                Update();
            }
            Draw(gDevice);
            break;
        case WM_COMMAND:
            switch(LOWORD(wParam)){
            case PAUSEBT:
                Pause = !Pause;
                if(Pause){
                    SetWindowText(b_pause,L"Start");
                } else{
                    SetWindowText(b_pause,L"Pause");
                }
                break;
            case CLEARBT:
                Clear();
                break;
            case LOADBT:
                Pause = true;
                SetWindowText(b_pause,L"Start");
                if(!LoadSeed()){
                    MessageBox(MainWnd,L"Loading Seed File Failed!",L"Error!",MB_OK);
                }
                break;
            case SAVEBT:
                Pause = true;
                SetWindowText(b_pause,L"Start");
                if(!SaveSeed()){
                    MessageBox(MainWnd,L"Writing Seed File Failed!",L"Error!",MB_OK);
                }
                break;
            case BORDBT:
                Borderless = !Borderless;
                Border();
                if(Borderless){
                    SetWindowText(b_border,L"Border");
                }else{
                    SetWindowText(b_border,L"Borderless");
                }
                break;
            }
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_LBUTTONDOWN:
            if(hwnd==disp){
                Pause = true;
                SetWindowText(b_pause,L"Start");
                int x,y;
                x = GET_X_LPARAM(lParam);
                y = GET_Y_LPARAM(lParam);
                if((x%PIX)&&(y%PIX)){
                    x = x/PIX+1;
                    y = y/PIX+1;
                    Matrix[x][y]->alive = !(Matrix[x][y]->alive);
                }
                Draw(gDevice);
            }
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
