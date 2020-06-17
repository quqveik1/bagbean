
#define TX_COMPILED
#include "TXLib.h"
#include <math.h>
//You must to delete "my" TXLib.h file from the ptoject or you can do not download "my" TXLib.h file, you need to download TXLib from https://sourceforge.net/projects/txlib/files/latest/download and install this
	

/*
-   -
a + b = a.x + b.x; a.y + b.y
->
a  * k = a.x * k; a.y * k
->  ->
a   b

a.y * b.y + a.x * b.x = 0
->
|a| = sqrt (a.x^2 + a.y^2)(
->
AB = {};
A = {1, 2}
B = {3, 4}
|AB| = {b.x - a.x, b.y - a.y};

*/


struct Vector 
{
    double x;
    double y;
};
struct Rect
{
    Vector pos;
    Vector size;

    double left  () { return this->pos.x; }
    double top   () { return this->pos.y; }
    double right () { return this->size.x + this->pos.x; }
    double bottom() { return this->size.y + this->pos.y; }
};
struct Ball 
{
    Vector pos;
    Vector v;
    Vector a;
    double r;
};

inline Vector  operator +  (const Vector &a, const Vector &b);
inline Vector &operator += (Vector &a, const Vector &b);
inline Vector  operator -  (const Vector &a, const Vector &b);
inline Vector  operator *  (const Vector &a, const double b);
inline Vector  operator *  (const Vector &a, const Vector &b);
inline Vector &operator *= (Vector &a, const Vector &b);

void Draw (const Vector &vector, const Vector &startPoint, COLORREF colorOfMainVector, double thickness = 1);
void drawArrows (const Vector &mainVector, const  Vector &startArrowPoint);
Vector makePerpendikularLine (const Vector &mainVector);
void drawVectorCircle (const Vector &vector, const Vector &startP, COLORREF color, double thickness = 1);
void rotateVector (Vector &vector, double angle);
void Draw_verVector (Vector &vector, const Vector &startPoint, COLORREF colorOfMainVector, const double thickness, double angle);

void BallFrame(Ball *ball, double dt); 
void Physics  (Ball *ball, Rect box, double dt);
double SpeedX (double vX);
double SpeedY (double vY);
bool ClearBackground (bool flagClearBackground);
void   SwitchColour ();
double SwitchRadius (double r);

/*
0000 1101 (2) = +13 (10)
1111 0010 (2) = ~13
---------
1111 1111

1111 0010 (2) = ~13
1111 0011 (2) = -13

  0000 1101 (2) = +13 (10)
  1111 0011 (2) = -13 (10)
  ---------
1 0000 0000 =>
1|0000 0000 =>
  0000 0000

*/ 


//New
int main()
{
    txCreateWindow (800, 600);
    txSetColor     (TX_LIGHTRED);
    txSetFillColor (TX_RED);
    
    /*
    Ball ball1 = {{100, 400}, {5, 5}, {0, 0.7}, 20};
    Ball ball2 = {{300, 100}, {5, 5}, {0, 0.7}, 10};
    Ball ball3 = {{400, 500}, {5, 5}, {0, 0.7}, 15};

    double dt = 1;

    bool flagClearBackground = false;

    for (;;)
    {
        BallFrame (&ball1, dt);
        BallFrame (&ball2, dt);
        BallFrame (&ball3, dt);
        
        txSleep (20);

        if (txGetAsyncKeyState('Q')) break;

        flagClearBackground = ClearBackground (flagClearBackground);
    }
    */

    Vector start =  {.x = 400, .y = 300};
    Vector vector = {.x = 0,   .y = -50};

    Draw_verVector (vector, start, TX_RED, 5, 0);

    return 0;
}

