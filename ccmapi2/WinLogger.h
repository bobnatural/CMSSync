#pragma once

class WinLogger
{
	void* Buffer;
	const char* sourceName;

public:
	WinLogger(const char* sourceName);
	~WinLogger();
	void LogFormat(int type, DWORD eventID, const char* format, va_list args) throw();
	void LogFormat(int type, const char* format, va_list args) throw();
	void Log(int type, DWORD eventID, const char* szMsg) throw();

	virtual void Error(const char* format, DWORD eventID) throw();
	virtual void Warn(const char* format) throw();
	virtual void Info(const char* format) throw();
	virtual void Success(const char* format) throw();

	virtual void ErrorFormat(const char* format, ...) throw();
	virtual void WarnFormat(const char* format, ...) throw();
	virtual void InfoFormat(const char* format, ...) throw();
	virtual void SuccessFormat(const char* format, ...) throw();

	void WarnFormat(DWORD eventID, const char* format, ...) throw();
	void ErrorFormat(DWORD eventID, const char* format, ...) throw();
};

