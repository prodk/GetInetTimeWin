// (c) Mykola Prodanov, January 2013, Juelich, Germany.

#if !defined(TIMEDLL)
#define TIMEDLL __declspec(dllimport)
#endif

// Required headers
#include <memory.h>
#include "winsock2.h"

#define BUF_SIZE 256

// Contains the required data
typedef struct _TimeData{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
} TimeData;

///////////////////////////////////////////////////////////////////////////////

// External function prototype
extern "C"{
	TIMEDLL int getyear();
	TIMEDLL int getmonth();
	TIMEDLL int getday();
	TIMEDLL int gethour();
	TIMEDLL int getminute();
	TIMEDLL int getsecond();
}