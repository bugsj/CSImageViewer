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

#include "StringFormatter.h"
#include "HTTPServerContentFolder.h"
#include "HTTPServerContent.h"
#include "HTTPServerPathMapper.h"

#include "DebugOutput.h"

#include <Windows.h>

namespace lwhttp {

	HTTPServerContentFolder::HTTPServerContentFolder(const wchar_t *path) {
		Tools::appendStr(&m_path, { path, L"*" });
	}

	bool HTTPServerContentFolder::scan()
	{
		WIN32_FIND_DATAW fd;

		HANDLE files = FindFirstFileW(m_path.data(), &fd);
		if (files == INVALID_HANDLE_VALUE) {
			return false;
		}

		m_foldername.clear();
		m_filename.clear();
		do {
			if (*reinterpret_cast<unsigned int *>(fd.cFileName) == 0x2e) { // fd.cFileName == L"."
				continue;
			}

			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				m_foldername.emplace_back(std::vector<wchar_t>());
				Tools::appendStr(&m_foldername.back(), { fd.cFileName, L"/" });
			}
			else {
				m_filename.emplace_back(std::vector<wchar_t>());
				Tools::transstr(&m_filename.back(), fd.cFileName);
			}
		} while (FindNextFileW(files, &fd));

		return true;
	}

	bool HTTPServerContentFolder::createPlain()
	{
		auto size_acc_op = [](size_t s, const std::vector<wchar_t> &t) { return s + t.size() + 1; };
		size_t size_est = std::accumulate(m_foldername.begin(), m_foldername.end(), size_t(0), size_acc_op)
		                + std::accumulate(m_filename.begin(), m_filename.end(), size_t(0), size_acc_op)
						+ size_t(1);

		std::vector<wchar_t> buf;
		buf.reserve(size_est);
		for (const auto &p : m_foldername) {
			Tools::appendStr(&buf, { p.data(), L"\n" });
		}
		for (const auto &p : m_filename) {
			Tools::appendStr(&buf, { p.data(), L"\n" });
		}
		Tools::transstr(&m_buf, buf.data());
		m_buf.erase(std::remove(m_buf.begin(), m_buf.end(), '\0'), m_buf.end());

		m_contenttype = HTTPContentType::plain;
		m_statuscode = HTTPStatusCode::OK;
		m_current = 0;
		m_modifiedtime = 0;
		DEBUGOUTPUTVAR(m_buf.size());
		return true;
	}

	bool HTTPServerContentFolder::createHTML(const HTTPServerRequest &request)
	{
		auto size_acc_op = [](size_t s, const std::vector<wchar_t> &t) { return s + t.size() + 1; };
		size_t size_est = std::accumulate(m_foldername.begin(), m_foldername.end(), size_t(0), size_acc_op)
			+ std::accumulate(m_filename.begin(), m_filename.end(), size_t(0), size_acc_op)
			+ size_t(1);

		std::vector<char> buf;
		Tools::StringFormatter<char> sf;
		buf.reserve(size_est);
		Tools::appendStr(&buf, { "<HTML>\n<HEAD>\n<TITLE>DIRECTORY</TITLE>\n<META NAME=\"viewport\" CONTENT=\"width=device-width, initial-scale=1.0, minimum-scale=0.5, maximum-scale=2, user-scalable=yes\"/></HEAD>\n<BODY>\n" });

		std::vector<char> tmp;
		for (const auto &p : m_foldername) {
			Tools::appendStr(&buf, { "<P><A HREF=\"", HTTPServerPathMapper::encodePath(&tmp, p.data()), "\">", sf.conv(p.data()), "</A></P>\n" });
		}
		for (const auto &p : m_filename) {
			Tools::appendStr(&buf, { "<P><A HREF=\"", HTTPServerPathMapper::encodePath(&tmp, p.data()), "\">", sf.conv(p.data()), "</A></P>\n" });
		}
		for (const auto &p : request.param()) {
			Tools::appendStr(&buf, { "<P>", p.first.c_str(), ":", p.second.c_str(), "</P>" });
		}
		Tools::appendStr(&buf, { "</BODY>\n</HTML>\n" });

		m_buf.swap(buf);
		m_buf.erase(std::remove(m_buf.begin(), m_buf.end(), '\0'), m_buf.end());

		m_contenttype = HTTPContentType::html;
		m_statuscode = HTTPStatusCode::OK;
		m_current = 0;
		m_modifiedtime = 0;
		DEBUGOUTPUTVAR(m_buf.size());
		return true;
	}

	size_t HTTPServerContentFolder::size()
	{
		return m_buf.size();
	}

	size_t HTTPServerContentFolder::getCurrentPos()
	{
		return m_current;
	}

	bool HTTPServerContentFolder::end()
	{
		return m_current == m_buf.size();
	}

	bool HTTPServerContentFolder::setPos(size_t pos)
	{
		if (pos > m_buf.size()) {
			pos = m_buf.size();
		}
		m_current = pos;
		return true;
	}

	const char *HTTPServerContentFolder::contentType()
	{
		return m_contenttype;
	}

	long long HTTPServerContentFolder::modifiedTime()
	{
		return m_modifiedtime;
	}

	HTTPStatusCode HTTPServerContentFolder::statusCode()
	{
		return m_statuscode;
	}

	size_t HTTPServerContentFolder::read(char *buf, size_t bufsize)
	{
		if (bufsize > m_buf.size() - m_current) {
			bufsize = m_buf.size() - m_current;
		}
		memcpy(buf, m_buf.data(), bufsize);
		m_current += bufsize;
		return bufsize;
	}

}