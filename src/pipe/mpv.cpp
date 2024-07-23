#include <pipe/mpv.hpp>

namespace pipe {
	mpv::mpv(std::string_view pipeProvided) {
		ownPipe = std::make_unique<os_specific>();
		isOpen = ownPipe->open(pipeProvided);
	}

	mpv::mpv(mpv&& obj) {
		ownPipe = std::move(obj.ownPipe);
	}

	bool mpv::write(std::string_view data) {
		std::string nlEnd;

		nlEnd.append(data);

		if (data.back() != '\n') {
			nlEnd.append("\n");
		}

		return ownPipe->write(nlEnd);
	}

	std::optional<std::string> mpv::waitResponse(int64_t request_id) {
		do {
			std::optional<nlohmann::json> msg = peekMsg();

			if (!msg.has_value()) {
				return std::nullopt;
			}

			nlohmann::json requestIdObj = msg.value();

			if (!requestIdObj.contains("request_id")) {
				continue;
			}

			if (requestIdObj["request_id"] == request_id) {
				return requestIdObj.dump();
			}

			// Otherwise keep waiting other messages to get
		} while(true);
	}

	std::optional<nlohmann::json> mpv::peekMsg() {
		std::optional<std::string> msg = ownPipe->read();

		if (!msg.has_value()) {
			return std::nullopt;
		}

		nlohmann::json jsonObj;

		try {
			jsonObj = nlohmann::json::parse(msg.value());
		}
		catch (...) {
			return std::nullopt;
		}
		
		return jsonObj;
	}

	bool mpv::isPipeOpen()
	{
		return isOpen;
	}
}