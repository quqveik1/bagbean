//#define _TX_ALLOW_TRACE 9
#define  _CRT_SECURE_NO_WARNINGS
#define  TX_USE_SPEAK

#include "TXLib.h"
#include "Q_Vector.h"
#include "Config.h"
#include "Q_Ball.h"
#include "Q_CoordinatSystem.h"
#include "Q_Console.h"
#include <io.h>
#include "Q_App.h"
#include "settingsWindow.cpp"   

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


//const int BallHistoryLength = 20;


//const Rect miniMap =   {.pos = {25,  685}, .size = {375,  285}};

#define key(a) txGetAsyncKeyState (a)
#define endifkey(a) if (key (a)) break;
#define endtillkey(a)           \
{                               \
    if (key (a))                \
    {                           \
        for (;;)                \
        {                       \
            if (!key (a))       \
            {                   \
                break;          \
            }                   \
        }                       \
    }                           \
}
#define printBlt(image)         \
{                               \
    txBitBlt (0, 0, image);     \
    _getch ();                  \
    txClear ();                 \
}

#define ERROR(condition)                   \
{                                           \
    if (condition)                         \
    {                                       \
        printf ("Танк горит!");                                   \
        DebugBreak ();                            \
                              \
    }                                       \
}


App* TheGS = 0;


void RunEngineExperiment ();
void drawEngineFrame (bool drawMiniMapFlag = true);
void Draw (const Vector &vector, const Vector &startPoint, COLORREF colorOfMainVector, double thickness = 1);
void DrawFromStart (const Vector &vector, const Vector &finishPoint, COLORREF colorOfMainVector, double thickness);
void drawArrows (const Vector &mainVector, const  Vector &startArrowPoint);
Vector makePerpendikularLine (const Vector &mainVector);
void drawVectorCircle (const Vector &vector, const Vector &startP, COLORREF color, double thickness = 1);
Vector rotateVector (const Vector &vector, double rad);
void Draw_verVector (const Vector &vector, const Vector &startPoint, COLORREF colorOfMainVector, const double thickness, double angle);
double DegToRad (double degrees);
double lengthV (const Vector &vector);
Vector vectorNormal (Vector vector);

void unitTest (BallSystem ballS, FILE *ballFile);
bool compareNearlyNum (double a, double b);
void copyFrame (BallSystem copyOfMainBallS[], BallSystem ballS, int currFrame);//currFrame(started from 0)
void writeAllBall (FILE *ballFile);
void writeBall (const Ball &ball, FILE *ballFile);

void playSystem (const char path[]);
void drawFrameReplay (BallSystem *ballS);
int readAllBall (BallSystem *ballS, FILE *ballFile);
int readBall (Ball *ball, FILE *ballFile);

double elDeg (const double number, const double deg);
double degreesOfDouble (const double number, int degree);



void loadImage (const char* path, HDC &finalImage, Vector originalSize, Vector finalSize);
void drawMiniMap ();
void drawSysInfo ();
void drawConsole ();
void printFAQ ();
bool inButtonClicked (Rect button);
bool onButtonClicked (Rect Button);
bool inButtonMouse (Rect Button);
bool inRect (Vector pos, Rect rectangle);

void drawAllBall (bool history = true);
void PhysicsAllBall ();
void drawBall (const Ball *ball);
void Control (Ball *ball);
void Physics (Ball *ball, int numberOfFind, bool Graphic);
void cometShooting ();
void CreateNewPlanet (BallSystem &ballS);
Vector returnMouseVector (Vector *finishPos);
Vector findElectricForce (Ball ball[], int numberOfFind, int length);
void Colision (Ball *ball1, Ball *ball2);
void FindColilision (BallSystem &ballS, int numberOfFind);
COLORREF sumColors (COLORREF a, COLORREF b);
void sumColorsUnitTest ();
void ControlAllBalls ();
void ControlAllBalls (BallSystem *ballS);
void ssCircle (Ball ball, Rect Box);
void ssLine (double StartX, double StartY, double FinishX, double FinishY);
void dynamicSleeping ();
Vector txToss (Vector pos);
double SpeedX (double vX);
double SpeedY (double vY);
void ClearBackground ();
void ClearBackground (HDC HUD, Vector startPos = {});
void   SwitchColour (Ball *ball);
double SwitchRadius (double r);
void ControlDelta ();
void makeScreenShot ();
void printfDCS (const char *str = "");
const char* openCustomFile ();
void endOfProgram ();
bool onButtons ();
bool onButtons (Vector pos);
bool onCreateButtons ();



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

/*
mkdir ("C:/realFolders/Name");  //create folder
FILE* myFile = fopen ("C:/ALexProjects/Name.txt", "w");
fprintf (myFile, "Sth = %i", number);                                                             
fclose (myFile);
mode r
fscanf (myFile, "Sth = %d", &newNumber);
char str[size];
fgets (str, size, myfile);
*/



