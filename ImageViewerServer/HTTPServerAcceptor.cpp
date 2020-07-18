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

#include "HTTPServerAcceptor.h"

#include "SimpleSocket.h"

namespace lwhttp {

	int HTTPServerAcceptor::accept() 
	{
		sockaddr_in client;
		int client_size = sizeof(client);

		SOCKET conn = ::WSAAccept(m_socket(), reinterpret_cast<sockaddr *>(&client), &client_size, NULL, NULL);

		if (conn != INVALID_SOCKET) {
			Tools::TraceInfo(L"Accepting...\n");

			HTTPServerAcceptMsg msg;
			msg.signal = 0;
			msg.socket = conn;
			msg.addr = client;
			concurrency::send(m_target, msg);
			return 0;
		}
		else {
			Tools::TraceInfo(L"Accept Failed\n");
			return 1;
		}

	}

	int HTTPServerAcceptor::listen()
	{
		if (m_socket.isInvalid()) {
			Tools::TraceInfo(L"Socket Create Failed\n");
			return 1;
		}

		constexpr int BACKLOG = 5;
		DEBUGOUTPUTVAR(m_port);
		if (m_socket.listen(m_port, BACKLOG) == SOCKET_ERROR) {
			Tools::TraceInfo(L"Listen Failed\n");
			return 2;
		}

		m_socket.selectEventAccept();
		Tools::TraceInfo(L"Listening...\n");

		WSANETWORKEVENTS networkevent;

		do {
			Tools::TraceInfo(L"Waiting...\n");
			DWORD idx = m_socket.waitEvent();
			idx -= WSA_WAIT_EVENT_0;
			if (idx == WSA_WAIT_TIMEOUT || idx == WSA_WAIT_FAILED) {
				continue;
			}
			Tools::TraceInfo(L"Event...\n");

			m_socket.enumNetworkEvents(&networkevent);
			if (networkevent.lNetworkEvents == FD_ACCEPT && networkevent.iErrorCode[FD_ACCEPT_BIT] == 0) {
				accept();
			}
		} while (true);

		return 1;
	}
		
	void HTTPServerAcceptor::run()
	{
		listen();
		done();
	}

}
