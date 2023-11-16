#pragma once

#include "TXLib.h"
#include "Q_Vector.h"
#include "Config.h"
#include "Q_Ball.h"


class Console 
{
public: 
	Vector scalePix_;
	Vector startPosPix_;

	Console	(Vector startPosPix, Vector scalePix);
	void print (const char* str);
	void clear ();
	void reDraw ();

private:
	void privatePrint (const int line);
	void privatePrint (const int line, const char* str);

	static const int nLines_ = 17;

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

	if (++lastLine_ >= nLines_) lastLine_ = 0; 
	
}

void Console::clear ()
{
	lastLine_ = 0;
}

void Console::reDraw ()
{
	int printL = 0;
	for (int i = lastLine_; i < nLines_; i++)
	{
		privatePrint (printL++, consoleText_[i].c_str());				
	}

	for (int i = 0; i < lastLine_; i++)
	{
		privatePrint (printL++, consoleText_[i].c_str());
	}
}

void Console::privatePrint (const int line)
{
	txTextOut (startPosPix_.x, startPosPix_.y + (letterSizeY_ * line), consoleText_[line].c_str());	
}

void Console::privatePrint (const int line, const char* str)
{
	txTextOut (startPosPix_.x, startPosPix_.y + (letterSizeY_ * line), str);
}