//New
int main()
{

    


    //txCreateWindow (800, 600);
    //txSetColor     (TX_LIGHTRED);
    //txSetFillColor (TX_RED);
    
    (void) _mkdir ("GravitySystemFolder");
    (void) _mkdir ("GravitySystemFolder/ScreenShots");


   
    UserScreen = {.x = (double)GetSystemMetrics (SM_CXSCREEN), .y = (double)GetSystemMetrics (SM_CYSCREEN)};
    CompressK = {.x = UserScreen.x / 1280, .y = UserScreen.y / 720};

    _txWindowStyle &= ~WS_CAPTION;
    MainScreen = txCreateWindow (UserScreen.x, UserScreen.y);
    SetWindowText (MainScreen,  "KVE - Physics Simulation");
    txSetColor     (TX_LIGHTRED);
    txSetFillColor (TX_RED);
    txSelectFont ("Fixedsys", 40 * CompressK.x, 20 * CompressK.y);

    App *gravitySys = new App ();
    TheGS = gravitySys;
    
    //int mode = 0;
    

    

    loadImage ("GravitySystemFolder/DevMaterials/Menu720p_1.bmp", TheGS->Menu, {1280, 720}, UserScreen);
    loadImage ("GravitySystemFolder/DevMaterials/HUD720p_1.bmp", TheGS->HUD, {1280, 720}, UserScreen);
    loadImage ("GravitySystemFolder/DevMaterials/LeftButtons720p.bmp", TheGS->LeftButtonsImage, {605, 157}, {605 * CompressK.x, 157 * CompressK.y});    
    loadImage ("GravitySystemFolder/DevMaterials/CreateButtonHud.bmp", TheGS->CreateModeHDC, {760, 140}, {760 * CompressK.x, 140 * CompressK.y});    
    loadImage ("GravitySystemFolder/DevMaterials/HUD720p_Replay_1.bmp", TheGS->ReplayHUD, {1280, 720}, UserScreen);
   
    

    TheGS->monitorS = {.pos= {0,0}, .size = UserScreen};
    TheGS->miniMap.startPosPix_ = {0, 490 * CompressK.y};
    TheGS->miniMap.scalePix_ = {290 * CompressK.x, 230 * CompressK.y};
    TheGS->miniMap.coorSize_ = {(double) txGetExtentX (), (double)txGetExtentY ()};
    TheGS->miniMap.intepretK_= {{TheGS->miniMap.scalePix_.x / TheGS->miniMap.coorSize_.x}, {TheGS->miniMap.scalePix_.y / TheGS->miniMap.coorSize_.y}};
    

    TheGS->LeftButtons = {.pos = {0, 0}, .size = {605 * CompressK.x, 157 * CompressK.y}};
    /*
    txBitBlt (0, 0, TheGS->LeftButtonsImage);
    _getch ();
    txBitBlt (0, 0, TheGS->LeftButtonsImage, TheGS->LeftButtons.size.x, TheGS->LeftButtons.size.y);
    _getch ();
    */
    TheGS->exitButtonHUD = {.pos = {1090 * CompressK.x, 0}, .size = {190 * CompressK.x, 80 * CompressK.y}};
    TheGS->newPlanetButton = {.pos = {0 * CompressK.x, 0}, .size = {300 * CompressK.x, 100 * CompressK.y}};
    TheGS->screenShotButton = {.pos = {0, 106 * CompressK.y}, .size = {160 * CompressK.x, 50 * CompressK.y}};
    TheGS->minusSpeedButton = {.pos = {168 * CompressK.x, 106 * CompressK.y}, .size = {43 * CompressK.x, 51 * CompressK.y}};
    TheGS->plusSpeedButton = {.pos = {220 * CompressK.x, 106 * CompressK.y}, .size = {43 * CompressK.x, 51 * CompressK.y}};
    TheGS->cleanButton = {.pos = {305 * CompressK.x, 0}, .size = {300 * CompressK.x, 100 * CompressK.y}};

    TheGS->rMinusButton = {.pos = {650 * CompressK.x, 5 * CompressK.y}, .size = {50 * CompressK.x, 60 * CompressK.y}};
    TheGS->rPlusButton = {.pos  = {706 * CompressK.x, 5 * CompressK.y}, .size = {50 * CompressK.x, 60 * CompressK.y}};
    TheGS->mMinusButton = {.pos  = {650 * CompressK.x, 80 * CompressK.y}, .size = {50 * CompressK.x, 60 * CompressK.y}};
    TheGS->mPlusButton = {.pos  = {706 * CompressK.x, 80 * CompressK.y}, .size = {50 * CompressK.x, 60 * CompressK.y}};


    //printfDCS ("Menu");
    //txBitBlt (0, 0, TheGS->Menu);


    TheGS->startButton = {.pos = {15 * CompressK.x, 127 * CompressK.y}, .size = {445 * CompressK.x, 198 * CompressK.y}};
    TheGS->repeatButton = {.pos = {15 * CompressK.x, 358 * CompressK.y}, .size = {445 * CompressK.x, 198 * CompressK.y}};
    TheGS->infoButton = {.pos = {15 * CompressK.x, 485 * CompressK.y}, .size = {105 * CompressK.x, 215 * CompressK.y}};
    TheGS->exitButton = {.pos = {1092 * CompressK.x, 637 * CompressK.y}, .size = {188 * CompressK.x, 84 * CompressK.y}};
       
    

    //TheGS->Menu = txLoadImage ("GravitySystemFolder/DevMaterials/Menu720p.bmp");
         
         
         //TheGS->ReplayHUD = txLoadImage ("GravitySystemFolder/DevMaterials/Hud720p_Replay.bmp");

    
    if (Resolution == 720)
    {  
        /*
        // TheGS->sysInfo = {.pos = {15, 20}, .size = {255, 170}};
         //TheGS->console.startPosPix_ = {15, 220};
         //TheGS->console.scalePix_ = {255, 140};
         
         TheGS->miniMap.startPosPix_ = {0, 410};
         TheGS->miniMap.scalePix_ = {275, 190};
         TheGS->miniMap.coorSize_ = {(double) txGetExtentX (), (double)txGetExtentY ()};
         TheGS->miniMap.intepretK_= {{TheGS->miniMap.scalePix_.x / TheGS->miniMap.coorSize_.x}, {TheGS->miniMap.scalePix_.y / TheGS->miniMap.coorSize_.y}};
         TheGS->exitButtonHUD = {.pos = {1020, 0}, .size = {180, 70}};
         TheGS->newPlanetButton = {.pos = {0, 0}, .size = {280, 80}};
         TheGS->screenShotButton = {.pos = {0, 90}, .size = {150, 40}};
         TheGS->minusSpeedButton = {.pos = {157, 90}, .size = {40, 40}};
         TheGS->plusSpeedButton = {.pos = {205, 90}, .size = {40, 40}};
         TheGS->cleanButton = {.pos = {285, 0}, .size = {280, 80}};
         //TheGS->mainPlace = {.pos = {290, 0}, .size = {(double) (txGetExtentX () - 290), (double) (txGetExtentY () - 0)}};
         TheGS->HUD = txLoadImage ("GravitySystemFolder/DevMaterials/Hud720p.bmp");
         //TheGS->path = "GravitySystemFolder/DevMaterials/Hud720p.bmp";


         //printfDCS ("Menu");
         //txBitBlt (0, 0, TheGS->Menu);

         TheGS->startButton = {.pos = {15, 105}, .size = {415, 160}};
         TheGS->repeatButton = {.pos = {15, 300}, .size = {415, 160}};
         TheGS->infoButton = {.pos = {15, 485}, .size = {100, 100}};
         TheGS->exitButton = {.pos = {1020, 530}, .size = {180, 70}};
         
         TheGS->Menu = txLoadImage ("GravitySystemFolder/DevMaterials/Menu720p.bmp");
         
         
         TheGS->ReplayHUD = txLoadImage ("GravitySystemFolder/DevMaterials/Hud720p_Replay.bmp");
         */
    }
    

    if (Resolution == 1080)
    {
        /*
        TheGS->sysInfo = {.pos = {15, 20}, .size = {255, 170}};
        TheGS->console.startPosPix_ = {15, 220};
        TheGS->console.scalePix_ = {255, 140};
        
        TheGS->miniMap.startPosPix_ = {0, 680};
        TheGS->miniMap.scalePix_ = {420, 300};
        TheGS->miniMap.coorSize_ = {(double) txGetExtentX (), (double)txGetExtentY ()};
        TheGS->miniMap.intepretK_= {{TheGS->miniMap.scalePix_.x / TheGS->miniMap.coorSize_.x}, {TheGS->miniMap.scalePix_.y / TheGS->miniMap.coorSize_.y}};
        TheGS->cleanButton = {.pos = {430, 0}, .size = {400, 140}};
        TheGS->exitButtonHUD = {.pos = {1535, 0}, .size = {(double) txGetExtentX () - 1535, 120}};
        TheGS->newPlanetButton = {.pos = {0, 0}, .size = {420, 135}};
        TheGS->screenShotButton = {.pos = {0, 155}, .size = {220, 60}};
        TheGS->minusSpeedButton = {.pos = {235, 150}, .size = {65, 60}};
        TheGS->plusSpeedButton = {.pos = {300, 150}, .size = {65, 60}};
        TheGS->HUD = txLoadImage ("GravitySystemFolder/DevMaterials/Hud1080p.bmp");
        //TheGS->path = "GravitySystemFolder/DevMaterials/Hud1080p.bmp";

        //TheGS->mainPlace = {.pos = {290, 0}, .size = {(double) (txGetExtentX () - 290), (double) (txGetExtentY () - 0)}};
        
        TheGS->startButton = {.pos = {25, 180}, .size = {620, 270}};
        TheGS->repeatButton = {.pos = {25, 500}, .size = {620, 270}};
        TheGS->infoButton = {.pos = {25, 815}, .size = {140, 160}};
        TheGS->exitButton = {.pos = {1535, 880}, .size = {(double) txGetExtentX () - 1535, (double)txGetExtentY () - 880}};
        TheGS->Menu = txLoadImage ("GravitySystemFolder/DevMaterials/Menu1080p.bmp");

        
        TheGS->ReplayHUD =  txLoadImage ("GravitySystemFolder/DevMaterials/Hud1080p_Replay.bmp");
         */
    }
    



    //TheGS->HUD = txLoadImage (TheGS->path);
    //printfDCS ("HUD");
    //TheGS->ReplayHUD = txLoadImage ("GravitySystemFolder/DevMaterials/Hud720p_Replay.bmp");
    //printfDCS ("REplayHud");
    
    
    //H//ICON hicon = (HICON) TheGS->Menu;
    //DrawIconEx (TheGS->Menu, 1800, 1000, hicon, 32, 32, NULL, DI_NORMAL, );
    txBegin ();

    for (;;)
    {
        SLEEPINGTIME = 30;
        txBitBlt (0, 0, TheGS->Menu);
        txSleep (0);
        for (;;)
        {
           
            //RECT r = {TheGS->startButton.left(), TheGS->startButton.top(), TheGS->startButton.right(), TheGS->startButton.bottom()};
            //POINT point = {txMousePos ().x, txMousePos ().y}; 
            if (onButtonClicked (TheGS->startButton))
            {
                if (key (VK_TAB))
                {
                    TheGS->writeSaves = false;
                }
                TheGS->ballS.currlength = 0;

                if (!key (VK_CONTROL))
                {
                    TheGS->ballS.ball [TheGS->ballS.currlength++] = {txToss ({0, 0}),     {0, 0},  1e17, 10, 2e5, TX_YELLOW};
                    TheGS->ballS.ball [TheGS->ballS.currlength++] = {txToss ({0, -400}),  {24, 0}, 1e4,  10, 2,   TX_CYAN};
                    TheGS->ballS.ball [TheGS->ballS.currlength++] = {txToss ({0, -200}),  {32, 0}, 1e4,  10, 2,   TX_RED};
                }
        

                /*
                ballS.addBall ({txToss ({0, 0}),   {0, 0}, 1e17, 10, 2e5, TX_RED});
                ballS.addBall ({txToss ({0, -400}),  {24, 0}, 1e4, 10, 2, TX_RED});
                ballS.addBall ({txToss ({0, -200}),  {10, 0}, 1e4, 10, 2, TX_RED}};
                */

                for (int i = TheGS->ballS.currlength; i < BallMax; i++)
                {
                    assert (0 <= i && i < BallMax);

                    TheGS->ballS.ball[i] = {txToss ({0, 0}),   {0, 0}, 0, 0, 0, TX_RED};
                }

                //ball[3] = {txToss ({0, -100}),  {10, 0}, 1e4, 10, 2, TX_RED};

                for (int i = 0; i < BallMax; i++)
                {
                    assert (0 <= i && i < BallMax);

                    TheGS->planetsInit[i] = TheGS->ballS.ball[i];
                }
                /*
                for (int n = 0; n < 1; n++)
                {   */
                for (int i = 0; i < TheGS->ballS.currlength; i++)
                {
                    assert (0 <= i && i < BallMax);
                    TheGS->ballS.ball[i] = TheGS->planetsInit[i];
                }

        
                RunEngineExperiment ();
                TheGS->writeSaves = true;
                
                break;
            }

       
            if (onButtonClicked (TheGS->repeatButton))
            {
                if (key (VK_CONTROL))
                {
                    const char *path = openCustomFile ();
                    //printf ("\"%s\"", path);
                    if (strcmp (path, ""))
                    {
                        playSystem (path);
                    }
                }
                else
                {
                    playSystem ("GravitySystemFolder/EngineExperiment.saves");
                }
                break;
            }

            if (onButtonClicked (TheGS->infoButton))
            {
                printFAQ ();
                continue;
            }

            if (onButtonClicked (TheGS->exitButton) || txGetAsyncKeyState ('O'))
            {
                endOfProgram ();
                delete TheGS;
                return 0;
            }

           if (key ('5') && key ('7'))
           {
               
                txSpeak ("Я роняю Запад, у! Я роняю Запад, у!\n"
                         "Я роняю Запад, у! Я роняю Запад, а!\n"
                         "Я роняю Запад, у! Я роняю запад, а!\n"
                         "На моём хуе вся индустрия США (ха-ха!)\n"
                         "Я роняю Запад, у! Я роняю Запад, эй!\n"
                         "Я роняю Запад! (эщкере!) Я роняю Запад, эй!\n"
                         "Я роняю Запад, у! Я роняю запад, а!\n"
                         "На моём хуе вся индустрия США, я\n" 
                ); 
           }

           if (key ('1') && key ('3') && key ('2') && key('9'))
           {
               //txPlaySound ("GravitySystemFolder/DevMaterials/FACE-Fiksiki.wav", SND_ASYNC);
           }
        }
    }

    
    txEnd ();

    endOfProgram ();
    return 0;
} 

