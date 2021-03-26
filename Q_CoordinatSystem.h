#pragma once

//#include "Q_Vector.h"
#include "TXLib.h"
#include "Q_Vector.h"
#include "Config.h"
#include "Q_Ball.h"


class coordinatSys
{
    public: Vector coorSize_;
	public: Vector scalePix_;
	public: Vector startPosPix_;
    private: Rect   sysBorderPix_ = {startPosPix_, scalePix_};

    public: Vector intepretK_ = {}; //= scalePix / coorSize;

    public: coordinatSys ();
    public: coordinatSys (Vector startPosPix, Vector scalePix, Vector coorSize);



	public: Vector drawCircle (Ball ball); 
    public: Vector drawCircle (Vector vector, double r = 10);
    public: void drawLine (Vector startLPos, Vector finishLPos, COLORREF color = CometColor);
    public: Vector interpret (Vector vector);
};


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
    //Vector intepretK = {}; //= scalePix / coorSize;

    //intepretK.x = scalePix.x / coorSize.x;
    //intepretK.y = scalePix.y / coorSize.y;

	Vector pixPos = interpret (ball.pos);

    double rScale = (intepretK_.x + intepretK_.y) / 2;

    //$s
    txSetFillColor (ball.color);

    //if (pixPos.x > sysBorderPix_.left () && pixPos.y > sysBorderPix_.top () && pixPos.x < sysBorderPix_.right () && pixPos.y < sysBorderPix_.bottom ())
    if (pixPos.x < (startPosPix_.x + scalePix_.x) && pixPos.x > startPosPix_.x)
    {
        if (pixPos.y < (startPosPix_.y + scalePix_.y) && pixPos.y > startPosPix_.y)
        {
            txCircle (pixPos.x, pixPos.y, ball.r * rScale);
        }
    }
    //_getch ();

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
    //Vector intepretK = {}; //= scalePix / coorSize;

    //intepretK.x = scalePix.x / coorSize.x;
    //intepretK.y = scalePix.y / coorSize.y;

    Vector pixPos = interpret(vector);

    double rScale = (intepretK_.x + intepretK_.y) / 2;

    //if (pixPos.x > sysBorderPix_.left() && pixPos.y > sysBorderPix_.top() && pixPos.x < sysBorderPix_.right() && pixPos.y < sysBorderPix_.bottom())
    txCircle(pixPos.x, pixPos.y, r * rScale);

    return pixPos;

}



Vector coordinatSys::interpret (Vector vector)
{
    return startPosPix_ + (intepretK_ * vector);
}