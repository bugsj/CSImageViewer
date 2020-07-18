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

#include <WinSock2.h>

#include "DebugOutput.h"

namespace lwhttp {

	struct HTTPServerAcceptMsg {
		int signal;
		SOCKET socket;
		sockaddr_in addr;
	};

	class EventHandle {
	private:
		WSAEVENT m_event;
	public:
		inline EventHandle() { m_event = ::WSACreateEvent(); }
		inline ~EventHandle() { ::CloseHandle(m_event); }
		inline WSAEVENT operator()() { return m_event; }
		inline const WSAEVENT *getPtr() { return &m_event; }
		inline bool isInvalid() { return m_event == WSA_INVALID_EVENT; }
		inline bool set() { return ::SetEvent(m_event); }
		inline bool reset() { return ::ResetEvent(m_event); }

		EventHandle(EventHandle &) = delete;
	};

	class SocketHandle {
	private:
		SOCKET m_socket;
	public:
		inline SocketHandle(SOCKET s) : m_socket(s) {}
		inline ~SocketHandle() { m_socket == INVALID_SOCKET ? 0 : ::closesocket(m_socket); }
		inline SOCKET operator()() { return m_socket; }
		inline bool isInvalid() { return m_socket == INVALID_SOCKET; }
		inline SOCKET release() { SOCKET socket = m_socket; m_socket = INVALID_SOCKET; return socket; }
		inline int bind(unsigned short port) {
			sockaddr_in local;
			local.sin_addr.S_un.S_addr = INADDR_ANY;
			local.sin_family = AF_INET;
			local.sin_port = ::htons(port);
			return ::bind(m_socket, reinterpret_cast<sockaddr *>(&local), sizeof(local));
		}
		inline int listen(int backlog) { return ::listen(m_socket, backlog); }
		inline int listen(unsigned short port, int backlog) { 
			if (this->bind(port) == SOCKET_ERROR) {
				return SOCKET_ERROR;
			}
			return this->listen(backlog); 
		}

		SocketHandle() = delete;
		SocketHandle(SocketHandle &) = delete;
	};

}
