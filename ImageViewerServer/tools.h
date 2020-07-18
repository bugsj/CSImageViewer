#pragma once

#include <vector>
#include <stdexcept>
#include <numeric>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

template <typename T, std::size_t N>
constexpr std::size_t array_size(const T(&)[N]) { return N; }

#undef max
inline int CHECK_SIZE_MAXINT32(long long n)
{
	if (n > std::numeric_limits<int>::max()) {
		throw std::length_error("size too big: maxint32");
	}
	return static_cast<int>(n);
}

inline int CHECK_VALUE_MAXINT32(long long n)
{
	if (n > std::numeric_limits<int>::max()) {
		throw std::overflow_error("value too big: maxint32");
	}
	return static_cast<int>(n);
}

inline unsigned int CHECK_SIZE_MAXUINT32(long long n)
{
	if (n > std::numeric_limits<unsigned int>::max()) {
		throw std::length_error("size too big: maxuint32");
	}
	return static_cast<unsigned int>(n);
}

inline int CHECK_DATE_MAX(long long n)
{
#ifdef _DEBUG
	if (n > 99999999) {
		throw std::overflow_error("date too big: 99999999");
	}
#endif
	return static_cast<int>(n);
}

inline int CHECK_CODE_MAX(long long n)
{
#ifdef _DEBUG
	if (n > 99999999) {
		throw std::overflow_error("code too big: 99999999");
	}
#endif
	return static_cast<int>(n);
}

namespace lwhttp {

	namespace Tools {

		inline size_t templatestrlen(const wchar_t *s)
		{
			return wcslen(s);
		}

		inline size_t templatestrlen(const char *s)
		{
			return strlen(s);
		}

		inline int templatestricmp(const wchar_t *s1, const wchar_t *s2)
		{
			return _wcsicmp(s1, s2);
		}

		inline int templatestricmp(const char *s1, const char *s2)
		{
			return _stricmp(s1, s2);
		}

		template<class T>
		const T* appendStr(std::vector<T>* str, const std::initializer_list<const T*>& ss)
		{
			size_t s0len = str->empty() ? 0 : templatestrlen(str->data());
			size_t n = ss.size();
			std::vector<size_t> sslen(n);
			auto ss_i = ss.begin();
			for (auto& sslen_i : sslen) {
				sslen_i = templatestrlen(*ss_i++);
			}
			str->resize(std::accumulate(sslen.begin(), sslen.end(), s0len) + 1);
			ss_i = ss.begin();
			for (auto& sslen_i : sslen) {
				::CopyMemory(str->data() + s0len, *ss_i++, sizeof(**ss_i) * sslen_i);
				s0len += sslen_i;
			}
			str->data()[s0len] = static_cast<T>(0);
			return str->data();
		}

		template<class T>
		inline const T* appendStr(std::vector<T>* str, const T* s1)
		{
			return appendStr(str, { s1 });
		}

		const wchar_t* convMBCS2UNICODE(std::vector<wchar_t>* dst, const char* src);
		const char* convUNICODE2MBCS(std::vector<char>* dst, const wchar_t* src);

		inline const wchar_t *transstr(std::vector<wchar_t> *dst, const char *src)
		{
			return convMBCS2UNICODE(dst, src);
		}

		inline const wchar_t *transstr(std::vector<wchar_t> *dst, const wchar_t *src)
		{
			size_t size = ::wcslen(src) + 1;
			dst->resize(size);
			wmemcpy(dst->data(), src, size);
			return dst->data();
		}

		inline const char *transstr(std::vector<char> *dst, const char *src)
		{
			size_t size = ::strlen(src) + 1;
			dst->resize(size);
			memcpy(dst->data(), src, size);
			return dst->data();
		}

		inline const char *transstr(std::vector<char> *dst, const wchar_t *src)
		{
			return convUNICODE2MBCS(dst, src);
		}
	}

}
