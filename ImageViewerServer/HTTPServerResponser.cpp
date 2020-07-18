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

#include "HTTPServerResponser.h"
#include "SimpleSocket.h"
#include "HTTPServerRequest.h"
#include "HTTPServerResponse.h"

namespace lwhttp {

	int HTTPServerResponser::respone()
	{
			m_request.append(m_client());
			if (!m_request.checkDoubleNewLine())
			{
				if (m_request.full()) {
					Tools::TraceInfo(L"Recv Buffer Full\n");
					return 1;
				}
				return 0;
			}
			m_response.reset();
			m_response.processRequest();
			Tools::TraceInfo(L"Sending\n");
			m_response.send(m_client());

			return 0;
	}

	int HTTPServerResponser::link()
	{
		WSANETWORKEVENTS networkevents;

		if (m_client.isEventInvalid()) {
			Tools::TraceInfo(L"Event Failed\n");
			return 0;
		}
		m_request.clear();
		int ret = m_client.selectEventIO();
		DWORD idx;
		do {
			idx = m_client.waitEvent();
			idx -= WAIT_OBJECT_0;
			if (idx == WSA_WAIT_TIMEOUT || idx == WSA_WAIT_FAILED) {
				continue;
			}

			ret = m_client.enumNetworkEvents(&networkevents);
			if (!networkevents.lNetworkEvents) {
				continue;
			}
			if (networkevents.lNetworkEvents & FD_READ) {
				if (respone() != 0) {
					break;
				}
			}
			if (networkevents.lNetworkEvents & FD_WRITE) {
				if (writeReady() != 0) {
					break;
				}
			}
			if (networkevents.lNetworkEvents & FD_CLOSE) {
				break;
			}
		} while (m_response.getKeepAlive());

		return 0;
	}

	void HTTPServerResponser::run()
	{
		HTTPServerAcceptMsg msg;

		do {
			msg = concurrency::receive(m_source);
			long long thread_id = GetCurrentThreadId();
			DEBUGOUTPUTVAR(thread_id);
			m_client.set(msg.socket);
		} while (link() == 0);
		done();
	}

}