void loadImage (const char* path, HDC &finalImage, Vector originalSize, Vector finalSize)
{
    finalImage = txCreateCompatibleDC (finalSize.x, finalSize.y);

    HDC Copyimage= txLoadImage (path);
    assert (Copyimage);

    StretchBlt (finalImage, 0, 0, finalSize.x, finalSize.y,  Copyimage, 0, 0, originalSize.x, originalSize.y, SRCCOPY);
    txDeleteDC (Copyimage);
}

/*
void loadImage (HDC &MainHDC, const char* imagePath, const char* backGroundPath /*= NULL, Vector backGroundSize =NULL)
{
    HDC backGround = txLoadImage (backGroundPath);   
    printBlt (backGround);
    HDC image = txLoadImage (emptyFilePath);
    MainHDC =  txLoadImage (emptyFilePath);
    printBlt (image);
    txBitBlt (image, 0, 0, backGroundSize.x, backGroundSize.y, backGround, 0, 0);
    printBlt (image);
    HDC Copyimage= txLoadImage (imagePath);
    StretchBlt (image, 0, 0, UserScreen.x, UserScreen.y,  Copyimage, 0, 0, TheGS->LeftButtons.size.x, TheGS->LeftButtons.size.y, SRCCOPY);
    txBitBlt (MainHDC, 0, 0, UserScreen.x, UserScreen.y, image, 0, 0);
    printBlt (MainHDC);
    txDeleteDC (Copyimage);  
}
*/

/*
void loadImage (HDC image, HDC backGround /*= NULL, Vector backGroundSize /*=NULL)
{
    image = txLoadImage (emptyFilePath);
    if (backGround != NULL) txBitBlt (image, 0, 0, backGroundSize.x, backGroundSize.y, backGround, 0, 0);
    HDC Copyimage= txLoadImage ("GravitySystemFolder/DevMaterials/HUD720p_1.bmp");
    StretchBlt (TheGS->HUD, 0, 0, UserScreen.x, UserScreen.y,  Copyimage, 0, 0, 1280, 720, SRCCOPY);
    printBlt (TheGS->HUD);
    txDeleteDC (Copyimage);  
}
*/

/*
HRESULT Load(LPCTSTR szFile)
    {
        CComPtr<IStream> pStream;
        
        // Load the file to a memory stream
        HRESULT hr = FileToStream(szFile, &pStream);

        if (SUCCEEDED(hr))
        {
            // Decode the picture
            hr = ::OleLoadPicture(
                    pStream,            // [in] Pointer to the stream that contains picture's data
                    0,                    // [in] Number of bytes read from the stream (0 == entire)
                    true,                // [in] Loose original format if true
                    IID_IPicture,        // [in] Requested interface
                    (void**)&m_pPicture // [out] IPictire object on success
                    );
        }

        return hr;
    }

    HRESULT DrawImg(HDC hdc, const RECT& rcBounds)
    {
        if (m_pPicture)
        {
            // Get the width and the height of the picture
            long hmWidth = 0, hmHeight = 0;
            m_pPicture->get_Width(&hmWidth);
            m_pPicture->get_Height(&hmHeight);

            // Convert himetric to pixels
            int nWidth    = MulDiv(hmWidth, ::GetDeviceCaps(hdc, LOGPIXELSX), HIMETRIC_INCH);
            int nHeight    = MulDiv(hmHeight, ::GetDeviceCaps(hdc, LOGPIXELSY), HIMETRIC_INCH);

            // Display the picture using IPicture::Render
            return m_pPicture->Render(
                hdc,                            // [in] Handle of device context on which to render the image
                rcBounds.left,                    // [in] Horizontal position of image in hdc
                rcBounds.top,                    // [in] Vertical position of image in hdc
                rcBounds.right - rcBounds.left,    // [in] Horizontal dimension of destination rectangle
                rcBounds.bottom - rcBounds.top, // [in] Vertical dimension of destination rectangle
                0,                                // [in] Horizontal offset in source picture
                hmHeight,                        // [in] Vertical offset in source picture
                hmWidth,                        // [in] Amount to copy horizontally in source picture
                -hmHeight,                        // [in] Amount to copy vertically in source picture
                &rcBounds                        // [in, optional] Pointer to position of destination for a metafile hdc
                );
        }

        return E_UNEXPECTED;
    }
    */


const char* openCustomFile ()
{
    char fileName[MAX_PATH] = "";

    OPENFILENAME ofn    = { sizeof (ofn), txWindow() };  //  +-- Загадка Жака Фреско... на размышление дается 20 секунд
                                                        //  V
    ofn.lpstrTitle      = "Файл, который нужно проиграть";

    ofn.lpstrFile       = fileName;
    ofn.nMaxFile        = sizeof (fileName);

    ofn.lpstrFilter     = "Saves\0" "*.saves*\0"  // ofn.nFilterIndex = 1
                        "All Files\0" "*.*\0";   // ofn.nFilterIndex = 3
    ofn.nFilterIndex    = 1;

    ofn.lpstrInitialDir = NULL;

    ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

   if ((WIN32::GetOpenFileNameA ))
        (WIN32::GetOpenFileNameA (&ofn) );
    // Весьма полезная функция, отображает диалог выбора файла.

    ///printf ("\n" "GetOpenFileName() returned: fileName = \"%s\"\n", fileName);

    return fileName;
}




void endOfProgram ()
{
    txEnd ();
    txDisableAutoPause ();
    txDeleteDC (TheGS->Menu);
    txDeleteDC (TheGS->HUD);
    txDeleteDC (TheGS->ReplayHUD);
    txDeleteDC (TheGS->LeftButtonsImage);
    txDeleteDC (TheGS->CreateModeHDC);
}

void printfDCS (const char *str)
{
    printf ("%s\n", str);
    for (int i = 0; i < _txCanvas_UserDCs->size (); i++)
    {
        printf ("%d: %p\n", i, (*_txCanvas_UserDCs)[i]);
    }
    _getch ();
}

/*
int main2()
{
    txCreateWindow (1900, 1000);
    txSetColor     (TX_LIGHTRED);
    txSetFillColor (TX_RED);
    Ball ball[BallLength] = {};
    Ball planetsInit[BallLength] = {};
    
    planetsInit[0] = {txToss ({0, 0}),   {0, 0}, 1e17, 10, 2, TX_RED};
    planetsInit[1] = {txToss ({0, -400}), {56, 56}, 1e4, 10, 2, TX_RED};
    txBegin ();
    RunEngineE2 (planetsInit);
    txEnd ();
    return 0;
}
*/


