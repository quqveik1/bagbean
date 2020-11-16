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
    HDC HUD;

    coordinatSys miniMap;

    Console console;

    BallSystem ballS;
    Ball planetsInit[BallMax];
    const Rect mainPlace;
    const Rect sysInfo;

    App ();



};

App::App ():
    HUD (txLoadImage ("GravitySystemFolder/DevMaterials/Hud3.bmp")),
    miniMap (Vector{25,  685}, Vector{375,  285}, {(double) txGetExtentX (), (double) txGetExtentY ()}),
    console (Vector{25, 375}, Vector{25, 375}),
    ballS ({}),
    planetsInit {},
    mainPlace ({.pos = {450, 0},   .size = {1350, 1000}}),
    sysInfo ({.pos = {25,  25},  .size = {400,  375}})
{

}
    