// x = 100; y = 400; r = 20; vX = 5; vY = 5; dt = 1; g = 0.7
void BallFrame (Ball *ball, double dt)
{
    /*
     double standartX = ball.vX;
     double standartY = vY;
     */
     const Rect box = { {ball->r, ball->r}, {txGetExtent().x - 2 * (ball->r), txGetExtent().y - 2 * (ball->r)} };
     Physics (ball, box, dt);

     ball->v.x = SpeedX (ball->v.x);
     ball->v.y = SpeedY (ball->v.y);

     SwitchColour ();
     (*ball).r = SwitchRadius ((*ball).r);

     txCircle ((*ball).pos.x, (*ball).pos.y, (*ball).r);
}


//x^2 + x = 0
//x^2 - 1 = 0 
//(x - 1)(x + 1) = 0

// 0 - x; 1 - y; 2 - vX; 3 - vY;
void Physics (Ball *ball, Rect box, double dt)
{
        ball->v += (ball->a * dt);
        //(*ball).v.y +=  ball->a.y * dt;
        //ball->  v.x +=  ball->a.x * dt;

        ball->pos += (ball->v * dt);
        //(*ball).pos.x += (*ball).v.x * dt;
        //(*ball).pos.y += (*ball).v.y * dt;


        if ((*ball).pos.x >= box.right ())
        {
            (*ball).v.x = -((*ball).v.x);
            (*ball).pos.x = box.right() - ((*ball).pos.x - box.right());
        }

        if ((*ball).pos.y >= box.bottom())
        {
            (*ball).v.y = -((*ball).v.y);
            (*ball).pos.y = box.bottom() - ((*ball).pos.y - box.bottom());
        }

        if ((*ball).pos.x <= box.left())
        {
            (*ball).v.x = -((*ball).v.x);
            (*ball).pos.x = box.left() - ((*ball).pos.x - box.left());
        }

        if ((*ball).pos.y <= box.top())
        {
            (*ball).v.y = -((*ball).v.y);
            (*ball).pos.y = box.top() - ((*ball).pos.y - box.top());
        }
        
}//

double SpeedX (double vX)
{
        if (txGetAsyncKeyState (VK_RIGHT))
        {
            if (vX >= 0) vX++;
            
            if (vX < 0)  vX--;
          
        }

        if (txGetAsyncKeyState (VK_LEFT))
        {
            if (vX >= 0) vX--;

            if (vX < 0)  vX++;
        }
        /*
        if (txGetAsyncKeyState ('S'))
        {
            vX = standartX;
        }
        */
        return vX;
}

double SpeedY (double vY)
{
        if (txGetAsyncKeyState (VK_UP))
        {
            if (vY >= 0) vY++;
            
            if (vY < 0)  vY--;
          
        }

        if (txGetAsyncKeyState (VK_DOWN))
        {
            if (vY >= 0) vY--;

            if (vY < 0)  vY++;
        }
        /*
        if (txGetAsyncKeyState ('S'))
        {
            vY = standartY;
        }
        */
        return vY;
}

void SwitchColour ()
{
    if (txGetAsyncKeyState ('B')) txSetFillColor (TX_BLUE);
    if (txGetAsyncKeyState ('R')) txSetFillColor (TX_RED);
    if (txGetAsyncKeyState ('G')) txSetFillColor (TX_GREEN);
    if (txGetAsyncKeyState ('Y')) txSetFillColor (TX_YELLOW);
    if (txGetAsyncKeyState ('P')) txSetFillColor (TX_PINK);
}

double SwitchRadius (double r)
{
    if (txGetAsyncKeyState('E')) r++;
    if (r > 0)
    {
        if (txGetAsyncKeyState('W')) r--;
    }

    return r;
}

bool ClearBackground (bool flagClearBackground)
{
     if (txGetAsyncKeyState ('C'))
     {
         flagClearBackground = true;
     }

     if (txGetAsyncKeyState ('N'))
     {
         flagClearBackground = false;
     }

     COLORREF color = txGetFillColor();
     txSetFillColor (TX_BLACK);
     if (flagClearBackground == true) txClear();
     txSetFillColor (color);

     return flagClearBackground;
}




