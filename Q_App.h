#pragma once


#include "Q_Vector.h"
#include "Config.h"
#include "Q_Ball.h"
#include "Q_CoordinatSystem.h"
#include "Q_Console.h"

bool isActive (HWND screen);

class App
{
public:
    int Resolution;
    const char *path;
    HWND currScreen;

    HDC Menu;
    Rect startButton;
    Rect repeatButton;
    Rect infoButton;
    Rect exitButton;

    HDC HUD;
    HDC LeftButtonsImage;
    Rect LeftButtons;
    Rect cleanButton;
    Rect exitButtonHUD;
    Rect newPlanetButton;
    Rect screenShotButton;
    Rect minusSpeedButton;
    Rect plusSpeedButton;

    HDC CreateModeHDC; 
    Rect rMinusButton;
    Rect rPlusButton;
    Rect mMinusButton;
    Rect mPlusButton; 
    Rect mText;
    Rect rText;

    bool writeSaves = true;

    HDC ReplayHUD;


    coordinatSys miniMap;

    Console console;

    BallSystem ballS = {};
    Ball planetsInit[BallMax];
    Rect mainPlace;
    Rect sysInfo;
    Rect monitorS;

    Rect windowPos;

    App ();
    void getWindowPos ();
};

App::App ():
    miniMap (Vector{25,  685}, Vector{375,  285}, {(double) txGetExtentX (), (double) txGetExtentY ()}),
    console (Vector{25, 375}, Vector{25, 375}),
    planetsInit {},
    mainPlace ({.pos = {450, 0},   .size = {1350, 1000}}),
    sysInfo   ({.pos = {25,  25},  .size = {400,  375}}),
    monitorS  ({.pos = {0, 0},     .size = MonitorSize})
{

}

void App::getWindowPos ()
{
    RECT window = {0};

    if (!GetWindowRect (MainScreen, &window))
    {
        printf ("%d", GetLastError ());
    }
    windowPos.pos = {.x = (double)window.left, .y = (double)window.top};
    windowPos.size = {.x = ((double)window.right - windowPos.pos.x), .y = ((double)window.bottom - windowPos.pos.y)};
}

bool isActive (HWND screen)
{
    HWND currScreen = GetForegroundWindow ();

    if (screen != currScreen)
    {
        return false;
    }
    return true;
}
