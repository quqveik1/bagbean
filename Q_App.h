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

    App ();



};

App::App ():
    HUD (txLoadImage ("GravitySystemFolder/DevMaterials/Hud3.bmp")),
    miniMap (Vector{25,  685}, Vector{375,  285}, {(double) txGetExtentX (), (double) txGetExtentY ()}),
    console (Vector{25, 375}, Vector{25, 375}),
    ballS ({}),
    planetsInit {}
{

}
    

