#pragma once

#include "TXLib.h"
#include "Q_Vector.h"
#include "Config.h"
#include "Q_Ball.h"


class Console 
{
public: 
	const Vector scalePix_;
	const Vector startPosPix_;

	Console	(Vector startPosPix, Vector scalePix, const *char font == NULL);
	void print(const *char str);



private:
	int lastLine_;

	

};

Console::Console (Vector startPosPix, Vector scalePix, const *char font) :
    startPosPix_ (startPosPix), 
    scalePix_ (scalePix),
	lastLine_ (0),
{
}

void Console::print(const *char str)
{
	txTextOut (startPosPix_.x, startPosPix_.y + (txGetTextExtent(str).y * lastLine_), str);
	lastLine_++;
}