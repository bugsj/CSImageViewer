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
#include <algorithm>
#include <memory>

#include <WinSock2.h>

#include "HTTPServerPathMapper.h"
#include "HTTPServerRequest.h"
#include "HTTPServerContent.h"
#include "HTTPServerContentFactory.h"

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

namespace lwhttp {

	class HTTPServerRequest;

	class HTTPServerResponse {
	private:
		HTTPServerRequest &m_request;
		std::vector<char> m_header;
		size_t m_current;
		std::unique_ptr<HTTPServerContent> m_content;
		HTTPServerContentFactory m_factory;
		size_t m_total_sent = 0;

	public:
		HTTPServerResponse(HTTPServerRequest &request, const HTTPServerPathMapper *pathmapper) : m_request(request), m_factory(pathmapper), m_current(0) {}
		size_t processRequest();

		void reset() {
			m_current = 0;
			m_total_sent = 0;
			m_content.reset();
		}

		int send(SOCKET s) {
			if (!m_content) {
				Tools::TraceInfo(L"Send 0 Content\n");
				return 0;
			}
			constexpr ULONG SENDBLOCK = 262144;
			WSABUF buf;
			DWORD sent = 0;
			DWORD total_send = static_cast<DWORD>(m_total_sent);
			int ret = 0;

			while (total_send < m_current && SOCKET_ERROR != ret)
			{
				buf.len = std::min(SENDBLOCK, static_cast<ULONG>(m_current - total_send));
				buf.buf = m_header.data() + total_send;
				ret = WSASend(s, &buf, 1, &sent, 0, 0, NULL);
				if (SOCKET_ERROR != ret) {
					total_send += sent;
				}
				else {
					int socket_err = WSAGetLastError();
					if (socket_err == WSAEWOULDBLOCK) {
						m_total_sent = total_send;
						DEBUGOUTPUTVAR(total_send);
						return total_send;
					}
					DEBUGOUTPUTVAR(socket_err);
				}
			}

			std::vector<char> contentbuf(SENDBLOCK);
			m_content->setPos(total_send - m_current);
			while (!m_content->end()) {
				sent = static_cast<DWORD>(m_content->read(contentbuf.data(), SENDBLOCK));
				buf.len = std::min(SENDBLOCK, sent);
				buf.buf = contentbuf.data();
				ret = WSASend(s, &buf, 1, &sent, 0, 0, NULL);
				if (ret != SOCKET_ERROR) {
					total_send += sent;
				}
				else {
					int socket_err = WSAGetLastError();
					m_total_sent = total_send;
					if (socket_err != WSAEWOULDBLOCK) {
						DEBUGOUTPUTVAR(socket_err);
					}
					break;
				}
			}

			Tools::StringFormatter<wchar_t> f;
			Tools::TraceInfo({ f.conv(total_send), L" of ", f.conv(m_current + m_content->size()), L" sent\n" });
			DEBUGOUTPUTVAR(total_send);
			return total_send;
		}

		bool getKeepAlive() { return m_request.getKeepAlive(); }

		size_t bufsize() { return m_header.size(); }
		size_t datasize() { return m_current; }
		char *data() { return m_header.data(); }

	};

}
