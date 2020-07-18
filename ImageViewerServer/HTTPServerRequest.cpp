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

#include <algorithm>

#include "HTTPServerRequest.h"


namespace lwhttp {

	constexpr unsigned int MAGICNUMBER_GET = 0x20544547;
	constexpr unsigned int MAGICNUMBER_POST = 0x54534f50;
	constexpr unsigned int MAGICNUMBER_PUT = 0x20545550;
	constexpr unsigned int MAGICNUMBER_DELETE = 0x454c4544;

	constexpr unsigned int GETSPACE_LEN = 4;
	constexpr unsigned int POSTSPACE_LEN = 5;
	constexpr unsigned int PUTSPACE_LEN = 4;
	constexpr unsigned int DELETESPACE_LEN = 7;

	bool is_pathch(const char ch)
	{
		return ch != ' ' && ch != '?' && ch != '\r' && ch != '\0';
	}

	bool is_wordch(const char ch)
	{
		return ch != ' ' && ch != '\n' && ch != '\r' && ch != '\0';
	}

	bool is_hex(const char ch)
	{
		if (ch >= '0' && ch <= '9') {
			return true;
		}
		if (ch >= 'A' && ch <= 'F') {
			return true;
		}
		if (ch >= 'a' && ch <= 'f') {
			return true;
		}
		return false;
	}

	int hex2dec(const char ch)
	{
		if (ch >= '0' && ch <= '9') {
			return ch - '0';
		}
		if (ch >= 'A' && ch <= 'F') {
			return ch - 'A' + 10;
		}
		if (ch >= 'a' && ch <= 'f') {
			return ch - 'a' + 10;
		}
		return 0;
	}

	bool HTTPServerRequest::decode()
	{
		if (!checkDoubleNewLine()) {
			return false;
		}

		char *pathptr;
		unsigned int head = *reinterpret_cast<unsigned int *>(m_buf.data());
		switch (head) {
		case MAGICNUMBER_POST:
			m_method = RequestMethod::HTTP_POST;
			pathptr = m_buf.data() + POSTSPACE_LEN;
			Tools::TraceInfo(L"request: POST\n");
			break;
		case MAGICNUMBER_DELETE:
			m_method = RequestMethod::HTTP_DELETE;
			pathptr = m_buf.data() + DELETESPACE_LEN;
			Tools::TraceInfo(L"request: DELETE\n");
			break;
		case MAGICNUMBER_GET:
			m_method = RequestMethod::HTTP_GET;
			pathptr = m_buf.data() + GETSPACE_LEN;
			Tools::TraceInfo(L"request: GET\n");
			break;
		case MAGICNUMBER_PUT:
			m_method = RequestMethod::HTTP_PUT;
			pathptr = m_buf.data() + PUTSPACE_LEN;
			Tools::TraceInfo(L"request: PUT\n");
			break;
		default:
			m_method = RequestMethod::HTTP_NULL;
			pathptr = m_buf.data();
			Tools::TraceInfo(L"request: error\n");
		}

		m_fullpath.clear();
		while (is_pathch(*pathptr)) {
			if (pathptr[0] == '%' && is_hex(pathptr[1]) && is_hex(pathptr[2])) {
				m_fullpath.push_back(hex2dec(pathptr[1]) * 16 + hex2dec(pathptr[2]));
				pathptr += 3;
			}
			else {
				m_fullpath.push_back(*pathptr);
				++pathptr;
			}
		}

		m_param.clear();
		m_paramline.clear();
		if (*pathptr == '?') {
			std::string key;
			std::string current;
			while (is_wordch(*pathptr++)) {
				if (pathptr[0] == '%' && is_hex(pathptr[1]) && is_hex(pathptr[2])) {
					current.push_back(hex2dec(pathptr[1]) * 16 + hex2dec(pathptr[2]));
					m_paramline.push_back(hex2dec(pathptr[1]) * 16 + hex2dec(pathptr[2]));
					pathptr += 2;
				}
				else if (*pathptr == '=') {
					key.swap(current);
					current.clear();
					m_paramline.push_back(*pathptr);
				}
				else if (*pathptr == '&' || !is_wordch(*pathptr)) {
					if (key.empty() && current.empty()) {
						continue;
					}
					if (key.empty()) {
						key.swap(current);
						current.clear();
					}
					m_param.emplace(std::make_pair(std::move(key), std::move(current)));
					key.clear();
					current.clear();
					m_paramline.push_back(*pathptr);
				}
				else {
					current.push_back(*pathptr);
					m_paramline.push_back(*pathptr);
				}
			}
		}
		m_paramline.push_back('\0');
		Tools::TraceInfo({ "request param: ", m_paramline.data(), "\n" });

		m_fullpath.push_back('\0');
		Tools::TraceInfo({ "request path: ", m_fullpath.data(), "\n" });

		auto fiter = m_fullpath.rbegin() + 1;
		while (fiter != m_fullpath.rend() && *fiter != '/') {
			m_filename.push_back(*fiter++);
		}
		std::reverse(m_filename.begin(), m_filename.end());
		m_filename.push_back('\0');
		Tools::TraceInfo({ "request file: ", m_filename.data(), "\n" });

		return true;
	}
}