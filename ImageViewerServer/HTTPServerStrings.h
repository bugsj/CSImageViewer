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

#include <map>

#include "StringFormatter.h"

namespace lwhttp {

	namespace HTTPContentType {
		extern const char *plain;
		extern const char *html;
		extern const char *css;
		extern const char *icon;
		extern const char *jpeg;
		extern const char *png;
		extern const char *gif;
		extern const char *webp;
		extern const char *mpeg;
		extern const char *mp4;
		extern const char *zip;
		extern const char *octet;
	}

	enum class HTTPStatusCode : int {
		OK = 200,
		NotFound = 404,
		InternalServerError = 500
	};

	extern std::map<HTTPStatusCode, const char *> HTTPStatusCodeString;

	//const wchar_t *GetFileExt(const wchar_t *file);
	//const char *GetFileContentType(const wchar_t *file);
	//const char *GetFileExt(const char *file);
	//const char *GetFileContentType(const char *file);

	

	template<typename T>
	const T *GetFileExt(const T *file) {
		const T *extend = file + Tools::templatestrlen(file);
		const T *extbegin = extend;

		while (extbegin > file && *extbegin != T_TEXT(T, '/') && *extbegin != T_TEXT(T, '\\')) {
			--extbegin;
			if (*extbegin == T_TEXT(T, '.')) {
				return extbegin;
			}
		}
		return extend;
	}

	template<typename T>
	const char *GetFileContentType(const T *file) {
		const T *ext = GetFileExt(file);
		const char *contenttype;
		if (Tools::templatestricmp(ext, T_TEXT(T, ".html")) == 0 || Tools::templatestricmp(ext, T_TEXT(T, ".htm")) == 0) {
			contenttype = HTTPContentType::html;
		}
		else if (Tools::templatestricmp(ext, T_TEXT(T, ".ico")) == 0) {
			contenttype = HTTPContentType::icon;
		}
		else if (Tools::templatestricmp(ext, T_TEXT(T, ".jpg")) == 0 || Tools::templatestricmp(ext, T_TEXT(T, ".jpeg")) == 0) {
			contenttype = HTTPContentType::jpeg;
		}
		else if (Tools::templatestricmp(ext, T_TEXT(T, ".png")) == 0) {
			contenttype = HTTPContentType::png;
		}
		else if (Tools::templatestricmp(ext, T_TEXT(T, ".gif")) == 0) {
			contenttype = HTTPContentType::gif;
		}
		else if (Tools::templatestricmp(ext, T_TEXT(T, ".webp")) == 0) {
			contenttype = HTTPContentType::webp;
		}
		else if (Tools::templatestricmp(ext, T_TEXT(T, ".css")) == 0) {
			contenttype = HTTPContentType::css;
		}
		else if (Tools::templatestricmp(ext, T_TEXT(T, ".txt")) == 0) {
			contenttype = HTTPContentType::plain;
		}
		else if (Tools::templatestricmp(ext, T_TEXT(T, ".zip")) == 0) {
			contenttype = HTTPContentType::zip;
		}
		else {
			contenttype = HTTPContentType::octet;
		}
		return contenttype;
	}

}
