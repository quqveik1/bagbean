#define _CRT_SECURE_NO_WARNINGS
#include "TXLib.h"
#include "Q_Vector.h"
#include "Q_CoordinatSystem.h"




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




struct Button
{
    Rect buttonCoor;
    HDC buttonHDC;
};



#define key(a) txGetAsyncKeyState (a)
void openImage (HDC &hdc);
void RunCreatingButton (Button &button);
bool onButtonClicked (Rect Button);
void RunSettingsButton (FILE *file);




int main ()
{
    txCreateWindow (1920, 1080);
    txSetFillColor (TX_BLACK);
    Button button[6];
    FILE *file = fopen ("Buttons.txt", "w");

    for (int i = 0;;)
    {
        if (key (VK_CONTROL) && key ('N')) 
        {
            RunCreatingButton (button[i]);
            i++;
        }
        if (key (VK_CONTROL) && key ('O')) 
        {
            RunSettingsButton (file);    
        }
        if (key (VK_ESCAPE))
        {
            return 0;
        }
    }


    


    return 0;
}

void RunSettingsButton (FILE *file)
{
    Button button;
    openImage (button.buttonHDC); 
    txBitBlt (0, 0, button.buttonHDC);
    for (;;)
    {

        txMessageBox ("Теперь нажмите на верхную левую часть", "Информация");

        for (;;)
        {
            if (key (VK_ESCAPE))
            {
                //VK_ESCAPEprintf ("%d", key ('Q'));

                return;
            }
            if (txMouseButtons () == 1)
            {
                button.buttonCoor.pos = {.x = txMouseX (), .y = txMouseY ()};
                txSleep (1000);
                break;
            }
        }
    

        for (;;)
        {
            if (key (VK_ESCAPE)) return;
            if (txMouseButtons () == 1)
            {
                button.buttonCoor.size = {.x = txMouseX () -  button.buttonCoor.pos.x, .y = txMouseY () - button.buttonCoor.pos.y};
                break;
            }
        }

        fprintf (file, "Button = {.pos  = {%lf, %lf}, .size = {%lf, %lf}}\n", button.buttonCoor.pos.x, button.buttonCoor.pos.x, button.buttonCoor.size.x, button.buttonCoor.size.y);

        txMessageBox ("Удачно", "Информация");
    }

    if (key (VK_ESCAPE)) return;

}

void RunCreatingButton (Button &button)
{
    openImage (button.buttonHDC);

    button.buttonCoor.pos = {.x = 0, .y = 0};

    for (;;)
    {
        if (txMouseButtons () == 1)
        {
            button.buttonCoor.pos = {.x = txMouseX (), .y = txMouseY ()};
        }
        if (key (VK_CONTROL) && key ('E'))
        {
            break;
        }
        txBitBlt (button.buttonCoor.pos.x, button.buttonCoor.pos.y, button.buttonHDC);
        txSleep (16);
        txClear ();
    }

    txMessageBox ("Теперь Нажмите на верхную левую часть", "Информация");

    for (;;)
    {
        if (txMouseButtons () == 1)
        {
            button.buttonCoor.pos = {.x = txMouseX (), .y = txMouseY ()};
            break;
        }
    }


    for (;;)
    {
        if (txMouseButtons () == 1)
        {
            button.buttonCoor.size = {.x = txMouseX () -  button.buttonCoor.pos.x, .y = txMouseY () - button.buttonCoor.pos.y};
            break;
        }
    }
}

void openImage (HDC &hdc)
{
    char fileName[MAX_PATH] = "";

    OPENFILENAME ofn    = { sizeof (ofn), txWindow() };  //  +-- Загадка Жака Фреско... на размышление дается 20 секунд
                                                        //  V
    ofn.lpstrTitle      = "Файл, который хотите открыть";

    ofn.lpstrFile       = fileName;
    ofn.nMaxFile        = sizeof (fileName);

    ofn.lpstrFilter     = "Image\0" "*.bmp*\0"  // ofn.nFilterIndex = 1
                        "C++ Files\0" "*.cpp*\0"  // ofn.nFilterIndex = 2
                        "All Files\0" "*.*\0";   // ofn.nFilterIndex = 3
    ofn.nFilterIndex    = 1;

    ofn.lpstrInitialDir = NULL;

    ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if ((WIN32::GetOpenFileNameA ))
        (WIN32::GetOpenFileNameA (&ofn) );
    // Весьма полезная функция, отображает диалог выбора файла.
    hdc = txLoadImage (fileName);

    //printf ("\n" "GetOpenFileName() returned: fileName = \"%s\"\n", fileName);
}

bool onButtonClicked (Rect Button)
{
    
    if (txMouseButtons () == 1)
    {
        if (txMouseX () > Button.left () && txMouseX () <  Button.right ())
        {
            if (txMouseY () > Button.top () && txMouseY () <  Button.bottom ())
            {
                    return true;
            }
        }
    }
    return false;

    
}
