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

#include <agents.h>

#include "HTTPServerCommon.h"
#include "SimpleSocket.h"
#include "DebugOutput.h"

namespace lwhttp {

	class HTTPServerAcceptor : public concurrency::agent
	{
	private:
		concurrency::ITarget<HTTPServerAcceptMsg> &m_target;
		SimpleSocket m_socket;
		unsigned short m_port;

	public:
		HTTPServerAcceptor() = delete;
		HTTPServerAcceptor(HTTPServerAcceptor &) = delete;
		explicit HTTPServerAcceptor(concurrency::ITarget<HTTPServerAcceptMsg> &target, unsigned short port = 80) : m_target(target), m_port(port) {}

	protected:
		void run() override;

	public:
		int listen();

		int accept();
	};

}
