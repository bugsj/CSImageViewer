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
#include <algorithm>

#include "ZipReader.h"

namespace lwhttp {

	class HTTPServerContentZIP : public HTTPServerContent {
	private:
		Tools::ZipReader m_zip;
		std::vector<long long> m_index;
		std::vector<char> m_buf;
		size_t m_current = 0;
		const char *m_contenttype = nullptr;
		long long m_modifiedtime = 0;
		HTTPStatusCode m_statuscode = HTTPStatusCode::InternalServerError;

	public:
		~HTTPServerContentZIP() override;
		size_t size() override;
		size_t getCurrentPos() override;
		bool end() override;
		bool setPos(size_t pos) override;
		const char *contentType() override;
		long long modifiedTime() override;
		HTTPStatusCode statusCode() override;
		size_t read(char *buf, size_t bufsize) override;

	private:
		HTTPServerContentZIP(const wchar_t *file);
		int createDirHTML();
		int createIndex();
		int createDecompressedFile(long long index);
		int createDecompressedFileFrame(long long index);
		void removeIllegalChar() {
			auto it = m_buf.begin();
			while (it != m_buf.end()) {
				if (*it != '\0') {
					++it;
				}
				else {
					it = m_buf.erase(it);
				}
			}
		}

	public:
		static std::unique_ptr<HTTPServerContent> create(const wchar_t *file, const HTTPServerRequest &request);
	};

}
