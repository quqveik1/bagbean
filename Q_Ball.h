#pragma once

#include "TXLib.h"
#include "Q_Vector.h"
#include "Config.h"


bool onButtons (Vector pos);

struct Ball 
{
    Vector pos;
    Vector v;
    double m = 10e4;
    double r = 10;
    double charge = 2e1;
    COLORREF color = TX_WHITE;
    bool alive = 1;

    Vector history [BallHistoryLength];

    int oldestNum;

    void fillHistory ();


    void draw ();
    void DrawHistory ();
    void DrawHistoryLines ();
};


void Ball::draw ()
{
    $s
    txSetFillColour (color);
    txSetColour (color);
    txCircle (pos.x, pos.y, r);
}

struct BallSystem
{
    Ball ball[BallMax];

    int currlength = 0;

    void addBall (Ball newBall);
};

struct Rect
{
    Vector pos;
    Vector size;
    Vector finishPos;

    double left  () const { return this->pos.x; }
    double top   () const { return this->pos.y; }
    double right () const { return this->size.x + this->pos.x; }
    double bottom() const { return this->size.y + this->pos.y; }
};

void Ball::fillHistory ()
{
    assert (0 <= oldestNum && oldestNum <= BallHistoryLength);

    if (oldestNum >= (BallHistoryLength))
    {
        oldestNum = 0;
    }

    history [oldestNum] = pos;
    oldestNum++;

    
}

void Ball::DrawHistory ()
{
    const double kRgb = 225 / BallHistoryLength;
    double nColor = 0;
    COLORREF currColor = txGetColor ();
    COLORREF currFillColor = txGetFillColor ();

    for (int i = oldestNum; i < BallHistoryLength; i++)
    {
        assert (0 <= i && i < BallHistoryLength);

        txSetFillColor (RGB (nColor, nColor, nColor));
        txSetColor     (RGB (nColor, nColor, nColor));
        if (!onButtons (history[i])) txCircle (history[i].x, history[i].y, 2);
        nColor += kRgb;
    }

    for (int i = 0; i < oldestNum; i++)
    {
        assert (0 <= i && i < BallHistoryLength);

        txSetFillColor (RGB (nColor, nColor, nColor));
        txSetColor     (RGB (nColor, nColor, nColor));
        if (!onButtons (history[i])) txCircle (history[i].x, history[i].y, 2);
        nColor += kRgb;
    }

    txSetFillColor (currFillColor);
    txSetColor     (currColor);
    //txCircle (history[oldestNum].x, history[oldestNum].y, 3);
}

void Ball::DrawHistoryLines ()
{
    const double kRgb = 225 / BallHistoryLength;
    double nColor = 0;
    //$(oldestNum);

    for (int i = oldestNum; i < BallHistoryLength - 1; i++)
    {
        //cout << i;
        txSetFillColor (RGB (nColor, nColor, nColor));
        txSetColor     (RGB (nColor, nColor, nColor));
      // txCircle (history[i].x, history[i].y, 2);
        assert (0 <= i     && i     < BallHistoryLength);
        assert (0 <= i + 1 && i + 1 < BallHistoryLength);

        txLine   (history[i].x, history[i].y, history[i + 1].x, history[i + 1].y);
        nColor += kRgb;
    }

    if (oldestNum != 0)
    {
        assert (0 <= BallHistoryLength - 1 && BallHistoryLength - 1 < BallHistoryLength);


        txLine (history[BallHistoryLength - 1].x, history[BallHistoryLength - 1].y, history[0].x, history[0].y);
    }

    for (int i = 0; i < oldestNum - 1; i++)
    {
        assert (0 <= i && i < BallHistoryLength);
        //cout << i;
        txSetFillColor (RGB (nColor, nColor, nColor));
        txSetColor     (RGB (nColor, nColor, nColor));
        //txCircle (history[i].x, history[i].y, 2);
        txLine   (history[i].x, history[i].y, history[i + 1].x, history[i + 1].y);
        nColor += kRgb;
    }

    txSetFillColor (TX_PINK);
    txSetColor     (TX_PINK);

    assert (0 <= oldestNum && oldestNum < BallHistoryLength);
    txCircle (history[oldestNum].x, history[oldestNum].y, 3);
}

void BallSystem::addBall (Ball newBall)
{
    assert (0 <= currlength && currlength < BallMax - 1);

    ball[currlength].pos    = newBall.pos;
    ball[currlength].v      = newBall.v;
    ball[currlength].m      = newBall.m;
    ball[currlength].charge = newBall.charge;
    ball[currlength].r      = newBall.r;
    ball[currlength].color  = newBall.color;

    currlength++;
}

/*

bool onButtons (Vector pos)
{
      return    inRect (pos, TheGS->cleanButton) ||
                inRect (pos, TheGS->exitButtonHUD) || 
                inRect (pos, TheGS->newPlanetButton) ||
                inRect (pos, TheGS->screenShotButton) ||
                inRect (pos, TheGS->plusSpeedButton) ||
                inRect (pos, TheGS->minusSpeedButton);
}
*/