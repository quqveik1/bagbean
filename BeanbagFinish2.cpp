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

const int BallHistoryLength = 100;
const int BallLength = 2;
const double Precision = 1e-100;
const double ElectricKf = 800;
const double DT = 0.09;
const double MinDistance = 5;

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

inline Vector  operator +  (const Vector &a, const Vector &b);
inline Vector &operator += (Vector &a, const Vector &b);
inline Vector  operator -  (const Vector &a, const Vector &b);
inline Vector  operator *  (const Vector &a, const double b);
inline Vector  operator *  (const Vector &a, const Vector &b);
inline Vector &operator *= (Vector &a, const Vector &b);
inline Vector  operator /  (const Vector &a, double m);
inline Vector operator / (const double a, const Vector &b);
Vector operator ^ (const Vector &vector, int degree);

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

void drawAllBall (Ball ball[]);
void PhysicsAllBall (Ball ball[]);
void drawBall (Ball *ball, COLORREF colorCircle);
void Control (Ball *ball);
void Physics (Ball *ball, Ball balls[], int numberOfFind, bool Graphic);
void PhysicsNoGrathics (Ball *ball, Ball balls[], int numberOfFind);
Vector findElectricForce (Ball ball[], int numberOfFind, int length);
void Colision (Ball *ball1, Ball *ball2);
void FindColilision (Ball balls[], int numberOfFind);
void ControlAllBalls (Ball balls[]);
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
    
    txCreateWindow (800, 600);
    txSetColor     (TX_LIGHTRED);
    txSetFillColor (TX_RED);

    

    Ball ball[BallLength] = {};
    

    ball[0] = {{400, 300}, {0, 0}, 100000, 10, 2, TX_RED};
    ball[1] = {{600, 300}, {56, 56}, 100, 10, 2, TX_RED};

    bool flagClearBackground = true;

    txBegin ();
    for (;;)
    {
        PhysicsAllBall (ball);
        PhysicsAllBall (ball);
        PhysicsAllBall (ball);

        ControlAllBalls (ball);

        drawAllBall (ball);

        txSleep (20);

        if (txGetAsyncKeyState('Q')) break;

        flagClearBackground = ClearBackground (flagClearBackground);
    }
    txEnd ();

    return 0;
}

void drawAllBall (Ball ball[])
{
     for (int i = 0; i < BallLength; i++)
     {
         if (ball[i].alive)
            drawBall(&ball[i], ball->color);
     }
}

void PhysicsAllBall (Ball ball[])
{
     for (int i = 0; i < BallLength; i++)
     {
         if (ball[i].alive)
             Physics (&ball[i], ball, i, false);
     }
}

void drawBall (Ball *ball, COLORREF colorCircle)
{
    txSetFillColor (colorCircle);
    txCircle ((*ball).pos.x, (*ball).pos.y, (*ball).r);
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

void ControlAllBalls (Ball balls[])
{
    for (int i = 0; i < BallLength; i++)
    {
        if (balls[i].alive) 
            Control (&balls[i]);
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
void Physics (Ball *ball, Ball balls[], int numberOfFind, bool Graphic)
{
    if (Graphic)
    {
        ball->DrawHistoryLines ();
    }
    const Rect box = { {ball->r, ball->r}, {txGetExtent().x - 2 * (ball->r), txGetExtent().y - 2 * (ball->r)} };

    Vector fGravity  = {.x = 0, .y = 0};

    Vector currPosMouse = {txMouseX (), txMouseY ()};
    //Vector mouseForce = (currPosMouse - ball->pos) * 2;
    Vector fElectric = findElectricForce (balls, numberOfFind, BallLength);

    Vector resultantForce = fGravity + /*mouseForce*/  fElectric;

    Vector a = resultantForce / ball->m;

    
    txSetFillColor (TX_YELLOW);
    txSetColor     (TX_YELLOW);
    txCircle (txMousePos ().x, txMousePos ().y, 10);
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

    FindColilision (balls, numberOfFind);
        
}//
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

void FindColilision (Ball balls[], int numberOfFind)
{
    for (int i = 0; i < BallLength; i++)
    {
        if (i != numberOfFind)
        {
            if (balls[i].alive)
            {
                Vector distanceV = balls[i].pos - balls[numberOfFind].pos;
                double distanceS = lengthV (distanceV);

                if (MinDistance > distanceS)
                {
                    Colision (&balls[i], &balls[numberOfFind]);
                }
            }
        }
    }
}

void Colision (Ball *ball1, Ball *ball2)
{
    ball2->m = ball2->m + ball1->m;
    ball1->m = 0;

    double sumSquare = M_PI * ball1->r + M_PI * ball1->r;
    ball2->r = sumSquare / M_PI;
    ball1->r = 0;

    ball2->charge = ball2->charge + ball1->charge;
    ball1->charge = 0;

    ball1->alive = false;
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
                Draw ((vectorNormal (vectorDistance) * vectorLength) * 100000, ball[numberOfFind].pos, TX_PINK);
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