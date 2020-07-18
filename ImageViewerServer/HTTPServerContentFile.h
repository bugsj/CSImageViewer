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

#include "HTTPServerContent.h"
#include "HTTPServerRequest.h"

#include <memory>

namespace lwhttp {

	class HTTPServerContentFile : public HTTPServerContent {
	private:
		FILE *m_file;
		size_t m_filesize;
		const char *m_contenttype;
		long long m_modifiedtime;
		HTTPStatusCode m_statuscode;

	public:
		~HTTPServerContentFile() override;
		size_t size() override;
		size_t getCurrentPos() override;
		bool end() override;
		bool setPos(size_t pos) override;
		const char *contentType() override;
		long long modifiedTime() override;
		HTTPStatusCode statusCode() override;
		size_t read(char *buf, size_t bufsize) override;

	private:
		HTTPServerContentFile(const wchar_t *file);

	public:
		static std::unique_ptr<HTTPServerContent> create(const wchar_t *file, const HTTPServerRequest &request);
	};

}
