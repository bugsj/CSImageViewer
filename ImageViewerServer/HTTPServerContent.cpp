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


namespace lwhttp {

	HTTPServerContent::~HTTPServerContent() {}

	size_t HTTPServerContent::size()
	{
		return 0;
	}

	size_t HTTPServerContent::getCurrentPos()
	{
		return 0;
	}
	
	bool HTTPServerContent::end()
	{
		return true;
	}
	
	bool HTTPServerContent::setPos(size_t pos)
	{
		return false;
	}
	
	const char *HTTPServerContent::contentType()
	{
		return HTTPContentType::octet;
	}

	long long HTTPServerContent::modifiedTime()
	{
		return 0;
	}

	HTTPStatusCode HTTPServerContent::statusCode()
	{
		return HTTPStatusCode::InternalServerError;
	}

	size_t HTTPServerContent::read(char *buf, size_t bufsize)
	{
		return 0;
	}

}