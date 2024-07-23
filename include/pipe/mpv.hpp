#pragma once
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <pipe/os_specific.hpp>
#include <string_view>
#include <string>

namespace pipe {
	class mpv {
	public:
		/**
		 * @brief Checks if pipe is open
		 * @return True if the pipe is open
		 */
		bool isPipeOpen();

		mpv(const mpv&) = delete;

		/**
		 * @brief Move MPV's pipe to another object
		 */
		mpv(mpv&&);

		/**
		 * @brief Open MPV's pipe
		 * @param PipeName The name of the pipe to open
		 * @note To preserve a single pipe across more time span, use a unique_ptr
		 */
		mpv(std::string_view);

		/**
		 * @brief Close pipe connection
		 * @details The underlying pipe type closes itself at the object destruction
		 */
		~mpv() = default;

		/**
		 * @brief Write raw data to the MPV pipe
		 * @param Data The data to write
		 */
		bool write(std::string_view);

		/**
		 * @brief Wait the response from the pipe
		 * @param Request_ID The request ID to wait for
		 * @return The response from the pipe, if failed return std::nullopt
		 */
		std::optional<std::string> waitResponse(int64_t);

	private:
		std::unique_ptr<os_specific> ownPipe;

		bool isOpen = false;

		std::optional<nlohmann::json> peekMsg();
	};
}