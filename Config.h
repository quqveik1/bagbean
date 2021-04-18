#pragma once

#define OFF  if (false)

#define SQR(O)  ( (O) * (O) )

const int BallHistoryLength = 20;
const int BallMax = 100;
int BallLength = 3;
const double DivisionPrecision  = 1e-100;
const double ElectricKf = 6e3;
const double DT = 0.09;
const double MinDistance = 0;
int LastComet = 0;

const int RoundingPrecision = 7;
const double MaxDeltaPrecision = 0.1;
const COLORREF CometColor = TX_PINK;
int SLEEPINGTIME = 30;
const float Scale = 3;
Vector MonitorSize = {1800, 1000};
int Resolution = 0;
HWND MainScreen;

//int y = SQR (5);  ==>  int y = 5 * 5  ==>  int y = 25;
//int y = SQR (3 + 2);  ==>  int y = 3 + 2 * 3 + 2  ==>  int y = 11;
//int z = SQR (y++);  ==>  int y = 5 * 5  ==>  int y = 25;