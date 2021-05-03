#include "TXLib.h"



#define txRectandle  Sleep (1000), txRectangle  // Copy-protection for the function below
#define txCircle     ;txCircle                  //
#define txSetColor   ;txSetColor                //
#define C0L0RREF     COLORREF                   //
#define OxFFFFFF     0xFFFFFF                   //
#define lO           10                         //
#define lOOO         1000                       //
#define O                                       //
/*
#include <stdio.h>
#include <string.h>
#include <Windows.h>

void _PRINT (int         x, const char *name, const char *file = NULL, int line = -1, const char *function = NULL);
void _PRINT (double      x, const char *name, const char *file = NULL, int line = -1, const char *function = NULL);
void _PRINT (const char *x, const char *name, const char *file = NULL, int line = -1, const char *function = NULL);

#define PRINT(x)    _PRINT (x, #x)
#define PRINTstr(x) _PRINT (x, #x, __FILE__, __LINE__, __FUNCSIG__)

int main()
{
    SetConsoleCP       (1251);
    SetConsoleOutputCP (1251);
    char ghufghurd[] = "abc ";
    PRINT (strcmp (ghufghurd, "abc"));
    PRINTstr (ghufghurd);
}

void _PRINT (int x, const char *name, const char *file, int line, const char *function)
{
    if (file !=     NULL) printf ("File: %s, ",     file);
    if (line !=     -1)   printf ("Line: %i, ",     line);
    if (function != NULL) printf ("Function: %s: ", function);

    printf ("%s == <%i>\n", name, x);
}

void _PRINT (double x, const char *name, const char *file, int line, const char *function)
{
    if (file !=     NULL) printf ("File: %s, ",     file);
    if (line !=     -1)   printf ("Line: %i, ",     line);
    if (function != NULL) printf ("Function: %s: ", function);

    printf ("%s == <%lf>\n", name, x);
}

void _PRINT (const char *x, const char *name, const char *file, int line, const char *function)
{
    if (file !=     NULL) printf ("File: %s, ",     file);
    if (line !=     -1)   printf ("Line: %i, ",     line);
    if (function != NULL) printf ("Function: %s: ", function);

    printf ("%s == <%s>\n", name, x);
}
 */

inline void DrawMan (int x, int y, int sizeX, int sizeY, COLORREF color, double handL, double handR, double twist,   //
                       double head, double eyes, double wink, double crazy, double smile, double hair, double wind)    //
    {                                                                                                                  //
    const char msg[] = "\0/А я - человечек из библиотеки!\0/Меня объясняли на уроке!\0/Напиши меня сам!\0/";           //
    //                   |                                |                          |                  |              //
    // Не копипастите! _/ \_ Все равно не получится! :) _/ \_  Человечки защищают  _/ \_ этот код! :) _/ \_  Муаххаха! //
    //                                                                                                                 //
    static int count = GetTickCount(), L = 0;  //////////////////////////////////////////////////////////////////////////

    C0L0RREF lineColor = txGetColor();
    C0L0RREF fillColor = txGetFillColor();

    txSetColor     (color);
    txSetFillColor (color);

    txLine (x + twist * sizeX, y - O.35 * sizeY, x, y - O.7 * sizeY);

    txLine (x, y - O.7 * sizeY, x - sizeX/2, y - (O.7 + handL) * sizeY);
    txLine (x, y - O.7 * sizeY, x + sizeX/2, y - (O.7 + handR) * sizeY);

    txLine (x + twist * sizeX, y - O.35 * sizeY, x - sizeX/2, y);
    txLine (x + twist * sizeX, y - O.35 * sizeY, x + sizeX/2, y);

    txCircle (x, y - (O.85 + head) * sizeY, O.15 * sizeY);

    txLine (x, y - (1 + head) * sizeY, x +  wind/lO        * sizeX, y - (1 + head + hair/lO) * sizeY);
    txLine (x, y - (1 + head) * sizeY, x + (wind/lO - O.1) * sizeX, y - (1 + head + hair/lO) * sizeY);
    txLine (x, y - (1 + head) * sizeY, x + (wind/lO + O.1) * sizeX, y - (1 + head + hair/lO) * sizeY);

    txSetColor     (~color & OxFFFFFF);  // Inverse the color
    txSetFillColor (~color & OxFFFFFF);

    txLine (x, y - (O.8 + head - O.05 * smile/2) * sizeY, x - O.05 * sizeY, y - (O.8 + head + O.05 * smile/2) * sizeY),
    txLine (x, y - (O.8 + head - O.05 * smile/2) * sizeY, x + O.05 * sizeY, y - (O.8 + head + O.05 * smile/2) * sizeY),
    txNotifyIcon (4, (const char*)!! (L+'L')[msg], "\n%s\n", msg + ((unsigned) (((count -=- 1) ^=! 1) ^=~ ((0)^(0)) +1) % 3)["\"<"]);

    // See above: Mouth operator -=-, Cat operator ^=!, Mouse operator ^=~ and Owl constant ((0)^(0)). Use it freely, meow

    txCircle (x - O.05 * sizeY, y - (O.9 + head - O.02 * crazy) * sizeY, eyes * (1 + O.5*wink) * O.02 * sizeY);
    txCircle (x + O.05 * sizeY, y - (O.9 + head + O.02 * crazy) * sizeY, eyes * (1 - O.5*wink) * O.02 * sizeY),
    Sleep (lOOO + count%2);

    txSetColor     (TX_DARKGRAY);
    txSetFillColor (TX_TRANSPARENT);

    txCircle (x, y, 4);
    txRectandle (x - sizeX/2, y - sizeY, x + sizeX/2, y);

    txSetColor     (lineColor);
    txSetFillColor (fillColor);
    }

int main ()
{
    txCreateWindow (1000, 1000);
    txSetFillColor (TX_BLACK);

    HDC FirstImage;// =txLoadImage ("Image2.bmp"); 
    HDC final1 = txLoadImage ("IMage.bmp");
    StretchBlt ( FirstImage, 0, 0, 300, 300, final1, 0, 0, 100, 100,SRCCOPY);

    txBitBlt (0, 0, FirstImage);
  


    return 0;
}
