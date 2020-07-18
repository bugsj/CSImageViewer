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

#include "HTTPServerStrings.h"

namespace lwhttp {

	namespace HTTPContentType {
		const char *plain = "text/plain";
		const char *html = "text/html";
		const char *css = "text/css";
		const char *jpeg = "image/jpeg";
		const char *png = "image/png";
		const char *gif = "image/gif";
		const char *webp = "image/webp";
		const char *icon = "image/x-icon";
		const char *mpeg = "video/mpeg";
		const char *mp4 = "video/mp4";
		const char *zip = "application/zip";
		const char *octet = "application/octet-stream";
	}

	std::map<HTTPStatusCode, const char *> HTTPStatusCodeString = {
		{HTTPStatusCode::OK, "200 OK"},
		{HTTPStatusCode::NotFound, "404 Not Found"},
		{HTTPStatusCode::InternalServerError, "500 Internal Server Error"}
	};

}
