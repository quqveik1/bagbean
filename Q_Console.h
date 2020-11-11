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

	Console	(Vector startPosPix, Vector scalePix);
	void print (const char* str);
	void clear ();
	void reDraw ();



private:
	void privatePrint (const int line);

	static const int nLines_ = 12;

	string consoleText_[nLines_]; 
	int lastLine_;

	const int letterSizeY_;
};

Console::Console (Vector startPosPix, Vector scalePix) :
    startPosPix_ (startPosPix), 
    scalePix_ (scalePix),
	lastLine_ (0),
	letterSizeY_ (txGetTextExtent("W").cy)
{
}

void Console::print (const char* str)
{
	consoleText_[lastLine_]	= str;
	if (str == "Комета успешно добавилась") DebugBreak();
						   
	privatePrint (lastLine_);

	if (++lastLine_ >= nLines_ - 1) lastLine_ = 0; 
	
}


void Console::clear ()
{
	lastLine_ = 0;
}

void Console::reDraw ()
{
	
	for (int i = 0; i < lastLine_; i++)
	{
		privatePrint (i);
	}
}

void Console::privatePrint (const int line)
{

	txTextOut (startPosPix_.x, startPosPix_.y + (letterSizeY_ * line), consoleText_[line].c_str());			
}