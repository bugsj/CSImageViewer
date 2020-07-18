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

#include <vector>
#include <memory>

#include "HTTPServerContent.h"
#include "HTTPServerRequest.h"

#include "DebugOutput.h"

namespace lwhttp {

	class HTTPServerContentFolder : public HTTPServerContent {
	private:
		std::vector<std::vector<wchar_t>> m_foldername;
		std::vector<std::vector<wchar_t>> m_filename;
		std::vector<char> m_buf;
		std::vector<wchar_t> m_path;
		size_t m_current = 0;
		const char *m_contenttype = nullptr;
		long long m_modifiedtime = 0;
		HTTPStatusCode m_statuscode = HTTPStatusCode::InternalServerError;
	public:
		~HTTPServerContentFolder() override {}
		size_t size() override;
		size_t getCurrentPos() override;
		bool end() override;
		bool setPos(size_t pos) override;
		const char *contentType() override;
		long long modifiedTime() override;
		HTTPStatusCode statusCode() override;
		size_t read(char *buf, size_t bufsize) override;

	private:
		HTTPServerContentFolder(const wchar_t *path);
		bool scan();
		bool createPlain();
		bool createHTML(const HTTPServerRequest &request);

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
		static std::unique_ptr<HTTPServerContent> create(const wchar_t *path, const HTTPServerRequest &request) {
			std::unique_ptr<HTTPServerContentFolder> content(new HTTPServerContentFolder(path));

			content->scan();
			content->createHTML(request);
			size_t length = content->size();

			if (length == 0) {
				return std::make_unique<HTTPServerContent>();
			}
			Tools::TraceInfo(L"folder content created\n");
			return std::move(content);
		}

	};

}
