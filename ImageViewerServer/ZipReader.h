#pragma once

#include <vector>
#include <io.h>
#include <fcntl.h>

#include "StringFormatter.h"
#include "zip.h"

namespace lwhttp {

	namespace Tools {

		class ZipReader {
		private:
			zip_t *m_za;
		public:
			ZipReader() : m_za(nullptr) {}
			~ZipReader() {
				if (m_za != nullptr) {
					zip_close(m_za);
					m_za = nullptr;
				}
			}

			int open(const wchar_t *wname) {
				int fd = 0;
				errno_t err = _wsopen_s(&fd, wname, _O_RDONLY | _O_RANDOM | _O_BINARY, _SH_DENYNO, _S_IREAD);
				if (err != 0) {
					return err;
				}
				zip_t *za;
				int error;
				za = zip_fdopen(fd, ZIP_RDONLY, &error);
				if (za != NULL) {
					if (m_za != nullptr) {
						zip_close(m_za);
					}
					m_za = za;
					return 0;
				}
				else {
					_close(fd);
					Tools::StringFormatter<wchar_t> sf;
					Tools::TraceInfo(L"zip: zip open failed(");
					Tools::TraceInfo(sf.conv(error));
					Tools::TraceInfo(L")\n");
					return error;
				}
			}

			long long getSize(long long index) {
				struct zip_stat sb;
				if (zip_stat_index(m_za, index, 0, &sb) == 0 && (sb.valid & ZIP_STAT_SIZE)) {
					return sb.size;
				}
				else {
					return 0;
				}
			}

			long long read(std::vector<char> *buf, long long index) {
				long long size = getSize(index);
				buf->resize(size);
				if (size == 0) {
					return 0;
				}

				zip_file_t *uf = zip_fopen_index(m_za, index, 0);
				if (uf == NULL) {
					return 0;
				}

				long long total_read = 0;
				long long size_read = 0;
				while (total_read < size) {
					size_read = zip_fread(uf, buf->data() + total_read, size - total_read);
					if (size_read == -1) {
						break;
					}
					DEBUGOUTPUTVAR(size_read);
					total_read += size_read;
				}

				zip_fclose(uf);
				return size;
			}

			long long getNumEntries() {
				return zip_get_num_entries(m_za, 0);
			}

			const char *getName(long long index) {
				return zip_get_name(m_za, index, ZIP_FL_ENC_RAW);
			}

		};

	}

}