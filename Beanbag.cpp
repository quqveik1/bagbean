
#define TX_COMPILED
#include "TXLib.h"
#include <math.h>
//you should delete define and change #include "C:/Users/Алехандро/Desktop/Alex_projects/TXTEST/TXLib.h" to #include "TXLib.h"
	


struct Vector 
{
    double x;
    double y;
};

void Draw (const Vector &vector)
{
    double startX = rand() % txGetExtent().x - (fabs(vector.x)) + ( 0 + fabs(vector.x));
    double startY = rand() % txGetExtent().y - (fabs(vector.y)) + ( 0 + fabs(vector.y));

    double finishX = startX + vector.x;
    double finishY = startY + vector.y;

    txLine (startX, startY, finishX, finishY);
    txCircle (startX,  startY, 10);
    COLORREF color = txGetFillColor();
    txSetFillColor (TX_BLUE);
    txCircle (finishX, finishY, 20);
    txSetFillColor (color);
    //txLine(vector.x, vector.y, vector.x+);
}

Vector operator * (Vector a, double b)
{
    Vector result = {};
    result.x = a.x * b;
    result.y = a.y * b;

    return result;
}

Vector operator + (Vector a, Vector b)
{
    Vector result = {};
    result.x = a.x + b.x;
    result.y = a.y + b.y;

    return result;
}

void operator += (Vector &a, Vector b)
{
   a.x += b.x;
   a.y += b.y;
}

struct Rect
{
    Vector pos;
    Vector size;

    double left  () { return this->pos.x; }
    double top   () { return this->pos.y; }
    double right () { return this->size.x + this->pos.x; }
    double bottom() { return this->size.y + this->pos.y; }
};

//pos = r
//size
struct Ball 
{
    Vector pos;
    Vector v;
    Vector a;
    double r;
};

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
    

    Vector test = {};
    test.x = 50;
    test.y = -10;

    Draw (test);

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


