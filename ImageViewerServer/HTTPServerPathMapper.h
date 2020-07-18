/*
	image viewer server - lightweight web server with extraction support for compressed archives

	Copyright(c) 2020 Luo Jie

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this softwareand associated documentation files(the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions :

	The above copyright noticeand this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
 */

#pragma once

#include "tools.h"

namespace lwhttp {

	inline constexpr unsigned char digital2hex(unsigned char ch) {
		if (ch < 10) {
			return static_cast<unsigned char>('0' + ch);
		}
		else {
			return static_cast<unsigned char>('A' - 10 + ch);
		}
	}

	class HTTPServerPathMapper {
	private:
		std::vector<wchar_t> m_root;
	public:
		HTTPServerPathMapper(const wchar_t *root) {
			Tools::transstr(&m_root, root);
		}

		const wchar_t *map(std::vector<wchar_t> *local, const char *path) const {
			local->clear();
			std::vector<wchar_t> upath;
			Tools::appendStr(local, { m_root.data(), Tools::convUTF82UNICODE(&upath, path) });
			for (auto &ch : *local) {
				if (ch == L'/') {
					ch = L'\\';
				}
			}
			return local->data();
		}

		static const char *encodePath(std::vector<char> *path, const wchar_t *pathstr) {
			std::vector<char> vrawpath;
			Tools::convUNICODE2UTF8(&vrawpath, pathstr);
			const char *rawpath = vrawpath.data();
			path->clear();

			while (*rawpath != 0) {
				if ((*rawpath >= 'A' && *rawpath <= 'Z') || (*rawpath >= 'a' && *rawpath <= 'z')
					|| (*rawpath >= '0' && *rawpath <= '9')
					|| *rawpath == '-' || *rawpath == '_' || *rawpath == '.' || *rawpath == '~'
					|| *rawpath == '/') {
					path->push_back(*rawpath);
				}
				else {
					path->push_back('%');
					path->push_back(digital2hex((*reinterpret_cast<const unsigned char *>(rawpath)) / 16));
					path->push_back(digital2hex((*reinterpret_cast<const unsigned char *>(rawpath)) % 16));
				}
				++rawpath;
			}
			path->push_back('\0');
			return path->data();
		}
	};
}