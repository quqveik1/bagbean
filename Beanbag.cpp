
#define TX_COMPILED
#include "C:/Users/Алехандро/Desktop/Alex_projects/TXTEST/TXLib.h"


struct Vector 
{
    double x;
    double y;
};

struct Rect
{
    Vector pos;
    Vector size;

};

double left  (const Rect *rect) { return rect->pos.x; }
double top   (const Rect *rect) { return rect->pos.y; }
double right (const Rect *rect) { return rect->size.x + 0; }
double bottom(const Rect *rect) { return rect->size.y + 0; }

struct Ball 
{
    Vector pos;
    Vector v;
    double r;
};

void BallFrame(Ball *ball, double dt, double g); 
void Physics (Ball *ball, Rect box, double dt, double g);
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

    Ball ball1 = {{100, 400}, {5, 5}, 20};
    Ball ball2 = {{300, 100}, {5, 5}, 10};
    Ball ball3 = {{400, 500}, {5, 5}, 15};

    bool flagClearBackground = false;

    double dt = 1;
    double g = 0.7;

    for (;;)
    {
        BallFrame (&ball1, dt, g);
        BallFrame (&ball2, dt, g);
        BallFrame (&ball3, dt, g);
        
        txSleep (20);

        if (txGetAsyncKeyState('Q')) break;

        flagClearBackground = ClearBackground (flagClearBackground);
    }
    

    return 0;
}

// x = 100; y = 400; r = 20; vX = 5; vY = 5; dt = 1; g = 0.7
void BallFrame (Ball *ball, double dt, double g)
{
    /*
     double standartX = ball.vX;
     double standartY = vY;
     */
     const Rect box = {{ball->r, ball->r}, {txGetExtent().x - (ball->r), txGetExtent().y - (ball->r)}};
     Physics (ball, box, dt, g);

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
void Physics (Ball *ball, Rect box, double dt, double g)
{

        (*ball).v.y +=  g * dt;

        (*ball).pos.x += (*ball).v.x * dt;
        (*ball).pos.y += (*ball).v.y * dt;


        if ((*ball).pos.x >= right (&box))
        {
            (*ball).v.x = -((*ball).v.x);
            (*ball).pos.x = right(&box) - ((*ball).pos.x - right(&box));
        }

        if ((*ball).pos.y >= bottom(&box))
        {
            (*ball).v.y = -((*ball).v.y);
            (*ball).pos.y = bottom(&box) - ((*ball).pos.y - bottom(&box));
        }

        if ((*ball).pos.x <= left(&box))
        {
            (*ball).v.x = -((*ball).v.x);
            (*ball).pos.x = left(&box) - ((*ball).pos.x - left(&box));
        }

        if ((*ball).pos.y <= top(&box))
        {
            (*ball).v.y = -((*ball).v.y);
            (*ball).pos.y = top(&box) - ((*ball).pos.y - top(&box));
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


