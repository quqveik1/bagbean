#include "TXLib.h"

void _PRINT (int x);
void _PRINT (double x);
void _PRINT (const char *x);

#define PRINT(x) printf ("%s == ", #x); _PRINT (x)

int main()
{
    int ghufghurd = 5;
    PRINT (ghufghurd);
}

void _PRINT (int x)
{
    printf ("%i", x);
}

void _PRINT (double x)
{
    printf ("%lf", x);
}

void _PRINT (const char *x)
{
    printf ("%s", x);
}

