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


	//if (str == "Комета успешно добавилась") DebugBreak();
						   
	//privatePrint (lastLine_);

	if (++lastLine_ >= nLines_) lastLine_ = 0; 
	
}




void Console::clear ()
{
	lastLine_ = 0;
}

void Console::reDraw ()
{

	int printL = 0;
	//char stri[100] = "";

	//txSetColor (TX_CYAN);
	for (int i = lastLine_; i < nLines_; i++)
	{
		//sprintf	(stri, "[%02d] [%s]", i, consoleText_[i].c_str());
	
		privatePrint (printL++, consoleText_[i].c_str());
		//printf ("-+020.6%f") |+000000000003.140000|
		//						12345678901234567890					
	}

	//txSetColor (TX_RED);
	for (int i = 0; i < lastLine_; i++)
	{
		//sprintf	(stri, "[%02d] [%s]", i, consoleText_[i].c_str());
	
		privatePrint (printL++, consoleText_[i].c_str());
	}
	//printf ("--------------------------------\n");
}

void Console::privatePrint (const int line)
{

	txTextOut (startPosPix_.x, startPosPix_.y + (letterSizeY_ * line), consoleText_[line].c_str());	
	//printf ("%s\n", consoleText_[line].c_str ());
}

void Console::privatePrint (const int line, const char* str)
{
	txTextOut (startPosPix_.x, startPosPix_.y + (letterSizeY_ * line), str);
}