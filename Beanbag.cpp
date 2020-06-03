
#define TX_COMPILED
#include "C:/Users/Алехандро/Desktop/Alex_projects/TXTEST/TXLib.h"

struct Rect
{
    double left;
    double top;
    double right;
    double bottom;
};

struct Ball 
{
    double x;
    double y;
    double r;
    double vX;
    double vY;
    double dt;
    double g;
};

void BallFrame(Ball *ball);
void Physics (Ball *ball, Rect box);
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

    Ball ball1 = {100, 400, 20, 5, 5, 1, 0.7};
    Ball ball2 = {300, 100, 10, 5, 5, 1, 0.7};
    Ball ball3 = {400, 500, 15, 5, 5, 1, 0.7};
    bool flagClearBackground = false;
    for (;;)
    {
        BallFrame (&ball1);
        BallFrame (&ball2);
        BallFrame (&ball3);
        
        txSleep (20);

        if (txGetAsyncKeyState('Q')) break;
        flagClearBackground = ClearBackground (flagClearBackground);
    }
    

    return 0;
}

// x = 100; y = 400; r = 20; vX = 5; vY = 5; dt = 1; g = 0.7
void BallFrame (Ball *ball)
{
    /*
     double standartX = ball.vX;
     double standartY = vY;
     */

     Physics (ball, {(*ball).r, (*ball).r, txGetExtent().x - (*ball).r, txGetExtent().y - (*ball).r});

     (*ball).vX = SpeedX ((*ball).vX);
     (*ball).vY = SpeedY ((*ball).vY);

     SwitchColour ();
     (*ball).r = SwitchRadius ((*ball).r);

     txCircle ((*ball).x, (*ball).y, (*ball).r);
}


//x^2 + x = 0
//x^2 - 1 = 0 
//(x - 1)(x + 1) = 0

// 0 - x; 1 - y; 2 - vX; 3 - vY;
void Physics (Ball *ball, Rect box)
{

        (*ball).vY +=  (*ball).g * (*ball).dt;

        (*ball).x += (*ball).vX * (*ball).dt;
        (*ball).y += (*ball).vY * (*ball).dt;


        if ((*ball).x >= box.right)
        {
            (*ball).vX = -((*ball).vX);
            (*ball).x = box.right - ((*ball).x - box.right);
        }

        if ((*ball).y >= box.bottom)
        {
            (*ball).vY = -((*ball).vY);
            (*ball).y = box.bottom - ((*ball).y - box.bottom);
        }

        if ((*ball).x <= box.left)
        {
            (*ball).vX = -((*ball).vX);
            (*ball).x = box.left - ((*ball).x - box.left);
        }

        if ((*ball).y <= box.top)
        {
            (*ball).vY = -((*ball).vY);
            (*ball).y = box.top - ((*ball).y - box.top);
        }
        
        
}

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