void CreateNewPlanet (BallSystem &ballS)
{
    Ball newBall = {};
    newBall.pos = {UserScreen.x/2, UserScreen.y/2};
    newBall.m      = 1e4;
    newBall.charge = 2e1;
    newBall.r      = 10;
    Vector MousePos = {};
    bool lastOperationR = true;

    for (;;)
    {
        //printf ("%d", !onCreateButtons ());
        if (txMouseButtons () == 2)
        {
            newBall.pos = {.x = txMouseX (), .y = txMouseY ()};
        } 

        if (onButtonClicked (TheGS->rPlusButton))
        {
            lastOperationR = true;
            newBall.r++;
        }
        if (onButtonClicked (TheGS->rMinusButton))
        {
            lastOperationR = true;
            newBall.r--;
        }


        if (onButtonClicked (TheGS->mMinusButton))
        {
            lastOperationR = false;
            newBall.m--;
        }
        if (onButtonClicked (TheGS->mPlusButton))
        {
            lastOperationR = false;
            newBall.m++;
        }

   

        if (txMouseButtons () == 1 && !onCreateButtons ()) 
        { 
            MousePos  = {txMouseX(), txMouseY()};
            newBall.v = newBall.pos - MousePos;
        }

        DrawFromStart (newBall.v, newBall.pos, TX_CYAN, 5);
        drawAllBall (false);
        drawMiniMap ();
        char rInfo[100] = {};
        char mInfo[100] = {};
        sprintf (rInfo, "%d", (int) newBall.r);
        sprintf (mInfo, "%d", (int) newBall.m);
        $s;
        txSetColor (TX_WHITE);
        txTextOut (220 * CompressK.x, 20 * CompressK.y + 2, rInfo);
        txTextOut (190 * CompressK.x, 90 * CompressK.y + 2, mInfo);


        
       

        if (onButtonClicked (TheGS->exitButtonHUD)) break;

        newBall.draw ();
        txSleep  (SLEEPINGTIME);
        //txBitBlt (0,0, TheGS->HUD);
        //txBitBlt (0,0, TheGS->LeftButtonsImage);
        ClearBackground (TheGS->HUD);
        ClearBackground (TheGS->CreateModeHDC);
    }

    TheGS->ballS.addBall (newBall);





    /*
    bool escape = 0;

    const char *cposx = txInputBox ("Введите координату по x:", "Создание новой планеты", "400");
    if (!strcmp (cposx, "")) return;
    const char *cposy = txInputBox ("Введите координату по y:", "Создание новой планеты", "400");
    if (!strcmp (cposy, "")) return;
    assert (cposx);
    Vector newPos = {};
    if (!sscanf (cposx, "%lf", &newPos.x)) return;
    if (!sscanf (cposy, "%lf", &newPos.y)) return;


    const char *cvx = txInputBox ("Введите скорость по x:", "Создание новой планеты", "20");
    if (!strcmp (cvx, "")) return;
    const char *cvy = txInputBox ("Введите скорость по y:", "Создание новой планеты", "0");
    if (!strcmp (cvy, "")) return;
    Vector newV = {};
    sscanf (cvx, "%lf", &newV.x);
    sscanf (cvy, "%lf", &newV.y);

    const char *ccharge = txInputBox ("Введите заряженность(притяжение работает благодаря этому):", "Создание новой планеты", "10");
    if (!strcmp (ccharge, "")) return;
    double newCharge = 0;
    sscanf (ccharge, "%lf", &newCharge);


    const char *cm = txInputBox ("Введите массу:", "Создание новой планеты", "1000");
    if (!strcmp (cm, "")) return;
    double newm = 0;
    sscanf (cm, "%lf", &newm);

    const char *cr = txInputBox ("Введите радиус:", "Создание новой планеты", "10");
    if (!strcmp (cr, "")) return;
    double newR = 0;
    sscanf (cr, "%lf", &newR);

    Ball newBall;
    newBall.pos = newPos;
    newBall.v = newV;
    newBall.charge = newCharge;
    newBall.m = newm;
    newBall.r = newR;


    
    ballS.addBall (newBall);
    */


}

bool onCreateButtons ()
{
    return  inButtonMouse (TheGS->exitButtonHUD) ||
            inButtonMouse (TheGS->rMinusButton)  ||
            inButtonMouse (TheGS->rPlusButton)   ||
            inButtonMouse (TheGS->mMinusButton)  ||
            inButtonMouse (TheGS->mPlusButton);
            
}

void unitTest (BallSystem ballS, FILE *ballFile)
{
    static int frameCount = 1;
    //txClearConsole ();

    bool correctInput = true;
    double posX = 0;
    double posY = 0;
    unsigned int color = 0;
    double  r = 0;
    unsigned int alive = 0;

    //printf ("Frame: %i\n", frameCount);
    frameCount++;

    for (int i = 0; i < BallMax; i++)
    {

        assert (0 <= i && i < BallMax);
        
        //OFF static char str[500] = "";

        //fgets (str, sizeof (str), ballFile);
        //printf ("//%s//\n", str);
        //(void) _getch ();

        printf ("Readed elements: %i////", fscanf (ballFile, " { { %lf , %lf } , color: %u , r: %lf , alive: %u } | | |", &posX, &posY, &color, &r, &alive)); //!!!!!!!!!!!
        
        
        
        
        printf ("Ball: %i:", i + 1);

        assert (0 <= i && i < BallMax);

        double deltaOfPosX = fabs (ballS.ball[i].pos.x - posX);

        //printf ("Error: pos ({%e, %e}, {%e, %e})\n", ballS.ball[0].pos.x, ballS.ball[0].pos.y, posX, posY);
       // printf ("ERROR: Delta = %e /// pos ({%d, %d}, {%d, %d})\n", deltaOfPosX, (int) ballS.ball[i].pos.x, (int) ballS.ball[i].pos.y, (int) posX, (int) posY);
        if (!(compareNearlyNum (ballS.ball[i].pos.x, posX)) || !(compareNearlyNum (ballS.ball[i].pos.y, posY)) )
        {
            correctInput = false;
            $se;
            printf ("Error: Delta = %e /// pos ({%e, %e}, {%e, %e}) ", deltaOfPosX, ballS.ball[i].pos.x, ballS.ball[i].pos.y, posX, posY);
            //DebugBreak ();
        }
        
        assert (0 <= i && i < BallMax);
        if ( !(compareNearlyNum (ballS.ball[i].r, r)) )
        {
            correctInput = false;
            $se;
            printf ("Error: r (%lg, %lg) ", ballS.ball[i].r, r);
        }

        assert (0 <= i && i < BallMax);
        if ((unsigned int) ballS.ball[i].color != color)
        {
            correctInput = false;
            $se;
            printf ("Error: color (%u, %u) ", ballS.ball[i].color, color);
        }

        assert (0 <= i && i < BallMax);
        if ((unsigned int) ballS.ball[i].alive != alive)
        {
            correctInput = false;
            $se;
            printf ("Error: alive (%u, %u) ", ballS.ball[i].alive, alive);
        }

        printf ("\n");
    }

    printf ("\n");

    (void) _getch ();

    //void (printf ("Correct: %u\n", (unsigned int) correctInput));
}


bool compareNearlyNum (double a, double b)
{
    double moduleDelta = fabs (a - b);
    return (moduleDelta <= MaxDeltaPrecision);
}

void playSystem (const char path[])
{
    FILE *ballSystemRecording = fopen (path, "r");  
    //printf ("%p", ballSystemRecording);
    BallSystem currBallS = {};
     

    for (;;)
    {
        ClearBackground (TheGS->ReplayHUD);

        int readCode = readAllBall (&currBallS, ballSystemRecording);
        if (readCode == 0)
        {
            txMessageBox ("Программа не может считать данные сохранений", "Ошибка", MB_OK);
            return;
        }
        if (readCode == -1)
        {
            txMessageBox ("Повтор закончился", "Окончание повтора", MB_OK);
            return;
        }

        drawFrameReplay (&currBallS);

        if (txGetAsyncKeyState ('O')) break;
        if (onButtonClicked (TheGS->exitButtonHUD)) break;
        ControlDelta ();
        txSleep (SLEEPINGTIME);
    }

    fclose (ballSystemRecording);

}

void dynamicSleeping ()
{
    static int timesleep = SLEEPINGTIME;

    if (txGetAsyncKeyState (VK_F1)) timesleep = 10;
    if (txGetAsyncKeyState (VK_F2)) timesleep = 50;
    if (txGetAsyncKeyState (VK_F3)) timesleep = 100;
    if (txGetAsyncKeyState (VK_F4)) timesleep = 200;
    if (txGetAsyncKeyState (VK_F5)) timesleep = 500;
    if (txGetAsyncKeyState (VK_F6)) timesleep = 1000;

    //printf ("%i", timesleep);

    txSleep (timesleep);
}


void drawFrameReplay (BallSystem *ballS)
{
    for (int i = 0; i < BallMax; i++)
    {
        assert (0 <= i && i < BallMax);

        if (ballS->ball[i].alive)
            drawBall ( &(ballS->ball[i]));    
    }
}

void writeBall (const Ball &ball, FILE *ballFile)
{
    fprintf (ballFile, "{{%7.*lf, %7.*lf}, color: %7u, r: %5.2lf, alive: %u} ||| ", RoundingPrecision, ball.pos.x, RoundingPrecision, ball.pos.y, ball.color, ball.r, ball.alive);
}

void writeAllBall (FILE *ballFile)
{
    //OFF fprintf (ballFile, "/BEGIN  :::  ");
    fprintf (ballFile, "Num: %d||", TheGS->ballS.currlength);

    for (int i = 0; i < TheGS->ballS.currlength; i++)
    {
        assert (0 <= i && i < BallMax);

        writeBall (TheGS->ballS.ball[i], ballFile);
    }
    
    fprintf (ballFile, "\n");
}

int readAllBall (BallSystem *ballS, FILE *ballFile)
{
    int size = 0;
    int readCode = fscanf (ballFile, "Num: %d|| ", &size);
    if (readCode == 0 || readCode == -1)
    {
        return readCode;
    }

    for (int i = 0; i < size; i++)
    {
        assert (0 <= i && i < BallMax);
        readCode = readBall ( &(ballS->ball[i]), ballFile);

        if (readCode == 0 || readCode == -1)
        {
            return readCode;
        }
    }
    return 100;

    //lining ();
}

int readBall (Ball *ball, FILE *ballFile)
{
    return fscanf (ballFile, "{{%lf, %lf}, color: %u, r: %lf, alive: %u} ||| ", &ball->pos.x , &ball->pos.y, &ball->color, &ball->r, (unsigned int*) &ball->alive);
    //printf (          "{{%lf, %lf}, color: %u, r: %lf, alive: %u}\n ",    ball->pos.x ,  ball->pos.y,  ball->color,  ball->r,                  ball->alive);
    //_getch();
    
}



