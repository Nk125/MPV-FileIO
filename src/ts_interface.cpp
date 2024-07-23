#include <ts_interface.hpp>
#include <nlohmann/json.hpp>
#include <pipe/mpv.hpp>
#include <vector>

extern "C" MPV_WRAPPER const char* sendMPV(const char* data, long long request_id, const char* pipeName, bool rawCommand) {
	pipe::mpv mpv(pipeName);

	static std::string resStr;

	if (!mpv.isPipeOpen()) {
		return "";
	}

	if (rawCommand) {
		mpv.write(data);
		return "";
	}

	nlohmann::json jsonObj;

	try {
		// Checks if the data is a valid json document
		jsonObj = nlohmann::json::parse(data);
	}
	catch (...) {
		return "";
	}

	// Replaces the request_id with the provided in the function
	jsonObj["request_id"] = request_id;

	mpv.write(jsonObj.dump());

	resStr.clear();

	std::optional<std::string> response = mpv.waitResponse(request_id);

	resStr = response.value_or(std::string());

	return resStr.c_str();
}