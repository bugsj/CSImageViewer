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

#include "HTTPServerContentFile.h"
#include "HTTPServerContent.h"
#include "HTTPServerStrings.h"

#include "cstdio"

namespace lwhttp {

	HTTPServerContentFile::HTTPServerContentFile(const wchar_t *file) {
		struct _stat64i32 attr;
		_wstat(file, &attr);
		m_modifiedtime = attr.st_mtime;

		errno_t err = _wfopen_s(&m_file, file, L"rb");
		if (err == 0) {
			m_filesize = attr.st_size;
			m_contenttype = GetFileContentType(file);
			m_statuscode = HTTPStatusCode::OK;
		}
		else {
			m_filesize = 0;
			m_contenttype = HTTPContentType::octet;
			m_statuscode = HTTPStatusCode::NotFound;
		}
	}

	std::unique_ptr<HTTPServerContent> HTTPServerContentFile::create(const wchar_t *file, const HTTPServerRequest &request) {
		std::unique_ptr<HTTPServerContent> content(new HTTPServerContentFile(file));
		size_t length = content->size();
		if (length == 0) {
			content = std::make_unique<HTTPServerContent>();
		}
		return content;
	}

	HTTPServerContentFile::~HTTPServerContentFile() {
		if (m_file != NULL) {
			fclose(m_file);
		}
	}
	
	size_t HTTPServerContentFile::size() 
	{
		return m_filesize;
	}
	
	size_t HTTPServerContentFile::getCurrentPos() 
	{
		if (m_file == NULL) {
			return 0;
		}
		fpos_t pos;
		fgetpos(m_file, &pos);
		return static_cast<size_t>(pos);
	}

	bool HTTPServerContentFile::end() 
	{
		if (m_file == NULL) {
			return true;
		}
		fpos_t pos;
		fgetpos(m_file, &pos);
		return static_cast<size_t>(pos) == m_filesize;
	}

	bool HTTPServerContentFile::setPos(size_t pos)
	{
		if (m_file == NULL) {
			return false;
		}
		return fseek(m_file, static_cast<long>(pos), SEEK_SET) == 0;
	}

	const char *HTTPServerContentFile::contentType()
	{
		return m_contenttype;
	}

	long long HTTPServerContentFile::modifiedTime()
	{
		return m_modifiedtime;
	}

	HTTPStatusCode HTTPServerContentFile::statusCode()
	{
		return m_statuscode;
	}

	size_t HTTPServerContentFile::read(char *buf, size_t bufsize)
	{
		if (m_file == NULL) {
			return 0;
		}
		return fread(buf, 1, bufsize, m_file);
	}


}
