#pragma once

class WinLogger
{
	void* Buffer;
	const char* sourceName;

public:
	WinLogger(const char* sourceName);
	~WinLogger();
	void LogFormat(int type, const char* format, va_list args) throw();
	void Log(int type, const char* szMsg) throw();

	virtual void Error(const char* format) throw();
	virtual void Warn(const char* format) throw();
	virtual void Info(const char* format) throw();
	virtual void Success(const char* format) throw();

	virtual void ErrorFormat(const char* format, ...) throw();
	virtual void WarnFormat(const char* format, ...) throw();
	virtual void InfoFormat(const char* format, ...) throw();
	virtual void SuccessFormat(const char* format, ...) throw();

};

