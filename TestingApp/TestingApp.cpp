#include "TXLib.h"
#include "Q_CoordinatSystem.h"

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

int main ()
{
    txCreateWindow (1000, 1000);
    txSetFillColor (TX_BLACK);

    coordinatSys cSystem;

    cSystem.startPosPix = {200, 200};
    cSystem.coorSize = {100, 200};
    cSystem.scalePix = {200, 200};

    Vector newPos = cSystem.drawCCircle ({-100, -100}, 5);

    cout << newPos.x << "    " << newPos.y; 


    return 0;
}
