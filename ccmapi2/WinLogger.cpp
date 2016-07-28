#include "stdafx.h"
#include "stdio.h"
#include "WinBase.h"
#include "WinLogger.h"


WinLogger::WinLogger(const char* sourceName)
{
	this->sourceName = sourceName;
	Buffer = VirtualAlloc(0, 1024 * 64, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}


WinLogger::~WinLogger()
{
	try{
		if (Buffer)
			VirtualFree(Buffer, 0, MEM_RELEASE);
	}
	catch (...)	{}
	Buffer = NULL;
}

void WinLogger::LogFormat(int type, const char* format, va_list args) throw()
{
	LogFormat(type, 1100, format, args);
}

void WinLogger::LogFormat(int type, DWORD eventID, const char* format, va_list args) throw(){
	//	if (hLogFile == NULL) return;
	__try{
		__try{
			//			EnterCriticalSection(&cs);
			char* szBuffer = (char*)Buffer;
			if (szBuffer)
			{
				//Format and print message
				_vsnprintf_s(szBuffer, (1024 * 64) / sizeof(char) - sizeof(char), _TRUNCATE, format, args);				
				Log(type, eventID, szBuffer);
			}
		}
		__finally{
			//			LeaveCriticalSection(&cs);
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
	}
}

void WinLogger::Log(int type, DWORD eventID, const char* szMsg) throw()
{
	//char szTime[1024];
	//SYSTEMTIME time = { 0 };
	//::GetLocalTime(&time);

	//long id = ::GetCurrentThreadId();

	//sprintf_s(szTime, 1024, "\r\n[0x%.4X] %.2d:%.2d:%.2d.%.3d; ", id, time.wHour, time.wMinute,
	//	time.wSecond, time.wMilliseconds);

	HANDLE eventLog = OpenEventLog(NULL, this->sourceName);
	ReportEvent(eventLog, (WORD)type, 0, eventID, NULL, 1, 0, &szMsg, NULL);

	//::WriteToLog_Handle(hLogFile, szTime);
	//::WriteToLog_Handle(hLogFile, szMsg);
}

void WinLogger::ErrorFormat(const char* format, ...) throw()
{
	va_list args;
	va_start(args, format);
	LogFormat(EVENTLOG_ERROR_TYPE, format, args);
	va_end(args);
}
void WinLogger::WarnFormat(const char* format, ...) throw()
{
	va_list args;
	va_start(args, format);
	LogFormat(EVENTLOG_WARNING_TYPE, format, args);
	va_end(args);
}

void WinLogger::InfoFormat(const char* format, ...) throw()
{
	va_list args;
	va_start(args, format);
	LogFormat(EVENTLOG_INFORMATION_TYPE, format, args);
	va_end(args);
}

void WinLogger::SuccessFormat(const char* format, ...) throw()
{
	va_list args;
	va_start(args, format);
	LogFormat(EVENTLOG_SUCCESS, format, args);
	va_end(args);
}


// Log4Net style:
void WinLogger::Error(const char* msg, DWORD eventID = 14) throw()
{
	Log(EVENTLOG_ERROR_TYPE, eventID, msg);
}
void WinLogger::Warn(const char* msg) throw()
{
	Log(EVENTLOG_WARNING_TYPE, 149, msg);
}
void WinLogger::Info(const char* msg) throw()
{
	Log(EVENTLOG_INFORMATION_TYPE, 1499, msg);
}
void WinLogger::Success(const char* msg) throw()
{
	Log(EVENTLOG_SUCCESS, 1501, msg);
}

void WinLogger::WarnFormat(DWORD eventID, const char* format, ...) throw()
{
	va_list args;
	va_start(args, format);
	LogFormat(EVENTLOG_WARNING_TYPE, format, args);
	va_end(args);
}
void WinLogger::ErrorFormat(DWORD eventID, const char* format, ...) throw()
{
	va_list args;
	va_start(args, format);
	LogFormat(EVENTLOG_ERROR_TYPE, format, args);
	va_end(args);
}
