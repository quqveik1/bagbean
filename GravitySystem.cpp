
#define  _CRT_SECURE_NO_WARNINGS

#include "TXLib.h"
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

const int BallHistoryLength = 200;
int BallLength = 3;
const double Precision  = 1e-100;
const double ElectricKf = 6e3;
const double DT = 0.09;
const double MinDistance = 5;
int LastComet = 0;
const int BallMax = 10;

struct Vector 
{
    double x;
    double y;
};
struct Rect
{
    Vector pos;
    Vector size;

    double left  () const { return this->pos.x; }
    double top   () const { return this->pos.y; }
    double right () const { return this->size.x + this->pos.x; }
    double bottom() const { return this->size.y + this->pos.y; }
};

struct Ball 
{
    Vector pos;
    Vector v;
    double m;
    double r;
    double charge;
    COLORREF color;
    bool alive = 1;

    Vector history [BallHistoryLength];

    int oldestNum;
    void fillHistory ();

    void DrawHistory ();
    void DrawHistoryLines ();
};

struct BallSystem
{
    Ball ball[BallMax];

    int currlength = 0;

    void addBall (Ball newBall);
};

inline Vector  operator +  (const Vector &a, const Vector &b);
inline Vector &operator += (Vector &a, const Vector &b);
inline Vector  operator -  (const Vector &a, const Vector &b);
inline Vector  operator *  (const Vector &a, const double b);
inline Vector  operator *  (const Vector &a, const Vector &b);
inline Vector &operator *= (Vector &a, const Vector &b);
inline Vector  operator /  (const Vector &a, double m);
inline Vector operator / (const double a, const Vector &b);
Vector operator ^ (const Vector &vector, int degree);

void RunEngineE2 (const Ball planetsInit[]);

void RunEngineExperiment (BallSystem &ball);
void Draw (const Vector &vector, const Vector &startPoint, COLORREF colorOfMainVector, double thickness = 1);
void drawArrows (const Vector &mainVector, const  Vector &startArrowPoint);
Vector makePerpendikularLine (const Vector &mainVector);
void drawVectorCircle (const Vector &vector, const Vector &startP, COLORREF color, double thickness = 1);
Vector rotateVector (const Vector &vector, double rad);
void Draw_verVector (const Vector &vector, const Vector &startPoint, COLORREF colorOfMainVector, const double thickness, double angle);
double DegToRad (double degrees);
double lengthV (const Vector &vector);
Vector vectorNormal (Vector vector);


double elDeg (const double number, const double deg);
double degreesOfDouble (const double number, int degree);

