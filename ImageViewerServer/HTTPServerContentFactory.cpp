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

#include "HTTPServerContent.h"
#include "HTTPServerContentFolder.h"
#include "HTTPServerContentFile.h"
#include "HTTPServerContentZIP.h"
#include "HTTPServerRequest.h"
#include "HTTPServerPathMapper.h"
#include "HTTPServerContentFactory.h"

namespace lwhttp {
	std::unique_ptr<HTTPServerContent> HTTPServerContentFactory::create(HTTPServerRequest &request)
	{
		std::unique_ptr<HTTPServerContent> content;

		const char *filename = request.getFileName();
		std::vector<wchar_t> local;
		const wchar_t *localpath = m_pathmapper->map(&local, request.getFullPath());
		Tools::TraceInfo({ L"local file:", localpath, L"\n" });
		if (*filename == '\0') {
			content = HTTPServerContentFolder::create(localpath, request);
		}
		else if (GetFileContentType(filename) == HTTPContentType::zip) {
			content = HTTPServerContentZIP::create(localpath, request);
		}
		else {
			content = HTTPServerContentFile::create(localpath, request);
		}

		return content;
	}
}
