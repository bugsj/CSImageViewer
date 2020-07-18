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

#include "DebugOutput.h"
#include "HTTPServerCommon.h"
#include "HTTPServerPathMapper.h"
#include "HTTPServerRequest.h"
#include "HTTPServerResponse.h"
#include "SimpleSocket.h"

namespace lwhttp {

	class HTTPServerResponser : public concurrency::agent
	{
	private:
		concurrency::ISource<HTTPServerAcceptMsg> &m_source;
		const HTTPServerPathMapper *m_pathmapper;
		SimpleSocket m_client;
		HTTPServerRequest m_request;
		HTTPServerResponse m_response;

	public:
		HTTPServerResponser() = delete;
		HTTPServerResponser(HTTPServerResponser &) = delete;
		explicit HTTPServerResponser(concurrency::ISource<HTTPServerAcceptMsg> &source, const HTTPServerPathMapper *mapper)
			: m_source(source), m_pathmapper(mapper), m_request(), m_response(m_request, mapper) {}

	protected:
		void run() override;

	public:
		int link();

		int respone();
		int writeReady() {
			m_response.send(m_client());
			return 0;
		}
	};

}
