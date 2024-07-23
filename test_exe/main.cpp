#include <iostream>
#include <nlohmann/json.hpp>
#include <random>
#include <string>
#include <ts_interface.hpp>

long long genRandomRequestId() {
	std::random_device rd;
	std::mt19937_64 mt(rd());
	std::uniform_int_distribution<long long> uidist;

	return uidist(mt);
}

std::string getCmd() {
	std::string cmd;

	std::cout << "\nCommand: ";

	std::getline(std::cin, cmd);

	return cmd;
}

std::string getPipename() {
	std::string pipename;

	std::cout << "\nPipe name: ";

	std::cin.ignore();

	std::getline(std::cin, pipename);

	return pipename;
}

bool isRawCommand() {
	bool rawCommand = false;

	std::cout << "Raw command? (1/0): ";

	std::cin >> rawCommand;

	return rawCommand;
}

int main() {
	std::cout << "=== MPV Pipe Playground ===\n\n";

	bool rawCommand = false, prefilled = false;

	std::string cmd, pipename;

	while (true) {
		bool refill = true;

		if (prefilled) {
			std::cout << "Seems like you already filled some values, do you want to keep them? (1/0): ";

			bool keep = false;

			std::cin >> keep;

			std::cout << "\n";

			refill = !keep;
		}

		if (refill) {
			rawCommand = isRawCommand();
			pipename = getPipename();
			cmd = getCmd();
		}

		if (!rawCommand) {
			try {
				nlohmann::json dummyObj = nlohmann::json::parse(cmd);
			}
			catch (...) {
				std::cerr << "\nInvalid JSON\n";
				prefilled = false;
				continue;
			}
		}

		prefilled = true;

		const char* ret = sendMPV(cmd.c_str(), genRandomRequestId(), pipename.c_str(), rawCommand);

		std::cout << "Returned from dll: " << ret << "\n";
	}
}