void drawAllBall (BallSystem &ballS);
void PhysicsAllBall (BallSystem &ball);
void drawBall (Ball *ball, COLORREF colorCircle);
void Control (Ball *ball);
void Physics (Ball *ball, BallSystem &ballS, int numberOfFind, bool Graphic);
void PhysicsNoGrathics (Ball *ball, Ball balls[], int numberOfFind);
void cometShooting (BallSystem &ballS);
//void addBall (Ball ball[], int *lastBall, Ball newBall);
Vector findElectricForce (Ball ball[], int numberOfFind, int length);
void Colision (Ball *ball1, Ball *ball2);
void FindColilision (BallSystem &ballS, int numberOfFind);
COLORREF sumColors (COLORREF a, COLORREF b);
void sumColorsUnitTest ();
void ControlAllBalls (BallSystem &ballS);
void ssCircle (double x, double y, double r);
void ssLine (double StartX, double StartY, double FinishX, double FinishY);
//void solarSystem (Ball balls[]);
Vector txToss (Vector pos);
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
    /*
    Vector start =  {.x = 400, .y = 300};
    Vector vector = {.x = 0,   .y = -50};
    Draw_verVector (vector, start, TX_RED, 5, DegToRad (1));
    */
    
    txCreateWindow (1900, 1000);
    txSetColor     (TX_LIGHTRED);
    txSetFillColor (TX_RED);

    //sumColorsUnitTest();

    //(void) (_getch ());

    BallSystem ballS = {};
    Ball planetsInit[BallMax] = {};
    
    /*
    ball[0] = {txToss ({0, 0}),   {0, 0}, 1e17, 10, 2e5, TX_RED};
    ball[1] = {txToss ({0,    -400}),  {7, 7}, 1e4, 10, 2, TX_RED};
    ball[2] = {txToss ({0,    +400}), {18, 17}, 1e4, 10, 5, TX_RED};
    ball[3] = {txToss ({400,  -400}), {7, 7}, 1e4, 10, 2, TX_RED};
    */

    ballS.ball[0] = {txToss ({0, 0}),   {0, 0}, 1e17, 10, 2e5, TX_YELLOW};
    ballS.ball[1] = {txToss ({0, -400}),  {24, 0}, 1e4, 10, 2, TX_CYAN};
    ballS.ball[2] = {txToss ({0, -200}),  {32, 0}, 1e4, 10, 2, TX_RED};
    ballS.currlength = 3;

    /*
    ballS.addBall ({txToss ({0, 0}),   {0, 0}, 1e17, 10, 2e5, TX_RED});
    ballS.addBall ({txToss ({0, -400}),  {24, 0}, 1e4, 10, 2, TX_RED});
    ballS.addBall ({txToss ({0, -200}),  {10, 0}, 1e4, 10, 2, TX_RED}};
    */

    for (int i = ballS.currlength; i < BallMax; i++)
    {
        ballS.ball[i] = {txToss ({0, 0}),   {0, 0}, 0, 0, 0, TX_RED};
    }

    //ball[3] = {txToss ({0, -100}),  {10, 0}, 1e4, 10, 2, TX_RED};

    for (int i = 0; i < BallMax; i++) planetsInit[i] = ballS.ball[i];

    txBegin ();
    for (;;)
    {
        for (int i = 0; i < ballS.currlength; i++) ballS.ball[i] = planetsInit[i];

        //txEnd ();
        ///printf ("Vx = ");
        //if (scanf  ("%lg", &ballS.ball[2].v.x) != 1) break;

        //if (ballS.ball[2].v.x == -1 && ballS.ball[2].v.y  == -1) break;
        //txBegin ();
        
        
        RunEngineExperiment (ballS);
    }

    txEnd ();

    return 0;
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

