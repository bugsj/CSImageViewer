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
#include <map>
#include <string>

#include <WinSock2.h>

#include "DebugOutput.h"

namespace lwhttp {

	//enum class RequestItem {
	//	Method,
	//	FullPath,
	//	FileName,
	//};
	enum class RequestMethod {
		HTTP_GET, HTTP_POST, HTTP_PUT, HTTP_DELETE, HTTP_NULL
	};

	constexpr size_t RECV_BUFF_DEFAULT_SIZE = 65536;


	class HTTPServerRequest {
	private:
		std::vector<char> m_buf;
		size_t m_current;
		RequestMethod m_method;
		std::vector<char> m_fullpath;
		std::vector<char> m_filename;
		std::map<std::string, std::string> m_param;
		std::vector<char> m_paramline;
		
	public:
		HTTPServerRequest() : m_buf(RECV_BUFF_DEFAULT_SIZE), m_current(0), m_method(RequestMethod::HTTP_NULL) {}

		void clear() { m_current = 0; }
		bool full() const { return m_buf.size() == m_current; }

		size_t append(char *data, size_t size) {
			size_t bufsize = m_buf.size();
			if (m_current + size > bufsize) {
				m_buf.resize(m_current + size + RECV_BUFF_DEFAULT_SIZE);
			}
			memcpy(m_buf.data(), data, size);
			m_current += size;
			return size;
		}
		size_t append(SOCKET s) {
			std::vector<char> recv_buf(m_buf.size() - m_current);
			WSABUF buf = { static_cast<ULONG>(recv_buf.size()), recv_buf.data() };
			DWORD recv_cnt;
			DWORD flags = 0;
			int ret = WSARecv(s, &buf, 1, &recv_cnt, &flags, NULL, NULL);

			return append(recv_buf.data(), recv_cnt);
		}


		size_t set(char *data, size_t size) {
			clear();
			return append(data, size);
		}

		bool checkDoubleNewLine() const {
			return m_current >= 4 && *reinterpret_cast<const unsigned int *>(m_buf.data() + m_current - 4) == 0xa0d0a0d;
		}

		bool decode();
		const char *getFullPath() const { return m_fullpath.data(); }
		const char *getFileName() const { return m_filename.data(); }
		const bool getKeepAlive() const { return true; }
		const std::map<std::string, std::string> &param() const { return m_param; }
		const char *param(const char *key) const {
			auto rs = m_param.find(key);
			if (rs != m_param.end()) {
				return rs->second.c_str();
			}
			else {
				return nullptr;
			}
		}
		const char *paramline() const { return m_paramline.data(); }

		size_t bufsize() const { return m_buf.size(); }
		size_t datasize() const { return m_current; }
		const char *data() const { return m_buf.data(); }
	};
}