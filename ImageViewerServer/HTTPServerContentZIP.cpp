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

#include "HTTPServerContentZIP.h"
#include "HTTPServerContent.h"

namespace lwhttp {

	HTTPServerContentZIP::HTTPServerContentZIP(const wchar_t *file) {
		struct _stat64i32 attr;
		_wstat(file, &attr);
		m_modifiedtime = attr.st_mtime;

		if (m_zip.open(file) != 0) {
			m_contenttype = HTTPContentType::html;
			m_statuscode = HTTPStatusCode::NotFound;
		}
		else {
			m_contenttype = HTTPContentType::html;
			m_statuscode = HTTPStatusCode::OK;
		}
	}

	int HTTPServerContentZIP::createDirHTML() {
		long long n = m_zip.getNumEntries();
		Tools::transstr(&m_buf, "<HTML>\n<HEAD><TITLE>zip files</TITLE><META NAME=\"viewport\" CONTENT=\"width=device-width, initial-scale=1.0, minimum-scale=0.5, maximum-scale=2, user-scalable=yes\"/></HEAD>\n<BODY>\n");
		Tools::StringFormatter<char> sf;
		for (long long i = 0; i < n; ++i) {
			Tools::appendStr(&m_buf, { "<P><A HREF=\"?index=", sf.conv(i), "\">", m_zip.getName(i), "</A></P>\n" });
		}
		Tools::appendStr(&m_buf, "</BODY>\n</HTML>\n" );
		removeIllegalChar();
		m_contenttype = HTTPContentType::html;
		m_statuscode = HTTPStatusCode::OK;
		return 0;
	}

	int HTTPServerContentZIP::createDecompressedFile(long long index) {
		long long size = m_zip.getSize(index);
		if (m_zip.read(&m_buf, index) != 0) {
			m_contenttype = GetFileContentType(m_zip.getName(index));
			m_statuscode = HTTPStatusCode::OK;
		}
		else {
			m_statuscode = HTTPStatusCode::InternalServerError;
		}

		return 0;
	}


	std::unique_ptr<HTTPServerContent> HTTPServerContentZIP::create(const wchar_t *file, const HTTPServerRequest &request) {
		std::unique_ptr<HTTPServerContentZIP> content(new HTTPServerContentZIP(file));
		const char *p = request.param("index");
		if (p != nullptr) {
			content->createDecompressedFile(atoi(p));
		}
		else {
			content->createDirHTML();
		}
		size_t length = content->size();
		if (length == 0) {
			return std::make_unique<HTTPServerContent>();
		}
		return std::move(content);
	}

	HTTPServerContentZIP::~HTTPServerContentZIP() {
	}

	size_t HTTPServerContentZIP::size()
	{
		return m_buf.size();
	}

	size_t HTTPServerContentZIP::getCurrentPos()
	{
		return m_current;
	}

	bool HTTPServerContentZIP::end()
	{
		return  m_buf.size() == m_current;
	}

	bool HTTPServerContentZIP::setPos(size_t pos)
	{
		if (pos > m_buf.size()) {
			pos = m_buf.size();
		}
		m_current = pos;
		return true;
	}

	const char *HTTPServerContentZIP::contentType()
	{
		return m_contenttype;
	}

	long long HTTPServerContentZIP::modifiedTime()
	{
		return m_modifiedtime;
	}

	HTTPStatusCode HTTPServerContentZIP::statusCode()
	{
		return m_statuscode;
	}

	size_t HTTPServerContentZIP::read(char *buf, size_t bufsize)
	{
		if (bufsize > m_buf.size() - m_current) {
			bufsize = m_buf.size() - m_current;
		}
		memcpy(buf, m_buf.data() + m_current, bufsize);
		m_current += bufsize;
		return bufsize;
	}

}