void RunEngineExperiment (BallSystem &ballS)
{
    bool flagClearBackground = true;
        
    for (;;)
    {
        //solarSystem (ball);
        PhysicsAllBall (ballS);
        //solarSystem (ball);
        PhysicsAllBall (ballS);
        //solarSystem (ball);
        PhysicsAllBall (ballS);

        

        ControlAllBalls (ballS);

        drawAllBall (ballS);

        txSleep (20);

        if (txGetAsyncKeyState('Q')) break;

        flagClearBackground = ClearBackground (flagClearBackground);
    }

        
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

void cometShooting (BallSystem &ballS)
{
    if (txMouseButtons () == 1)
    {
        Vector startPos = {txMouseX(), txMouseY()};

        printf ("Вектор\n");

        txSleep (5e3);

        if (txMouseButtons () == 2)
        {
            Vector finishPos = {txMouseX(), txMouseY()};

            Vector speed = startPos - finishPos;

            /*
            
            */

            Ball comet   = {};
            comet.pos    = finishPos;
            comet.v      = speed;
            comet.m      = 1e4;
            comet.charge = 2e1;
            comet.r      = 10;
            
            ballS.addBall (comet);
        }
    }
}

void BallSystem::addBall (Ball newBall)
{
    ball[currlength].pos    = newBall.pos;
    ball[currlength].v      = newBall.v;
    ball[currlength].m      = newBall.m;
    ball[currlength].charge = newBall.charge;
    ball[currlength].r      = newBall.r;
    ball[currlength].color  = newBall.color;

    currlength++;
}

void drawAllBall (BallSystem &ballS)
{
     for (int i = 0; i < ballS.currlength; i++)
     {

         if (ballS.ball[i].alive)
            drawBall(&ballS.ball[i], ballS.ball[i].color);
     }
}

void PhysicsAllBall (BallSystem &ballS)
{
    cometShooting (ballS);
    for (int i = 0; i < ballS.currlength; i++)
    {
        if (ballS.ball[i].alive)
            Physics (&ballS.ball[i], ballS, i, true);
    }
}

void drawBall (Ball *ball, COLORREF colorCircle)
{
    txSetFillColor (colorCircle);

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
    const Rect box = { {0, 0}, {txGetExtent().x, txGetExtent().y} };

    return Vector 
    {
        pos.x += 0.5 * box.right  (),
        pos.y += 0.5 * box.bottom ()
    };
}

void ControlAllBalls (BallSystem &ballS)
{
    for (int i = 0; i < ballS.currlength; i++)
    {
        if (ballS.ball[i].alive) 
            Control (&ballS.ball[i]);
    }
}

void Control (Ball *ball)
{
    ball->v.x = SpeedX (ball->v.x);
    ball->v.y = SpeedY (ball->v.y);

    SwitchColour ();
    (*ball).r = SwitchRadius ((*ball).r);
}

//x^2 + x = 0
//x^2 - 1 = 0 
//(x - 1)(x + 1) = 0

// 0 - x; 1 - y; 2 - vX; 3 - vY;
void Physics (Ball *ball, BallSystem &ballS, int numberOfFind, bool Graphic)
{
    if (Graphic)
    {
        ball->DrawHistoryLines ();
    }
    const Rect box = { {ball->r, ball->r}, {txGetExtent().x - 2 * (ball->r), txGetExtent().y - 2 * (ball->r)} };

    Vector fGravity  = {.x = 0, .y = 0};

    Vector currPosMouse = {txMouseX (), txMouseY ()};
    //Vector mouseForce = (currPosMouse - ball->pos) * 2;
    Vector fElectric = findElectricForce (ballS.ball, numberOfFind, ballS.currlength);

    Vector resultantForce = fGravity + /*mouseForce*/  fElectric;

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

    FindColilision (ballS, numberOfFind);
        
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
    //(*ball).v.y +=  ball->a.y * dt;
    //ball->  v.x +=  ball->a.x * dt;
    Draw (ball->v, ball->pos, TX_GREEN, 0);
    ball->pos += (ball->v * dt);
    //(*ball).pos.x += (*ball).v.x * dt;
    //(*ball).pos.y += (*ball).v.y * dt;
    //ball->fillHistory ();
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
        if (i != numberOfFind)
        {
            if (ballS.ball[i].alive)
            {
                Vector distanceV = ballS.ball[i].pos - ballS.ball[numberOfFind].pos;
                double distanceS = lengthV (distanceV);

                if (MinDistance > distanceS)
                {
                    Colision (&ballS.ball[i], &ballS.ball[numberOfFind]);
                }
            }
        }
    }
}

