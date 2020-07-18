#include "tools.h"

#include <vector>
#include <Windows.h>

namespace lwhttp {

	namespace Tools {

		HINSTANCE ResouceModule = NULL;

		const wchar_t* convMBCS2UNICODE(std::vector<wchar_t>* dst, const char* src)
		{
			int len = CHECK_SIZE_MAXINT32(std::strlen(src));
			int size = ::MultiByteToWideChar(CP_ACP, 0, src, len, NULL, 0) + 1;
			dst->resize(size);
			len = ::MultiByteToWideChar(CP_ACP, 0, src, len, dst->data(), size);

			return dst->data();
		}

		const char* convUNICODE2MBCS(std::vector<char>* dst, const wchar_t* src)
		{
			int len = CHECK_SIZE_MAXINT32(std::wcslen(src));
			int size = ::WideCharToMultiByte(CP_ACP, 0, src, len, NULL, 0, NULL, NULL) + 1;
			dst->resize(size);
			len = ::WideCharToMultiByte(CP_ACP, 0, src, len, dst->data(), size, NULL, NULL);

			return dst->data();
		}

		const wchar_t *convUTF82UNICODE(std::vector<wchar_t> *dst, const char *src)
		{
			int len = CHECK_SIZE_MAXINT32(std::strlen(src));
			int size = ::MultiByteToWideChar(CP_UTF8, 0, src, len, NULL, 0) + 1;
			dst->resize(size);
			len = ::MultiByteToWideChar(CP_UTF8, 0, src, len, dst->data(), size);

			return dst->data();
		}

		const char *convUNICODE2UTF8(std::vector<char> *dst, const wchar_t *src)
		{
			int len = CHECK_SIZE_MAXINT32(std::wcslen(src));
			int size = ::WideCharToMultiByte(CP_UTF8, 0, src, len, NULL, 0, NULL, NULL) + 1;
			dst->resize(size);
			len = ::WideCharToMultiByte(CP_UTF8, 0, src, len, dst->data(), size, NULL, NULL);

			return dst->data();
		}

	}
}

