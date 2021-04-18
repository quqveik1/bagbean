#pragma once


#include "Q_Vector.h"
#include "Config.h"
#include "Q_Ball.h"
#include "Q_CoordinatSystem.h"
#include "Q_Console.h"

/// <summary>
/// 
/// </summary>
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
    Rect cleanButton;
    Rect exitButtonHUD;
    Rect newPlanetButton;
    Rect screenShotButton;

    HDC ReplayHUD;

    coordinatSys miniMap;

    Console console;

    BallSystem ballS;
    Ball planetsInit[BallMax];
    Rect mainPlace;
    Rect sysInfo;
    Rect monitorS;

    App ();



};

App::App ():
    path ("GravitySystemFolder/DevMaterials/Hud3.bmp"),
    HUD (txLoadImage (path)),
    miniMap (Vector{25,  685}, Vector{375,  285}, {(double) txGetExtentX (), (double) txGetExtentY ()}),
    console (Vector{25, 375}, Vector{25, 375}),
    ballS ({}),
    planetsInit {},
    mainPlace ({.pos = {450, 0},   .size = {1350, 1000}}),
    sysInfo   ({.pos = {25,  25},  .size = {400,  375}}),
    monitorS  ({.pos = {0, 0},     .size = MonitorSize})
{

}
    