void Colision (Ball *ball1, Ball *ball2)
{
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
    Vector newV = ((ball1->v * ball1->m) + (ball2->v * ball2->m)) / (ball1->m + ball2->m);

    //ball2->color = 

    ball2->m = ball2->m + ball1->m;
    ball1->m = 0;

    ball2->v = newV;
    ball1->v = {0, 0};

    double sumSquare = M_PI * ball1->r + M_PI * ball1->r;
    ball2->r = sumSquare / M_PI;
    ball1->r = 0;

    ball2->charge = ball2->charge + ball1->charge;
    ball1->charge = 0;

    ball2->color = sumColors (ball1->color, ball2->color);

    ball1->alive = false;
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

void ssCircle (double x, double y, double r)
{
    Rect Box = {{0, 0}, {txGetExtentX (), txGetExtentY ()}} ;

    txCircle (x + 0.5 * Box.right (), y + 0.5 * Box.bottom (), r);
}

void ssLine (double StartX, double StartY, double FinishX, double FinishY)
{
    Rect Box = {{0, 0}, {txGetExtentX (), txGetExtentY ()}} ;

    txLine (StartX + 0.5 * Box.right (), StartY + 0.5 * Box.bottom (), FinishX + 0.5 * Box.right (), FinishY + 0.5 * Box.bottom ());
}

Vector findElectricForce (Ball ball[], int numberOfFind, int length)
{
    Vector fElectric = {0, 0};
        
    for (int j = 0; j < length; j++)
    {
        if (j != numberOfFind)
        {
            Vector vectorDistance = ball[j].pos - ball[numberOfFind].pos;
            double distance = lengthV (vectorDistance);

            if (distance * distance < Precision) continue;

            double vectorLength = (ball[j].charge * ball[numberOfFind].charge) / (distance * distance);
            fElectric += vectorNormal (vectorDistance) * vectorLength * ElectricKf;
            
            if (ball[j].alive)
            {
                Draw ((vectorNormal (vectorDistance) * vectorLength), ball[numberOfFind].pos, TX_PINK);
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

void Ball::fillHistory ()
{
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
        txSetFillColor (RGB (nColor, nColor, nColor));
        txSetColor     (RGB (nColor, nColor, nColor));
        txCircle (history[i].x, history[i].y, 2);
        nColor += kRgb;
    }

    for (int i = 0; i < oldestNum; i++)
    {
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
        txLine   (history[i].x, history[i].y, history[i + 1].x, history[i + 1].y);
        nColor += kRgb;
    }

    if (oldestNum != 0)
    {
        txLine (history[BallHistoryLength - 1].x, history[BallHistoryLength - 1].y, history[0].x, history[0].y);
    }

    for (int i = 0; i < oldestNum - 1; i++)
    {
        //cout << i;
        txSetFillColor (RGB (nColor, nColor, nColor));
        txSetColor     (RGB (nColor, nColor, nColor));
        //txCircle (history[i].x, history[i].y, 2);
        txLine   (history[i].x, history[i].y, history[i + 1].x, history[i + 1].y);
        nColor += kRgb;
    }

    txSetFillColor (TX_PINK);
    txSetColor     (TX_PINK);
    txCircle (history[oldestNum].x, history[oldestNum].y, 3);
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
        result = result * number;
    }

    return result;
}

void Draw (const Vector &vector, const Vector &startPoint, COLORREF colorOfMainVector, double thickness)
{
    if (thickness > 0)
    {
        Vector finishPoint = startPoint + vector;

        txSetColor (colorOfMainVector, thickness);
        txLine (startPoint.x, startPoint.y, finishPoint.x, finishPoint.y);
        drawArrows (vector, finishPoint);
        txSetFillColor (colorOfMainVector);
    }

    /*
    txCircle (startPoint.x, startPoint.y, thickness * 0.5 + 0.05 * length (vector));
    txCircle (startX,  startY, 7);
    COLORREF color = txGetFillColor();
    txSetFillColor (TX_BLUE);
    txCircle (finishX, finishY, 5);
    txSetFillColor (color);
    txLine(vector.x, vector.y, vector.x+);
    */
}

double lengthV (const Vector &vector)
{
    return sqrt (vector.x * vector.x + vector.y * vector.y);
}

Vector operator ^ (const Vector &vector, int degree)    
{
    Vector result = {};
    for (int i = 1; i <= degree; i++)
    {
        result.x = vector.x * vector.x;
        result.y = vector.y * vector.y;
    }

    return result;
}

void Draw_verVector (const Vector &vector, const Vector &startPoint, COLORREF /*colorOfMainVector*/, const double thickness, double rad)
{
    txBegin ();

    for (int i = 0; i < 360; i++)
    {
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
    Vector perpendicular1 = makePerpendikularLine(mainVector);
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