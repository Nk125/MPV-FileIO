#pragma once
#include <memory>
#include <optional>
#include <string>
#include <string_view>

namespace pipe {
	/**
 	 * @brief Use pImpl pattern to speed-up compilation times due to heavy-bloated system header files
 	 */
	class os_specific {
	public:
		os_specific();

		~os_specific();

		/**
		 * @brief Open the pipe
		 * @param Name The name of the pipe
		 * @return True if the pipe is opened
		 */
		bool open(std::string_view);

		/**
		 * @brief Write data to the opened pipe
		 * @param Data The data to write
		 * @return True if the data wrote succesfully
		 */
		bool write(std::string_view);

		/**
		 * @brief Reads the last messages from the pipe
		 * @return The read message, std::nullopt in error
		 */
		std::optional<std::string> read();
	
	private:
		class pipe_impl;
		
		std::unique_ptr<pipe_impl> underlyingPipe;
	};
}