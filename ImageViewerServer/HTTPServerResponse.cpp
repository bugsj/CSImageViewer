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

#include <memory>

#include "HTTPServerResponse.h"

#include "StringFormatter.h"
#include "DebugOutput.h"

#include "HTTPServerRequest.h"
#include "HTTPServerContentFactory.h"
#include "HTTPServerStrings.h"

namespace lwhttp {

	size_t HTTPServerResponse::processRequest()
	{
		m_request.decode();
		
		Tools::StringFormatter<char> sf;

		m_content = m_factory.create(m_request);

		const char *header = sf.join({ "HTTP/1.1 ", HTTPStatusCodeString[m_content->statusCode()], "\r\n"
			"Accept-Ranges: bytes\r\n"
			"Connection: ", getKeepAlive() ? "Keep-Alive" : "close","\r\n"
			"Content-Length: ", sf.conv(m_content->size()) ,"\r\n"
			"Content-Type: ", m_content->contentType(), "\r\n"
			"Date: ", sf.timestamp_now(), "\r\n"
			"Last-Modified: ", sf.timestamp(m_content->modifiedTime()), "\r\n"
			"Server: TESTSERVER\r\n"
			"\r\n"});

		m_current = strlen(Tools::transstr(&m_header, header));
		Tools::TraceInfo(L"Response Built\n");
		m_request.clear();

		return m_current + m_content->size();
	}

}