void RunEngineExperiment ()
{
    //txSetProgress (30);
    txSetProgress (90, Win32::TBPF_INDETERMINATE);

    TheGS->console.print ("Press f1 to open FAQ");
    FILE *ballSystemRecording = {};

    if (TheGS->writeSaves)
    {
        ballSystemRecording = fopen ("GravitySystemFolder/EngineExperiment.saves", "w");
       
        //printf ("%p|||", ballSystemRecording);
        //DebugBreak ();
        assert (ballSystemRecording);
    }
        
    for (int i = 0; i > -1; i++)
    {
        //TheGS->getWindowPos ();

        PhysicsAllBall ();

        PhysicsAllBall ();

        PhysicsAllBall ();
        //DebugBreak ();

        ControlAllBalls ();

        drawEngineFrame ();

         /*
        drawAllBall ();

        drawMiniMap ();
        */


        //printf ("%p|| %d", ballSystemRecording, TheGS->writeSaves);
        if (TheGS->writeSaves)
            writeAllBall (ballSystemRecording);

        //DebugBreak ();

        int fps = txGetFPS ();
        if (fps < 30) SLEEPINGTIME--;
        if (fps > 30) SLEEPINGTIME++;
        txSleep (SLEEPINGTIME);

        if (txGetAsyncKeyState('O')) break;

        if (onButtonClicked (TheGS->exitButtonHUD)) break;

        //ClearBackground (TheGS->HUD);
        
        ClearBackground (TheGS->HUD);
        ClearBackground (TheGS->LeftButtonsImage);
    }
    txSetProgress (0);

    if (TheGS->writeSaves)
    {
        fclose (ballSystemRecording); 
    }

}

void drawEngineFrame (bool drawMiniMapFlag)
{
    drawAllBall ();
    if (drawMiniMapFlag) drawMiniMap ();

}



void drawConsole ()
{
    char str[100] = "";

    // int n = 0;
    

    if (txGetAsyncKeyState (VK_CONTROL)) sprintf (str, "Нажата клавиша: ctrl");
    if (txGetAsyncKeyState (VK_TAB)) sprintf (str, "Нажата клавиша: tab");
    if (txGetAsyncKeyState (VK_F1)) 
    {
        printFAQ ();
    }

    if (strcmp (str, "") != 0)
    {
        TheGS->console.print (str);
    }
    //Какой оператор си инвертирует число! ~
    //!(т) (любое число неравное нулю)
    //false
    //!(0) true
    //0 - ложь, все кроме нуля правда
    //n | n != 0: !(n) = 0
    //!(0) = 1
    //n | n != 0: !!(n) = 1
    //!!(0) = 0
    //!!0 = 0
    //!!1 = 1
    //!!2 = 1
    //!!3 = 1
    
    //printf ("%с", "tf")
    //Чем отличчается %s %c
    //%s - строка
    //%c - char символ


    TheGS->console.reDraw ();


        


}
bool inButtonClicked (Rect button)
{
    if (txMouseButtons () == 1)
    {
        if (txMouseX () > button.left () && txMouseX () <  button.right ())
        {
            if (txMouseY () > button.top () && txMouseY () <  button.bottom ())
            {
                return true;
            }
        }
    }
    return false;
}

