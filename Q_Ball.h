#pragma once

#include "TXLib.h"
#include "Q_Vector.h"
#include "Config.h"


struct Ball 
{
    Vector pos;
    Vector v;
    double m;
    double r;
    double charge;
    COLORREF color;
    bool alive = 1;

    Vector history [BallHistoryLength];

    int oldestNum;
    void fillHistory ();

    void DrawHistory ();
    void DrawHistoryLines ();
};

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

    double left  () const { return this->pos.x; }
    double top   () const { return this->pos.y; }
    double right () const { return this->size.x + this->pos.x; }
    double bottom() const { return this->size.y + this->pos.y; }
};

void Ball::fillHistory ()
{
    assert (0 <= oldestNum && oldestNum < BallHistoryLength);

    history [oldestNum] = pos;
    oldestNum++;

    if (oldestNum >= (sizeof (history) / sizeof (history[0])))
    {
        oldestNum = 0;
    }
}

void Ball::DrawHistory ()
{
    const double kRgb = 225 / BallHistoryLength;
    double nColor = 0;

    for (int i = oldestNum; i < BallHistoryLength; i++)
    {
        assert (0 <= i && i < BallHistoryLength);

        txSetFillColor (RGB (nColor, nColor, nColor));
        txSetColor     (RGB (nColor, nColor, nColor));
        txCircle (history[i].x, history[i].y, 2);
        nColor += kRgb;
    }

    for (int i = 0; i < oldestNum; i++)
    {
        assert (0 <= i && i < BallHistoryLength);

        txSetFillColor (RGB (nColor, nColor, nColor));
        txSetColor     (RGB (nColor, nColor, nColor));
        txCircle (history[i].x, history[i].y, 2);
        nColor += kRgb;
    }

    txSetFillColor (TX_PINK);
    txSetColor     (TX_PINK);
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
    assert (0 <= currlength && currlength < BallMax);

    ball[currlength].pos    = newBall.pos;
    ball[currlength].v      = newBall.v;
    ball[currlength].m      = newBall.m;
    ball[currlength].charge = newBall.charge;
    ball[currlength].r      = newBall.r;
    ball[currlength].color  = newBall.color;

    currlength++;
}