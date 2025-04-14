#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

#include "Console.h"
#include "Cryptography.h"

namespace fs = std::filesystem;
using json = nlohmann::json;

std::string EnvironmentPath(const std::string& variable) {
	char* buffer = nullptr;
	size_t length = 0;

	if (_dupenv_s(&buffer, &length, variable.c_str()) == 0 && buffer != nullptr) {
		std::string path = std::string(buffer);
		free(buffer);
		return path;
	}
	return "";
}

void StockPassword(const std::string& hash, const std::string& salt) {
	std::string path       = EnvironmentPath("APPDATA");
	std::string folderpath = path + "\\RC-PWG";
	std::string filepath   = folderpath + "\\pw.json";

	if (!fs::exists(folderpath)) {
		fs::create_directories(folderpath);
	}

	if (!fs::exists(filepath)) {

		json j;
		j["hash"] = hash;
		j["salt"] = salt;

		std::ofstream file(filepath);
		if (file.is_open()) {
			file << j.dump(4);
			file.close();
		}
	}
}

struct Password {
	std::string hash;
	std::string salt;
};

std::vector<Password> ReadPassword() {
	std::string path = EnvironmentPath("APPDATA");
	std::string folderpath = path + "\\RC-PWG";
	std::string filepath = folderpath + "\\pw.json";

	if (fs::exists(filepath)) {
		std::ifstream file(filepath);
		std::vector<Password> v_Password;

		if (file.is_open()) {
			json j;
			file >> j;

			std::string hash = j["hash"];
			std::string salt = j["salt"];

			file.close();

			v_Password.push_back({ hash, salt });
			return v_Password;
		}

	}
	std::cout << "Failed hash recovery";
	std::cin.get();
}