#pragma once

#define OFF  if (false)

#define SQR(O)  ( (O) * (O) )

//int y = SQR (5);  ==>  int y = 5 * 5  ==>  int y = 25;
//int y = SQR (3 + 2);  ==>  int y = 3 + 2 * 3 + 2  ==>  int y = 11;
//int z = SQR (y++);  ==>  int y = 5 * 5  ==>  int y = 25;