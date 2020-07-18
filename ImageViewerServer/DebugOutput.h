#pragma once

#include "StringFormatter.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace lwhttp {

	namespace Tools {

		inline void TraceInfo(const char *log)
		{
			::OutputDebugStringA(log);
		}

		inline void TraceInfo(const char *log, const char *end)
		{
			::OutputDebugStringA(log);
			::OutputDebugStringA(end);
		}

		inline void TraceInfo(const wchar_t *log)
		{
			::OutputDebugStringW(log);
		}

		inline void TraceInfo(const wchar_t *log, const wchar_t *end)
		{
			::OutputDebugStringW(log);
			::OutputDebugStringW(end);
		}

		inline void CheckWinAPIHandle(const wchar_t *handlename, HANDLE handle, const wchar_t *func, long long line)
		{
			DWORD err = GetLastError();
			lwhttp::Tools::StringFormatter<wchar_t> f;
			const wchar_t *text;

			if (handle == INVALID_HANDLE_VALUE) {
				text = f.join({ handlename , L" == INVALID_HANDLE_VALUE(" , f.conv(err), L") ", func , L" ", f.conv(line), L"\n" });
			}
			else {
				text = f.join({ handlename , L" != INVALID_HANDLE_VALUE " , func , L" ", f.conv(line), L"\n" });
			}
			TraceInfo(text);
		}

		inline void DebugOutputVar(const wchar_t *varname, bool var, const wchar_t *func, long long line)
		{
			lwhttp::Tools::StringFormatter<wchar_t> f;
			TraceInfo(f.join({ L"Value(", varname , L" = " , (var ? L"true " : L"false) "), func , L" ", f.conv(line), L"\n" }));
		}

		inline void DebugOutputVar(const wchar_t *varname, wchar_t *var, const wchar_t *func, long long line)
		{
			lwhttp::Tools::StringFormatter<wchar_t> f;
			TraceInfo(f.join({ L"Value(", varname , L" = \"" , var, L"\" ", func , L" ", f.conv(line), L"\n" }));
		}

		inline void DebugOutputVar(const wchar_t *varname, long long var, const wchar_t *func, long long line)
		{
			lwhttp::Tools::StringFormatter<wchar_t> f;
			TraceInfo(f.join({ L"Value(", varname , L" = " , f.conv(var) , L") ", func , L" ", f.conv(line), L"\n" }));
		}

		template<typename T>
		inline void DebugOutputVar(const wchar_t *varname, T var, const wchar_t *func, long long line)
		{
			DebugOutputVar(varname, static_cast<long long>(var), func, line);
		}


#define CHECKWINAPIHANDLE(handle)  lwhttp::Tools::CheckWinAPIHandle(L ## #handle, handle, __FUNCTIONW__ , __LINE__)
#define DEBUGOUTPUTVAR(var)  lwhttp::Tools::DebugOutputVar(L ## #var, var, __FUNCTIONW__ , __LINE__)

		class DebugOutput
		{
		private:
			HANDLE m_logfile;

			DebugOutput() {
				m_logfile = ::CreateFileW(L"d:\\web\\log.txt", FILE_APPEND_DATA,
					FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			}
			int writeStr(const char *log) const;
			int writeStr(const wchar_t *wlog) const;

		public:
			~DebugOutput() {
				CloseHandle(m_logfile);
			}
			static const DebugOutput &get() {
				static DebugOutput obj;
				return obj;
			}

			int writeLog(const char *log, const char *endline = "\n") const;
			int writeLog(const wchar_t *wlog, const wchar_t *wendline = L"\n") const;

		};

		inline int WriteLog(const wchar_t *wlog, const wchar_t *wendline = L"\n")
		{
			return DebugOutput::get().writeLog(wlog, wendline);
		}

		inline int WriteLog(const char *log, const char *endline = "\n")
		{
			return DebugOutput::get().writeLog(log, endline);
		}

	}

}
