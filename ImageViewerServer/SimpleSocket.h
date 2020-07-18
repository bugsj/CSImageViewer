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

	class SimpleSocket {
	private:
		SOCKET m_socket;
		WSAEVENT m_event;

	public:
		inline SimpleSocket(SOCKET s) : m_socket(s) { m_event = ::WSACreateEvent(); }
		inline SimpleSocket() { m_socket = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED); m_event = ::WSACreateEvent(); }
		inline ~SimpleSocket() { ::closesocket(m_socket); ::WSACloseEvent(m_event); }
		inline SimpleSocket &set(SOCKET s) { ::closesocket(m_socket); m_socket = s; resetEvent(); return *this; }
		inline SOCKET operator()() { return m_socket; }
		inline bool isSocketInvalid() { return m_socket == INVALID_SOCKET; }
		inline bool isEventInvalid() { return m_event == WSA_INVALID_EVENT; }
		inline bool isInvalid() { return m_socket == INVALID_SOCKET || m_event == WSA_INVALID_EVENT; }
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
		inline int selectEventAccept() { return ::WSAEventSelect(m_socket, m_event, FD_ACCEPT); }
		inline int selectEventIO() { return ::WSAEventSelect(m_socket, m_event, FD_READ | FD_WRITE | FD_CLOSE); }
		inline DWORD waitEvent(DWORD timeout = WSA_INFINITE) { return ::WSAWaitForMultipleEvents(1, &m_event, FALSE, timeout, FALSE); }
		inline int enumNetworkEvents(WSANETWORKEVENTS *networkevent) { return ::WSAEnumNetworkEvents(m_socket, m_event, networkevent); }
		inline bool resetEvent() { return ::WSAResetEvent(m_event); }
		inline bool setEvent() { return ::WSASetEvent(m_event); }

		SimpleSocket(SimpleSocket &) = delete;
	};

}
