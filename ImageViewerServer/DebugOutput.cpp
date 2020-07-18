#include "DebugOutput.h"
#include "tools.h"

namespace lwhttp {

	namespace Tools {

		int DebugOutput::writeStr(const char* log) const
		{
			DWORD cnt = 0;
			long long lsize = strlen(log);
			if (INVALID_HANDLE_VALUE == m_logfile || lsize == 0) {
				return 0;
			}
			WriteFile(m_logfile, log, CHECK_SIZE_MAXUINT32(lsize), &cnt, NULL);
			return cnt;
		}

		int DebugOutput::writeStr(const wchar_t* wlog) const
		{
			std::vector<char> log;

			return writeStr(Tools::convUNICODE2MBCS(&log, wlog));
		}

		int DebugOutput::writeLog(const char* log, const char* endline) const
		{
			::SYSTEMTIME st;
			::GetLocalTime(&st);
			const int bufsize = 1024;
			std::vector<char> buf(bufsize);
			snprintf(buf.data(), bufsize, "%04d-%02d-%02d %02d:%02d:%02d.%03d: "
				, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

			Tools::appendStr(&buf, { log, endline });

			return writeStr(buf.data());
		}

		int DebugOutput::writeLog(const wchar_t* wlog, const wchar_t* wendline) const
		{
			std::vector<char> log;
			std::vector<char> endline;

			return writeLog(Tools::convUNICODE2MBCS(&log, wlog), Tools::convUNICODE2MBCS(&endline, wendline));
		}

	}

}
