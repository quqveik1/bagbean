
#define TX_COMPILED
#include "C:/Users/Алехандро/Desktop/Alex_projects/TXTEST/TXLib.h"

struct Rect
{
    double left;
    double top;
    double right;
    double bottom;
};

void Movement (double x, double y, double r, double vX, double vY, double dt, double g = 0.7);
void Physics (double *x, double *y, double *vX, double *vY, double dt, double g, Rect box);
double SpeedX (double vX, double standartX);
double SpeedY (double vY, double standartY);
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
    Movement (100, 400, 20, 5, 5, 1);
    

    return 0;
}

// x = 100; y = 400; r = 20; vX = 5; vY = 5; dt = 1; g = 0.7
void Movement (double x, double y, double r, double vX, double vY, double dt, double g)
{
     double standartX = vX;
     double standartY = vY;
     bool flagClearBackground = false;

     txCreateWindow (800, 600);
     txSetColor     (TX_LIGHTRED);
     txSetFillColor (TX_RED);

     for (;;)
     {
        flagClearBackground = ClearBackground(flagClearBackground);

        Physics (&x, &y, &vX, &vY, dt, g, {r, r, txGetExtent().x - r, txGetExtent().y - r});

        vX = SpeedX (vX, standartX);
        vY = SpeedY (vY, standartY);

        SwitchColour ();
        r = SwitchRadius (r);

       if (txGetAsyncKeyState ('Q')) break;
       txCircle (x, y, r);
       txSleep (33);
     }
}


//x^2 + x = 0
//x^2 - 1 = 0 
//(x - 1)(x + 1) = 0

// 0 - x; 1 - y; 2 - vX; 3 - vY;
void Physics (double *x, double *y, double *vX, double *vY, double dt, double g, Rect box)
{

        *vY +=  g * dt;

        *x += *vX * dt;
        *y += *vY * dt;


        if (*x >= box.right)
        {
            *vX = -(*vX);
            *x = box.right - (*x - box.right);
        }

        if (*y >= box.bottom)
        {
            *vY = -*vY;
            *y = box.bottom - (*y - box.bottom);
        }

        if (*x <= box.left)
        {
            *vX = -*vX;
            *x = box.left - (*x - box.left);
        }

        if (*y <= box.top)
        {
            *vY = -*vY;
            *y = box.top - (*y - box.top);
        }
        
        
}

double SpeedX (double vX, double standartX)
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

        if (txGetAsyncKeyState ('S'))
        {
            vX = standartX;
        }

        return vX;
}

double SpeedY (double vY, double standartY)
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

        if (txGetAsyncKeyState ('S'))
        {
            vY = standartY;
        }

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