bool onButtonClicked (Rect Button)
{
    
    if (txMouseButtons () == 1)
    {
        if (txMouseX () > Button.left () && txMouseX () <  Button.right ())
        {
            if (txMouseY () > Button.top () && txMouseY () <  Button.bottom ())
            {
                while (txMouseButtons () == 1)
                {
                };
                if (txMouseX () > Button.left () && txMouseX () <  Button.right ())
                {
                    if (txMouseY () > Button.top () && txMouseY () <  Button.bottom ())
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;

    
}

void printFAQ ()
{
    txMessageBox ("Это простая физическая симуляция\n\n"
                      "Для запуска просто запустите .exe, если вдруг антивирус винды предложит отказаться, то нажмите подробнее, выполнить в любом случае\n\n"
                      "Инструкция по управлению:\n\n"
                      "0. При запуске у вас будет меню Режим (\"Старт\" / \"Повтор\") \"Повтор\" - это проигрывание предыдущей симуляции, \"Старт\" - создание новой (При одновременном нажатии на Ctrl создается пустая симуляция)\n\n"
                      "1. Для запуска кометы зажмите ЛКМ и задайте рогаткой вектор полета\n"
                      "1.1 Для запуска кометы с заданием значений нажмите кнопку \"Новая планета\", задавайте положение планеты с помощью ПКМ, а вектор скорости с помощью ЛКМ\n\n"
                      "2. При нажатии на кнопку \"История\" будет включено отображение история перемещения планет\n\n"
                      "3. Стрелками влево, вправо и другими можно изменять скорость\n\n"
                      "4. С - чистить экран при перерисовки каждого кадра, N - обратное\n\n"
                      "5. W - уменьшение размера планет, Е - противоположное\n\n" 
                      "P.S. Можете понажимать разные клавиши, тут очень много функции\n\n",//!!!Автроы
                      "Инструкция по использованию", MB_OK);
}


void drawMiniMap ()
{
        for (int i = 0; i < TheGS->ballS.currlength; i++)
        {
            if (TheGS->ballS.ball[i].alive)
            {
               TheGS->miniMap.drawCircle (TheGS->ballS.ball[i]);
            }
        }

        //miniMap.drawCircle (pos);
        //txSleep (100);

}

void drawSysInfo ()
{
    char info [200] = "";
    for (int i = 0; i < TheGS->ballS.currlength; i++)
    {
        if (TheGS->ballS.ball[i].alive)
        {
            sprintf (info, "Ball %i = {%f, %f}\n", i, TheGS->ballS.ball[i].pos.x, TheGS->ballS.ball[i].pos.y);

            txTextOut (0 + TheGS->sysInfo.pos.x, (0 + TheGS->sysInfo.pos.x) + 25 * i, info);
        }
    }
}


void copyFrame (BallSystem copyOfMainBallS[], BallSystem ballS, int currFrame)//currFrame(started from 0)
{
    copyOfMainBallS[currFrame] = ballS;
}

/*
void RunEngineE2 (const Ball planetsInit[])
{
    Ball ball[BallLength] = {};
    ball[0] = {txToss ({0, 0}),   {0, 0}, 1e17, 10, 2, TX_RED};
    ball[1] = {txToss ({0, -400}), {56, 56}, 1e4, 10, 2, TX_RED};
    bool flagClearBackground = true;
        
    for (;;)
    {
        for (int i = 0; i < BallLength; i++) ball[i] = planetsInit[i];
        printf ("Vx = ");
        scanf  ("%lg", &ball[1].v.x);
        printf ("Vy = ");
        scanf  ("%lg", &ball[1].v.y);
        for (;;)
        {
            //solarSystem (ball);
            PhysicsAllBall (ball);
            //solarSystem (ball);
            PhysicsAllBall (ball);
            //solarSystem (ball);
            PhysicsAllBall (ball);
            ControlAllBalls (ball);
            drawAllBall (ball);
            txSleep (20);
            if (txGetAsyncKeyState('Q')) break;
            flagClearBackground = ClearBackground (flagClearBackground);
        }
    }
    
}
*/





/*
void solarSystem (Ball balls[])
{
    const Rect box = { {0, 0}, {txGetExtent().x, txGetExtent().y} };
    for (int i = 1; i < BallLength; i++)
    {
        if (balls[i].pos.x < 0.5 * box.right () && balls[i].pos.y < 0.5 * box.bottom ())
        {
            balls[i].v = {-56, 56};
        }
        if (balls[i].pos.x < 0.5 * box.right () && balls[i].pos.y > 0.5 * box.bottom ())
        {
            balls[i].v = {56, 56};
        }
        if (balls[i].pos.x > 0.5 * box.right () && balls[i].pos.y > 0.5 * box.bottom ())
        {
            balls[i].v = {56, -56};
        }
        if (balls[i].pos.x > 0.5 * box.right () && balls[i].pos.y < 0.5 * box.bottom ())
        {
            balls[i].v = {-56, -56};
        }
    }
}
*/
bool inButtonMouse (Rect Button)
{

     if (txMouseX () > Button.left () && txMouseX () <  Button.right ())
     {
            if (txMouseY () > Button.top () && txMouseY () <  Button.bottom ())
            {
                return true;
            }
     }
     return false;
}

void cometShooting ()
{
    if (txMouseButtons () == 1)
    {
        if (txMousePos().x >  TheGS->monitorS.pos.x && 
            txMousePos().x < (TheGS->monitorS.pos.x + TheGS->monitorS.size.x) &&
            txMousePos().y >  TheGS->monitorS.pos.y &&
            txMousePos().y < (TheGS->monitorS.pos.y + TheGS->monitorS.size.y) &&
            //isActive (MainScreen) &&
            !inButtonMouse (TheGS->cleanButton) && 
            !inButtonMouse (TheGS->exitButtonHUD) && 
            !inButtonMouse (TheGS->newPlanetButton) &&
            !inButtonMouse (TheGS->screenShotButton) &&
            !inButtonMouse (TheGS->plusSpeedButton) &&
            !inButtonMouse (TheGS->minusSpeedButton)
           ) 
        {
        
            Vector finishPos = {};
            Vector speed     = returnMouseVector (&finishPos);
            /*
            Ball comet   = {};
            comet.pos    = finishPos;
            comet.v      = speed;
            comet.m      = 1e4;
            comet.charge = 2e1;
            comet.r      = 10;
            $s
            comet.color = CometColor;
            */
            Ball comet = {};
            equal (comet.pos, finishPos);
            equal (comet.v, speed);
            comet.m      = 1e4;
            comet.charge = 2e1;
            comet.r      = 10;
            $s
            comet.color = CometColor;
            
            //txSleep (0);
            if ((TheGS->ballS.currlength) + 1 < BallMax)
            {
                TheGS->ballS.addBall (comet);
                TheGS->console.print ("Комета успешно добавилась");
            
            }
            if (TheGS->ballS.currlength + 1 >= BallMax)
            {
                txMessageBox ("Невозможно добавить новую планету", "Предупреждение", MB_OK);
            }
        }

        
        //txSleep (0);
        //$$p;
    }
}

Vector returnMouseVector (Vector *finishPos)
{
    Vector startPos = {txMouseX(), txMouseY()};
    Vector currMousePos = {};
    Vector speed = {};

    for (;;)
    {
        currMousePos = {txMouseX(), txMouseY()};
        speed        = startPos - currMousePos;
        Draw (speed, currMousePos, TX_CYAN, 5);
        drawAllBall ();
        drawMiniMap ();

        txSleep (SLEEPINGTIME);

        if (txMouseButtons () != 1) break;
        ClearBackground (TheGS->HUD);
        ClearBackground (TheGS->LeftButtonsImage);
    }

    *finishPos = currMousePos;

    return speed;
}

/*
void BallSystem::addBall (Ball newBall)
{
    assert (0 <= currlength && currlength < BallMax);

    ball[currlength].pos    = newBall.pos;
    ball[currlength].v      = newBall.v;
    ball[currlength].m      = newBall.m;
    ball[currlength].charge = newBall.charge;
    ball[currlength].r      = newBall.r;
    ball[currlength].color  = newBall.color;

    currlength++;
}
*/

void drawAllBall (bool history /* = true*/)
{
    static bool drawHistory = false;
     for (int i = 0; i < TheGS->ballS.currlength; i++)
     {
         assert (0 <= i && i < BallMax);
         if (onButtonClicked (TheGS->cleanButton) && history)
         {
            drawHistory = !drawHistory;
         }
         
         /*
         if (TheGS->ballS.ball[i].pos.x + TheGS->ballS.ball[i].r < TheGS->mainPlace.right () && TheGS->ballS.ball[i].pos.x - TheGS->ballS.ball[i].r > TheGS->mainPlace.left ())
         {
            if (TheGS->ballS.ball[i].pos.y + TheGS->ballS.ball[i].r < TheGS->mainPlace.bottom () && TheGS->ballS.ball[i].pos.y - TheGS->ballS.ball[i].r > TheGS->mainPlace.top ()) 
            {

            */

         //printf ("%d", !inRect ({TheGS->ballS.ball[i].pos.x, TheGS->ballS.ball[i].pos.y}, TheGS->miniMap.sysBorderPix_));
         //!!!ERROR
         //printf ("%d",  !inRect (TheGS->ballS.ball[i].pos, TheGS->miniMap.sysBorderPix_));
         if (!inRect (TheGS->ballS.ball[i].pos, TheGS->newPlanetButton) && !inRect (TheGS->ballS.ball[i].pos, TheGS->cleanButton) && !inRect (TheGS->ballS.ball[i].pos, TheGS->exitButtonHUD))
            {
            if (TheGS->ballS.ball[i].alive)
            {
                drawBall(&TheGS->ballS.ball[i]);
                if (drawHistory)
                    TheGS->ballS.ball[i].DrawHistory ();
            }
                    
        }
        
     }
}

bool onButtons ()
{
      return    inButtonMouse (TheGS->cleanButton) ||
                inButtonMouse (TheGS->exitButtonHUD) || 
                inButtonMouse (TheGS->newPlanetButton) ||
                inButtonMouse (TheGS->screenShotButton) ||
                inButtonMouse (TheGS->plusSpeedButton) ||
                inButtonMouse (TheGS->minusSpeedButton);
}

bool onButtons (Vector pos)
{
      return    inRect (pos, TheGS->cleanButton) ||
                inRect (pos, TheGS->exitButtonHUD) || 
                inRect (pos, TheGS->newPlanetButton) ||
                inRect (pos, TheGS->screenShotButton) ||
                inRect (pos, TheGS->plusSpeedButton) ||
                inRect (pos, TheGS->minusSpeedButton);
}

bool inRect (Vector pos, Rect rectangle)
{
    if (pos.x > rectangle.left () && pos.x <  rectangle.right ())
    {
        if (pos.y > rectangle.top () && pos.y <  rectangle.bottom ())
        {
            return true;
        }
    }
    return false;

}

void PhysicsAllBall ()
{
    cometShooting ();
    for (int i = 0; i < TheGS->ballS.currlength; i++)
    {
        assert (0 <= i && i < BallMax);

        if (TheGS->ballS.ball[i].alive)
            Physics (&TheGS->ballS.ball[i],  i, true);
    }
}

void drawBall (const Ball *ball)
{
    $s
    txSetFillColor (ball->color);
    txSetColor (ball->color);

    if (ball->pos.x <= txGetExtentX() && ball->pos.y <= txGetExtentY())
    {
        txCircle ((*ball).pos.x, (*ball).pos.y, (*ball).r);
    }
}

/*
// x = 100; y = 400; r = 20; vX = 5; vY = 5; dt = 1; g = 0.7
void BallFrame (Ball *ball, double dt, double thicknessOfVector, COLORREF colorCircle, Ball balls[], int numberOfFind)
{
     txSetFillColor (colorCircle);
     txCircle ((*ball).pos.x, (*ball).pos.y, (*ball).r);
     const Rect box = { {ball->r, ball->r}, {txGetExtent().x - 2 * (ball->r), txGetExtent().y - 2 * (ball->r)} };
     Physics (ball, box, dt, balls, numberOfFind);
     ball->v.x = SpeedX (ball->v.x);
     ball->v.y = SpeedY (ball->v.y);
     SwitchColour ();
     (*ball).r = SwitchRadius ((*ball).r);
}
void BallFrameNoGrathics (Ball *ball, double dt, double thicknessOfVector, COLORREF colorCircle, Ball balls[], int numberOfFind)
{
     txSetFillColor (colorCircle);
     txCircle ((*ball).pos.x, (*ball).pos.y, (*ball).r);
     const Rect box = { {ball->r, ball->r}, {txGetExtent().x - 2 * (ball->r), txGetExtent().y - 2 * (ball->r)} };
     PhysicsNoGrathics (ball, box, dt, balls, numberOfFind);
     ball->v.x = SpeedX (ball->v.x);
     ball->v.y = SpeedY (ball->v.y);
     SwitchColour ();
     (*ball).r = SwitchRadius ((*ball).r);
}*/

Vector txToss (Vector pos)
{
    const Rect box = { {0, 0}, {double (txGetExtent().x), double (txGetExtent().y)} };

    return Vector 
    {
        pos.x += 0.5 * box.right  (),
        pos.y += 0.5 * box.bottom ()
    };
}

void ControlAllBalls ()
{
    for (int i = 0; i <= TheGS->ballS.currlength; i++)
    {
        assert (0 <= i && i < BallMax);

        if (TheGS->ballS.ball[i].alive) 
            Control (&TheGS->ballS.ball[i]);

        if (onButtonClicked (TheGS->screenShotButton))
        {
            //CreateNewPlanet (TheGS->ballS);
            makeScreenShot ();
        } 

        if (onButtonClicked (TheGS->newPlanetButton))
        {
            CreateNewPlanet (TheGS->ballS);
        }
        ControlDelta ();
        
    }
}  


void makeScreenShot ()
{
    FILE *lastScreenShotr = fopen ("GravitySystemFolder/Config.txt", "r");
    assert (lastScreenShotr);
    int currRes = 0;
    int lastScreenShot = 0;

    fscanf (lastScreenShotr, "Resolution: %d\n", &currRes);
    fscanf (lastScreenShotr, "Last Screen: %d", &lastScreenShot);
    fclose (lastScreenShotr);

    FILE *lastScreenShotw = fopen ("GravitySystemFolder/Config.txt", "w");
    assert (lastScreenShotw);

    fprintf (lastScreenShotw, "Resolution: %d\n", currRes);
    fprintf (lastScreenShotw, "Last Screen: %d", lastScreenShot + 1);
    fclose (lastScreenShotw);

    char path[100] = {};
    sprintf (path, "GravitySystemFolder/ScreenShots/ScreenShot_%d.bmp", lastScreenShot++);
    HDC currScreen =  GetWindowDC (MainScreen);
    int saveVal = txSaveImage (path, currScreen);
    assert (saveVal);

}           

void ControlDelta ()
{
    if (inButtonClicked (TheGS->plusSpeedButton))
    {
        DT += 0.001;
    } 
    if (inButtonClicked (TheGS->minusSpeedButton))
    {
        //SLEEPINGTIME++;
        DT -= 0.001;
    } 
}

//use for read mode
void ControlAllBalls (BallSystem *ballS)
{
    for (int i = 0; i < ballS->currlength; i++)
    {
        assert (0 <= i && i < BallMax);

        if (ballS->ball[i].alive) 
            SwitchColour (&ballS->ball[i]);
    }    
}

void Control (Ball *ball)
{
    ball->v.x = SpeedX (ball->v.x);
    ball->v.y = SpeedY (ball->v.y);

    SwitchColour (ball);
    (*ball).r = SwitchRadius ((*ball).r);
}

//x^2 + x = 0
//x^2 - 1 = 0 
//(x - 1)(x + 1) = 0

// 0 - x; 1 - y; 2 - vX; 3 - vY;
void Physics (Ball *ball, int numberOfFind, bool Graphic)
{
    if (Graphic)
    {
        //ball->DrawHistoryLines ();
    }
    const Rect box = { {ball->r, ball->r}, {txGetExtent().x - 2 * (ball->r), txGetExtent().y - 2 * (ball->r)} };

    Vector fGravity  = {.x = 0, .y = 0};

    Vector currPosMouse = {txMouseX (), txMouseY ()};
    //Vector mouseForce = (currPosMouse - ball->pos) * 2;
    Vector fElectric = findElectricForce (TheGS->ballS.ball, numberOfFind, TheGS->ballS.currlength);

    Vector resultantForce = fGravity + /*mouseForce*/  fElectric;

    ERROR (ball->m == 0);

    if (ball->m == 0) ball->m = 0.00001;
    

    Vector a = resultantForce / ball->m;

    
    txSetFillColor (TX_YELLOW);
    txSetColor     (TX_YELLOW);
    //txCircle (txMousePos ().x, txMousePos ().y, 10);
    /*
    Draw (resultantForce, ball->pos, TX_LIGHTRED,  5);
    Draw (mouseForce,     ball->pos, TX_LIGHTCYAN, 3);
    Draw (fGravity,       ball->pos, TX_LIGHTGRAY, 2);
    */


    ball->v += (a * DT);
    //(*ball).v.y +=  ball->a.y * dt;
    //ball->  v.x +=  ball->a.x * dt;

   // Draw (ball->v, ball->pos, TX_GREEN, 3);

    ball->pos += (ball->v * DT);
    //txDrawMan ();
    //(*ball).pos.x += (*ball).v.x * dt;
    //(*ball).pos.y += (*ball).v.y * dt;

    ball->fillHistory ();

    /*
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
    */

    FindColilision (TheGS->ballS, numberOfFind);
        
}
//
/*
void PhysicsNoGrathics (Ball *ball, double dt, Ball balls[], int numberOfFind)
{
    //ball->DrawHistory();
    Rect box = { {ball->r, ball->r}, {txGetExtent().x - 2 * (ball->r), txGetExtent().y - 2 * (ball->r)} };
    Vector fGravity  = {.x = 0, .y = 70};
    Vector currPosMouse = {txMouseX (), txMouseY ()};
    Vector mouseForce = (currPosMouse - ball->pos) * 2;
    Vector fElectric = findElectricForce (balls, numberOfFind, BallLength);
    Vector resultantForce = fGravity + mouseForce + fElectric;
    Vector a = resultantForce / ball->m;
    txSetFillColor (TX_YELLOW);
    txSetColor     (TX_YELLOW);
    txCircle (txMousePos ().x, txMousePos ().y, 10);
    Draw (resultantForce, ball->pos, TX_LIGHTRED,  0);
    Draw (mouseForce,     ball->pos, TX_LIGHTCYAN, 0);
    Draw (fGravity,       ball->pos, TX_LIGHTGRAY, 0);
    ball->v += (a * dt);
    (*ball).v.y +=  ball->a.y * dt;
    ball->  v.x +=  ball->a.x * dt;
    Draw (ball->v, ball->pos, TX_GREEN, 0);
    ball->pos += (ball->v * dt);
    (*ball).pos.x += (*ball).v.x * dt;
    (*ball).pos.y += (*ball).v.y * dt;
    ball->fillHistory ();
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
        
}//*/
//                    ball1 -> ball2

void FindColilision (BallSystem &ballS, int numberOfFind)
{
    for (int i = 0; i < ballS.currlength; i++)
    {
        assert (0 <= i && i < BallMax);

        if (i != numberOfFind)
        {
            if (ballS.ball[i].alive)
            {
                Vector distanceV = {ballS.ball[i].pos - ballS.ball[numberOfFind].pos};
                double distanceS = lengthV (distanceV);

                if (MinDistance + (ballS.ball[i].r + ballS.ball[numberOfFind].r) > distanceS)
                {
                    Colision (&ballS.ball[i], &ballS.ball[numberOfFind]);
                }
            }
        }
    }
}

void Colision (Ball *ball1, Ball *ball2)
{
    //ERROR (1);
    /*
    закон сохранения импулса
    p1 = m1 * v1
    p2 = m2 * v2
    pFinish = p1 + p2
    =>
    vFinish = pFinish / mFinish
    =>
    скорость = ((масса1 * скорость1) + (масса2 * скорость2)) / масса_итоговоого_объекта
    */
    ERROR (ball1->m + ball2->m == 0);
    Vector newV = ((ball1->v * ball1->m) + (ball2->v * ball2->m)) / (ball1->m + ball2->m);

    //ball2->color = 
    ball2->pos = ((ball1->pos * ball1->m) + (ball2->pos * ball2->m))/(ball2->m + ball1->m);


    ball2->m = ball2->m + ball1->m;
    ball1->m = 0;

    ball2->v = newV;
    ball1->v = {0, 0};
                                                                                                                                 
    double sumSquare = 4/3 * (M_PI * ball1->r * ball1->r * ball1->r) + 4/3 * (M_PI * ball2->r * ball2->r * ball2->r);
    ball2->r = cbrt (sumSquare / M_PI);
    ball1->r = 0;

    ball2->charge = ball2->charge + ball1->charge;
    ball1->charge = 0;

    ball2->color = sumColors (ball1->color, ball2->color);


    ball1->alive = !"dead";
}

COLORREF sumColors (COLORREF a, COLORREF b)
{
    int aRed   = (a & 0x0000ff);
    int aGreen = (a & 0x00ff00) >> 8;   // 
    int aBlue  = (a & 0xff0000) >> 16;      // = a & 0xff

    //00000000_00000001_00000000
    //& 0b1111111100000000 =   00_00_0xff
    //               a & 100000000
    /*
    0   0000  0    
    1   0001  1    
    2   0010  2    
    3   0011  3
    4   0100  4
    5   0101  5
    6   0110  6
    7   0111  7
    8   1000  8
    9   1001  9
    10  1010  a
    11  1011  b
    12  1100  c
    13  1101  d
    14  1110  e
    15  1111  f
    */

    /*
    int aRed   = GetRValue (a);
    int aGreen = GetGValue (a);   // 
    int aBlue  = GetBValue (a);
    int bRed   = GetRValue (b);
    int bGreen = GetGValue (b);   // 
    int bBlue  = GetBValue (b);
    */
    
    int bRed   = (b & 0x0000ff);
    int bGreen = (b & 0x00ff00) >> 8;   // 
    int bBlue  = (b & 0xff0000) >> 16;

    int finishRed   = int ((bRed   + aRed)    /2);
    int finishGreen = int ((bGreen + aGreen) / 2);      
    int finishBlue  = int ((bBlue  + aBlue) /  2);
                                   
    COLORREF finishColor = RGB (finishRed, finishGreen, finishBlue);

    return finishColor;
}

void sumColorsUnitTest ()
{
     $unittest (sumColors (
                           RGB (20, 30, 50),
                           RGB (30, 40, 60)),
                RGB (25, 35, 55));
     $unittest (sumColors (
                           RGB (0,   0,  0),
                           RGB (30, 40, 60)),
                RGB (15, 20, 30));

}

void ssCircle (Ball ball, Rect Box)
{
    //Rect Box = {{0, 0}, {double (txGetExtentX ()), double (txGetExtentY ())}} ;

    ball.pos.x += 0.5 * Box.right ()  + Box.pos.x;
    ball.pos.y += 0.5 * Box.bottom () + Box.pos.y;



    //txCircle (x + 0.5 * Box.right (), y + 0.5 * Box.bottom (), r);

    drawBall (&ball);
}

void ssLine (double StartX, double StartY, double FinishX, double FinishY)
{
    Rect Box = {{0, 0}, {double (txGetExtentX ()), double (txGetExtentY ())}} ;

    txLine (StartX + 0.5 * Box.right (), StartY + 0.5 * Box.bottom (), FinishX + 0.5 * Box.right (), FinishY + 0.5 * Box.bottom ());
}

Vector findElectricForce (Ball ball[], int numberOfFind, int length)
{
    Vector fElectric = {0, 0};
        
    for (int j = 0; j < length; j++)
    {
        assert (0 <= j && j < BallMax);

        if (j != numberOfFind)
        {
            Vector vectorDistance = ball[j].pos - ball[numberOfFind].pos;
            double distance = lengthV (vectorDistance);

            if (distance * distance < DivisionPrecision) continue;

            double vectorLength = (ball[j].charge * ball[numberOfFind].charge) / (distance * distance);
            fElectric += vectorNormal (vectorDistance) * vectorLength * ElectricKf;
            
            if (ball[j].alive)
            {
                if (ball[j].pos.x + ball[j].r < TheGS->mainPlace.right () && ball[j].pos.x - ball[j].r > TheGS->mainPlace.left ())
                {
                    if (ball[j].pos.y + ball[j].r < TheGS->mainPlace.bottom () && ball[j].pos.y - ball[j].r > TheGS->mainPlace.top ()) 
                    {
                        //Draw ((vectorNormal (vectorDistance) * vectorLength), ball[numberOfFind].pos, TX_PINK);
                    }
                }
            }
            //printf ("vectorLength: %lg\n", vectorLength);
        }
        
    }

    return fElectric;
}

Vector vectorNormal (Vector vector)
{
    return vector / lengthV (vector);
}

/*
void Ball::fillHistory ()
{
    assert (0 <= oldestNum && oldestNum < BallHistoryLength);

    history [oldestNum] = pos;
    oldestNum++;

    if (oldestNum >= (sizeof (history) / sizeof (history[0])))
    {
        oldestNum = 0;
    }
}

void Ball::DrawHistory ()
{
    const double kRgb = 225 / BallHistoryLength;
    double nColor = 0;

    for (int i = oldestNum; i < BallHistoryLength; i++)
    {
        assert (0 <= i && i < BallHistoryLength);

        txSetFillColor (RGB (nColor, nColor, nColor));
        txSetColor     (RGB (nColor, nColor, nColor));
        txCircle (history[i].x, history[i].y, 2);
        nColor += kRgb;
    }

    for (int i = 0; i < oldestNum; i++)
    {
        assert (0 <= i && i < BallHistoryLength);

        txSetFillColor (RGB (nColor, nColor, nColor));
        txSetColor     (RGB (nColor, nColor, nColor));
        txCircle (history[i].x, history[i].y, 2);
        nColor += kRgb;
    }

    txSetFillColor (TX_PINK);
    txSetColor     (TX_PINK);
    //txCircle (history[oldestNum].x, history[oldestNum].y, 3);
}

void Ball::DrawHistoryLines ()
{
    const double kRgb = 225 / BallHistoryLength;
    double nColor = 0;
    //$(oldestNum);

    for (int i = oldestNum; i < BallHistoryLength - 1; i++)
    {
        //cout << i;
        txSetFillColor (RGB (nColor, nColor, nColor));
        txSetColor     (RGB (nColor, nColor, nColor));
      // txCircle (history[i].x, history[i].y, 2);
        assert (0 <= i     && i     < BallHistoryLength);
        assert (0 <= i + 1 && i + 1 < BallHistoryLength);

        txLine   (history[i].x, history[i].y, history[i + 1].x, history[i + 1].y);
        nColor += kRgb;
    }

    if (oldestNum != 0)
    {
        assert (0 <= BallHistoryLength - 1 && BallHistoryLength - 1 < BallHistoryLength);


        txLine (history[BallHistoryLength - 1].x, history[BallHistoryLength - 1].y, history[0].x, history[0].y);
    }

    for (int i = 0; i < oldestNum - 1; i++)
    {
        assert (0 <= i && i < BallHistoryLength);
        //cout << i;
        txSetFillColor (RGB (nColor, nColor, nColor));
        txSetColor     (RGB (nColor, nColor, nColor));
        //txCircle (history[i].x, history[i].y, 2);
        txLine   (history[i].x, history[i].y, history[i + 1].x, history[i + 1].y);
        nColor += kRgb;
    }

    txSetFillColor (TX_PINK);
    txSetColor     (TX_PINK);

    assert (0 <= oldestNum && oldestNum < BallHistoryLength);
    txCircle (history[oldestNum].x, history[oldestNum].y, 3);
}
*/

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

void SwitchColour (Ball *ball)
{
    if (txGetAsyncKeyState ('B')) ball->color = TX_BLUE;
    if (txGetAsyncKeyState ('R')) ball->color = TX_RED;
    if (txGetAsyncKeyState ('G')) ball->color = TX_GREEN;
    if (txGetAsyncKeyState ('Y')) ball->color = TX_YELLOW;
    if (txGetAsyncKeyState ('P')) ball->color = TX_PINK;
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

void ClearBackground ()
{
    static bool flagClearBackground = true;

     if (txGetAsyncKeyState ('C'))
     {
         flagClearBackground = true;
     }
      /*
     if (onButtonClicked (TheGS->cleanButton))
     {
       // flagClearBackground = !flagClearBackground;
        //txSleep (0);
     }]*/

     if (txGetAsyncKeyState ('N'))
     {
         flagClearBackground = false;
     }

     COLORREF color = txGetFillColor();
     txSetFillColor (TX_BLACK);
     if (flagClearBackground == true && TheGS->HUD != NULL) 
     {
         txBitBlt (0, 0, TheGS->HUD);
         //txBitBlt (0, 0, buttons);
     }
     if (flagClearBackground == true && TheGS->HUD == NULL) txClear();
     //txClear();
     txSetFillColor (color);

}

void ClearBackground (HDC HUD, Vector startPos /*= {}*/)
{
    static bool flagClearBackground = true;

     if (txGetAsyncKeyState ('C'))
     {
         flagClearBackground = true;
     }

     if (txGetAsyncKeyState ('N'))
     {
         flagClearBackground = false;
     }

     COLORREF color = txGetFillColor();
     //txSetFillColor (TX_BLACK);
     if (flagClearBackground == true && HUD != NULL) 
     {
         //if (size.x != 0 && size.y != 0) txBitBlt (0, 0, HUD, size.x, size.y);
          txBitBlt (startPos.x, startPos.y, HUD);

     }
     if (flagClearBackground == true && HUD == NULL) txClear();
     //txClear();
     //txSetFillColor (color);

}


double degreesOfDouble (const double number, int degree)
{
    double result = number;
    printf ("\nArguements: res: %lg \t degree: %d\n", result, degree);
    for (; degree > 1;)
    {
        if (degree % 2 == 0)
        {
            result *= result;
            degree /= 2;
            printf ("print: A\t result: %lg\t degree: %i \n", result, degree);
        }
        else if (degree % 2 == 1)
        {
            result *= number;
            degree--;
            printf ("print: B\t result: %lg\t degree: %i \n", result, degree);
        }
    }

    if (degree == 0)
    {
        result = 1;
    }

    return result;
}


double elDeg (const double number, const double deg)
{
    double result = 1;
    for (int i = 0; i < deg; i++)
    {
        assert (0 <= i && i < BallMax);
        result = result * number;
    }

    return result;
}

void Draw (const Vector &vector, const Vector &startPoint, COLORREF colorOfMainVector, double thickness)
{
    $s
    if (thickness > 0)
    {
        Vector finishPoint = startPoint + vector;

        txSetColor (colorOfMainVector, thickness);
        txLine (startPoint.x, startPoint.y, finishPoint.x, finishPoint.y);
        drawArrows (vector, finishPoint);
        txSetFillColor (colorOfMainVector);
    }
}

void DrawFromStart (const Vector &vector, const Vector &finishPoint, COLORREF colorOfMainVector, double thickness)
{
    $s
    if (thickness > 0)
    {
        Vector startPoint = finishPoint - vector;

        txSetColor (colorOfMainVector, thickness);
        txLine (startPoint.x, startPoint.y, finishPoint.x, finishPoint.y);
        drawArrows (vector, finishPoint);
        txSetFillColor (colorOfMainVector);
    }
}

double lengthV (const Vector &vector)
{
    return sqrt (vector.x * vector.x + vector.y * vector.y);
}

/*
Vector operator ^ (const Vector &vector, int degree)    
{
    Vector result = {};
    for (int i = 1; i <= degree; i++)
    {
        assert (0 <= i && i < BallMax);
        result.x = vector.x * vector.x;
        result.y = vector.y * vector.y;
    }

    return result;
}
*/

void Draw_verVector (const Vector &vector, const Vector &startPoint, COLORREF /*colorOfMainVector*/, const double thickness, double rad)
{
    txBegin ();

    for (int i = 0; i < 360; i++)
    {
        assert (0 <= i && i < BallMax);
        txSetFillColor (TX_BLACK);
        txClear ();

        Draw (rotateVector (vector, rad * i), startPoint, TX_RED, thickness);

        txSleep (0);
    }

    txEnd ();
}

double DegToRad (double degrees)
{
    return (degrees * M_PI / 180);
}


Vector rotateVector (const Vector &vector, double rad)
{
    double cosinus = cos (rad);
    double sinus   = sin (rad);

    return Vector {.x = vector.x * cosinus - vector.y * sinus,
                   .y = vector.y * cosinus + vector.x * sinus };
}

void drawArrows (const Vector &mainVector, const Vector &startArrowPoint)
{
    Vector perpendicular1 = makePerpendikularLine (mainVector);
    Vector perpendicular2 = perpendicular1 * -1;

    Vector arrow1 = perpendicular1 * 0.1 - mainVector * 0.2;
    Vector arrow2 = perpendicular2 * 0.1 - mainVector * 0.2;

    Vector arrow1finishPoint = ((arrow1) + startArrowPoint);
    Vector arrow2finishPoint = ((arrow2) + startArrowPoint);

    txLine (startArrowPoint.x, startArrowPoint.y, arrow1finishPoint.x, arrow1finishPoint.y);
    txLine (startArrowPoint.x, startArrowPoint.y, arrow2finishPoint.x, arrow2finishPoint.y);
}

void drawVectorCircle (const Vector &/*vector*/, const Vector &startP, COLORREF /*color*/, double /*thickness*/)
{
    txCircle (startP.x, startP.y, 5);
}

Vector makePerpendikularLine(const Vector &mainVector)
{
    Vector perpendikularLine = {};
    perpendikularLine.x =   mainVector.y;
    perpendikularLine.y = -(mainVector.x);

    return perpendikularLine;
}

/*
inline Vector operator * (const Vector &a, const double b)
{
    Vector result = {};
    result.x = a.x * b;
    result.y = a.y * b;

    return result;
}

inline Vector operator / (const double a, const Vector &b)
{
    Vector result = {};
    result.x = a / b.x;
    result.y = a / b.y;

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

inline Vector operator / (const Vector &a, double m)
{
    return {.x = a.x / m,
            .y = a.y / m
    };
}
*/

