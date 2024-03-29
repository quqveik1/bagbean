#pragma once

#include "TXLib.h"
#include "Q_Vector.h"
#include "Config.h"

class coordinatSys
{
    public: Vector coorSize_;
	public: Vector scalePix_;
	public: Vector startPosPix_;
    public: Rect   sysBorderPix_ = {startPosPix_, scalePix_};

    public: Vector intepretK_ = {}; //= scalePix / coorSize;

    public: coordinatSys ();
    public: coordinatSys (Vector startPosPix, Vector scalePix, Vector coorSize);
    public: void findRect ();



	public: Vector drawCircle (Ball ball); 
    public: Vector drawCircle (Vector vector, double r = 10);
    public: void drawLine (Vector startLPos, Vector finishLPos, COLORREF color = CometColor);
    public: Vector interpret (Vector vector);
};

void coordinatSys::findRect ()
{
    sysBorderPix_ =  {.pos = startPosPix_, .size = scalePix_};
}

coordinatSys::coordinatSys (Vector startPosPix, Vector scalePix, Vector coorSize) :
    startPosPix_ (startPosPix), 
    scalePix_ (scalePix),
    coorSize_ (coorSize)
{
    intepretK_.x = scalePix_.x / coorSize_.x;
    intepretK_.y = scalePix_.y / coorSize_.y;    
}

coordinatSys::coordinatSys () : 
    startPosPix_ ({0, 0}), 
    scalePix_ ({(double) txGetExtentX (), (double) txGetExtentY ()}),
    coorSize_ ({(double) txGetExtentX (), (double) txGetExtentY ()})
{
    intepretK_.x = 1;
    intepretK_.y = 1;
}


Vector coordinatSys::drawCircle (Ball ball)
{ 
	Vector pixPos = interpret (ball.pos);

    double rScale = (intepretK_.x + intepretK_.y) / 2;

    $s
    txSetFillColor (ball.color);
    txSetColor (ball.color);

    if (pixPos.x < (startPosPix_.x + scalePix_.x) && pixPos.x > startPosPix_.x)
    {
        if (pixPos.y < (startPosPix_.y + scalePix_.y) && pixPos.y > startPosPix_.y)
        {
            txCircle (pixPos.x, pixPos.y, ball.r * rScale);
        }
    }

    return pixPos;
}

void coordinatSys::drawLine (Vector startLPos, Vector finishLPos, COLORREF color)
{
    Vector startLPixPos = interpret (startLPos);

    Vector finishLPixPos = interpret (finishLPos);

    $s

    txSetFillColor (color);

    txLine (startLPos.x, startLPos.y, finishLPos.x, finishLPos.y);
}

Vector coordinatSys::drawCircle (Vector vector, double r)
{
    Vector pixPos = interpret(vector);

    double rScale = (intepretK_.x + intepretK_.y) / 2;

    txCircle (pixPos.x, pixPos.y, r * rScale);

    return pixPos;

}

Vector coordinatSys::interpret (Vector vector)
{
    return startPosPix_ + (intepretK_ * vector);
}
