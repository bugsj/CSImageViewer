#pragma once

#include <vector>
#include <memory>
#include "tools.h"
#include <type_traits>
#include <time.h>

namespace lwhttp {

	namespace Tools {
		
		template<class T> inline constexpr T t_char(wchar_t wc, char c) {
			return std::is_same<T, wchar_t>::value ? static_cast<T>(wc) : static_cast<T>(c);
		}

		template<class T> inline constexpr const T *t_char(const wchar_t* wc, const char* c) {
			return std::is_same<T, wchar_t>::value ? reinterpret_cast<const T*>(wc) : reinterpret_cast<const T*>(c);
		}

#define T_TEXT(expr_type, expr) lwhttp::Tools::t_char<expr_type>((L ## expr), (expr))

		template<class T> inline T last_digit_to_char(long long n) { return T_TEXT(T, '0') + static_cast<T>(n % 10); }

		constexpr int STRING_BUF_DEFAULT_SIZE = 1024;

		template<class T>
		class StringFormatter
		{
		private:
			std::vector<std::vector<T>> m_buf;

		private:

		public:
			StringFormatter() {}
			~StringFormatter() {}

			template<class U>
			const T *join(const std::vector<U>& ss) {
				static_assert(std::is_integral<U>::value, "integer only");
				if (ss.empty()) {
					return T_TEXT(T, "");
				}

				std::vector<T> item;
				item.reserve(STRING_BUF_DEFAULT_SIZE);
				for (auto n : ss) {
					if (item.empty()) {
						appendStr(&item, conv(n));
					}
					else {
						appendStr(&item, { T_TEXT(T, ","), conv(n) });
					}
				}
				m_buf.push_back(std::move(item));
				return m_buf.back().data();
			}

			inline const T *join(const std::initializer_list<const T*>& ss) {
				m_buf.emplace_back(std::vector<T>());
				return appendStr(&m_buf.back(), ss);
			}

			const T *conv(long long n) {
				if (n == 0) {
					return T_TEXT(T, "0");
				}

				std::vector<T> buf;
				int minus_sign = (n < 0 ? 1 : 0);
				long long absn = std::abs(n);
				buf.reserve(std::numeric_limits<long long>::digits10 + 4);
				while (absn > 0) {
					buf.push_back(last_digit_to_char<T>(absn));
					absn /= 10;
				}

				m_buf.emplace_back(std::vector<T>(buf.size() + minus_sign + 1));
				auto ptr = m_buf.back().begin();
				if (minus_sign == 1) {
					*ptr++ = T_TEXT(T, '-');
				}
				for (auto bufiter = buf.rbegin(); bufiter != buf.rend(); ++bufiter) {
					*ptr++ = *bufiter;
				}
				*ptr = T(0);
				
				return m_buf.back().data();
			}

			inline const T *conv(const wchar_t *s) {
				m_buf.emplace_back(std::vector<T>());
				transstr(&m_buf.back(), s);
				return m_buf.back().data();
			}

			inline const T *conv(const char *s) {
				m_buf.emplace_back(std::vector<T>());
				transstr(&m_buf.back(), s);
				return m_buf.back().data();
			}

			const T *timestamp(time_t ltime) {
				struct tm gtime;
				gmtime_s(&gtime, &ltime);
				char str[128];
				strftime(str, sizeof(str), "%a, %d %b %Y %H:%M:%S GMT", &gtime);

				m_buf.emplace_back(std::vector<T>());
				std::vector<T> &buf = m_buf.back();

				return transstr(&buf, str);
			}

			inline const T *timestamp_now() {
				time_t ltime;
				time(&ltime);

				return timestamp(ltime);
			}
		};

	}

}