void Draw (const Vector &vector, const Vector &startPoint, COLORREF colorOfMainVector, double thickness)
{
    Vector finishPoint = {};
    finishPoint = startPoint + vector;

    txSetColor (colorOfMainVector, thickness);
    txLine (startPoint.x, startPoint.y, finishPoint.x, finishPoint.y);
    drawArrows (vector, finishPoint);

    /*
    txCircle (startX,  startY, 7);
    COLORREF color = txGetFillColor();
    txSetFillColor (TX_BLUE);
    txCircle (finishX, finishY, 5);
    txSetFillColor (color);
    */
    //txLine(vector.x, vector.y, vector.x+);
}

void Draw_verVector (Vector &vector, const Vector &startPoint, COLORREF colorOfMainVector, const double thickness, double angle)
{
    for (int i = 0; i < 360; i++)
    {
        //.i = i % 100;
        angle++;
        rotateVector (vector, angle);

        Vector finishPoint = {};
        finishPoint = startPoint + vector; 
        txSetColor (colorOfMainVector, thickness);
        txLine (startPoint.x, startPoint.y, finishPoint.x, finishPoint.y);
        drawArrows (vector, finishPoint);

        txSleep (100);
        txSetFillColor (TX_BLACK);
        txClear ();
    }
}



void rotateVector (Vector &vector, double angle)
{
    Vector newVector = {};
    double cosinus = cos (angle * 3.14 / 180);
    double sinus   = sin (angle * 3.14 / 180);
    newVector.x = vector.x * cosinus - vector.y * sinus;
    newVector.y = vector.y * cosinus + vector.x * sinus;

    vector = newVector;
}

void drawArrows (const Vector &mainVector, const Vector &startArrowPoint)
{
    Vector perpendicular1 = makePerpendikularLine(mainVector);
    Vector perpendicular2 = perpendicular1 * -1;

    Vector arrow1 = perpendicular1 * 0.05 - mainVector * 0.2;
    Vector arrow2 = perpendicular2 * 0.05 - mainVector * 0.2;

    Vector arrow1finishPoint = ((arrow1) + startArrowPoint);
    Vector arrow2finishPoint = ((arrow2) + startArrowPoint);

    txLine (startArrowPoint.x, startArrowPoint.y, arrow1finishPoint.x, arrow1finishPoint.y);
    txLine (startArrowPoint.x, startArrowPoint.y, arrow2finishPoint.x, arrow2finishPoint.y);
}

void drawVectorCircle (const Vector &vector, const Vector &startP, COLORREF color, double thickness)
{
    Vector finishP = vector + startP;

    txSetColor (color, thickness);
    txLine   (startP.x, startP.y, finishP.x, finishP.y);
    txCircle (finishP.x, finishP.y, 5);
}

Vector makePerpendikularLine(const Vector &mainVector)
{
    Vector perpendikularLine = {};
    perpendikularLine.x =   mainVector.y;
    perpendikularLine.y = -(mainVector.x);

    return perpendikularLine;
}

inline Vector operator * (const Vector &a, const double b)
{
    Vector result = {};
    result.x = a.x * b;
    result.y = a.y * b;

    return result;
}


inline Vector operator + (const Vector &a, const Vector &b)
{
    Vector result = {};
    result.x = a.x + b.x;
    result.y = a.y + b.y;

    return result;
}

inline Vector operator - (const Vector &a, const Vector &b)
{
    return { .x = a.x - b.x,
             .y = a.y - b.y };
}

inline Vector &operator += (Vector &a, const Vector &b)
{
   a.x += b.x;
   a.y += b.y;
   
   return a;
}

inline Vector operator * (const Vector &a, const Vector &b)
{
    Vector copyA  = a;
    return copyA *= b;
}

inline Vector &operator *= (Vector &a, const Vector &b)
{
    a.x *= b.x;
    a.y *= b.y;
    return a;
}

/*
int x = 0;
x += 3;
int y = (x +  3);
int y = (x += 3);
(x += 3) += 7


Vector v = {};
Vector p = v + v;
Vector p = v += v;

*/

