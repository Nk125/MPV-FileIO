#include <pipe/os_specific.hpp>
#include <string>

#ifndef _WIN32
#error OS not supported yet
#else
#include <windows.h>
#endif

namespace pipe {
	class os_specific::pipe_impl {
	private:
		HANDLE underlyingPipe = INVALID_HANDLE_VALUE;
		OVERLAPPED writeOperation = {0}, readOperation = {0};

		std::string pipename;

		void acquirePipe() {
			bool acquired = false;

			while (!acquired) {
				acquired = WaitNamedPipeA(
					pipename.c_str(),
					NMPWAIT_WAIT_FOREVER
				);
			}
		}

		bool os_open() {
			while (true) {
				underlyingPipe = CreateFileA(
					pipename.c_str(),
					GENERIC_READ | GENERIC_WRITE,
					0,
					NULL,
					OPEN_EXISTING,
					FILE_FLAG_OVERLAPPED,
					NULL);

				if (underlyingPipe != INVALID_HANDLE_VALUE) {
					return true;
				}

				// Exit if an error other than ERROR_PIPE_BUSY occurs.

				DWORD gle = GetLastError();

				if (gle != ERROR_PIPE_BUSY) {
					break;
				}
				else {
					acquirePipe();
				}
			}

			return false;
		}

		bool adjustPipe() {
			DWORD mode = PIPE_READMODE_MESSAGE;

			bool succesfullyAdjusted = SetNamedPipeHandleState(
				underlyingPipe,	 // pipe handle
				&mode, // new pipe mode
				NULL,	 // don't set maximum bytes
				NULL);	 // don't set maximum time

			return succesfullyAdjusted;
		}

	public:
		void changePipeName(std::string_view newName) {
			pipename.assign(newName);
		}

		bool open() {
			/**
			 * TODO: Create Pipe Universal Client
			 */

			if (!os_open()) {
				return false;
			}

			if (!adjustPipe()) {
				return false;
			}

			return true;
		}

		pipe_impl() {
			ZeroMemory(&writeOperation, sizeof(writeOperation));
			ZeroMemory(&readOperation, sizeof(readOperation));
			underlyingPipe = INVALID_HANDLE_VALUE;
		}

		~pipe_impl() {

			CloseHandle(underlyingPipe);
		}

		bool write(std::string_view data) {
			DWORD written = 0, byteSize = static_cast<DWORD>(data.size()) * sizeof(char);

			bool completed = WriteFile(
				underlyingPipe,
				std::string(data).c_str(),
				byteSize,	// message length
				&written, // bytes written
				&writeOperation);

			if (completed) {
				return true;
			}

			if (GetLastError() != ERROR_IO_PENDING) {
				DWORD transfer = 0;

				completed = GetOverlappedResultEx(
					underlyingPipe,
					&writeOperation,
					&transfer,
					10000,
					false
				);

				if (completed) {
					return true;
				}
			}

			return false;
		}

		std::optional<std::string> read() {
			bool success = false;

			std::string wholeBuffer;

			do {
				std::string buffer;
				buffer.resize(64);

				DWORD bytesRead = 0;

				success = ReadFile(
					underlyingPipe,
					&buffer[0],
					buffer.size() * sizeof(char),
					&bytesRead,
					&readOperation);

				DWORD gle =	GetLastError();

				if (!success && gle != ERROR_MORE_DATA && gle != ERROR_IO_PENDING) {
					return std::nullopt;
				}

				if (gle == ERROR_MORE_DATA) {
					// Really read data, from a sync operation (?)
					buffer.resize(bytesRead);
				}

				if (gle == ERROR_IO_PENDING) {
					DWORD transfer = 0;

					bool res = GetOverlappedResultEx(
						underlyingPipe, 
						&readOperation, 
						&transfer, 
						10000,
						false);

					gle = GetLastError();

					if (!res && gle != ERROR_MORE_DATA) {
						return std::nullopt;
					}
					else {
						// Really read bytes
						buffer.resize(readOperation.InternalHigh);
					}
				}

				wholeBuffer.append(buffer);
			} while (!success); // repeat loop if ERROR_MORE_DATA

			return wholeBuffer;
		}
	};

	bool os_specific::open(std::string_view pipeName) {
		underlyingPipe->changePipeName(pipeName);

		return underlyingPipe->open();
	}

	os_specific::os_specific() : underlyingPipe(new pipe_impl) { }

	os_specific::~os_specific() = default;

	bool os_specific::write(std::string_view pipeData) {
		return underlyingPipe->write(pipeData);
	}

	std::optional<std::string> os_specific::read() {
		return underlyingPipe->read();
	}
}
