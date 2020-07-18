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
#include <memory>

#include "DebugOutput.h"

#include "HTTPServerAcceptor.h"
#include "HTTPServerResponser.h"
#include "HTTPServerPathMapper.h"

namespace lwhttp {

	class HTTPServer
	{
	private:
		concurrency::unbounded_buffer<HTTPServerAcceptMsg> m_msgbuf;
		std::unique_ptr<HTTPServerAcceptor> m_acceptor;
		std::vector< std::unique_ptr<HTTPServerResponser> > m_responsers;
		HTTPServerPathMapper m_pathmapper;

	public:
		HTTPServer() : m_pathmapper(L"d:\\SHARE") {
			WSADATA wsa;
			int WSAStartup_result = WSAStartup(MAKEWORD(2, 2), &wsa);
			DEBUGOUTPUTVAR(WSAStartup_result);
		}

	public:
		bool start(int responser_cnt)
		{
			bool result = true;

			m_responsers.reserve(responser_cnt);
			for (int i = 0; i < responser_cnt; ++i) {
				m_responsers.push_back(std::make_unique<HTTPServerResponser>(m_msgbuf, &m_pathmapper));
				result = (result && m_responsers.back()->start());
			}

			m_acceptor = std::make_unique<HTTPServerAcceptor>(m_msgbuf);
			result = (result && m_acceptor->start());
			return result;
		}
	};

}

