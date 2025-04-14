#include <iostream>
#include <vector>
#include <filesystem>
#include <conio.h>
#include <fstream>
#include <nlohmann/json.hpp>

#include "Console.h"
#include "PasswordAndKey.h"
#include "Cryptography.h"

namespace fs = std::filesystem;
using json = nlohmann::json;

std::string EncryptPassword(const std::string& password) {
	std::vector<Password> v_Password = ReadPassword();
	std::string pwhash = v_Password[0].hash;
	std::string key = AESKey(pwhash);

	std::string encrypted_pw = aes_e(password, key);
	std::string encrypted_pw_b64 = base64_encode(encrypted_pw);

	return encrypted_pw_b64;
}

std::string DecryptPassword(const std::string& password) {
	std::vector<Password> v_Password = ReadPassword();
	std::string pwhash = v_Password[0].hash;
	std::string key = AESKey(pwhash);

	std::string decrypted_pw_b64 = base64_decode(password);
	std::string decrypted_pw = aes_d(decrypted_pw_b64, key);

	return decrypted_pw;
}

void StockPasswordAndApp(const std::string& app, const std::string& password) {
	std::string path = EnvironmentPath("APPDATA");
	std::string folderpath = path + "\\RC-PWG";
	std::string filepath = folderpath + "\\pwu.json";

	json j;

	if (fs::exists(filepath)) {
		std::ifstream in(filepath);
		if (in.is_open()) {
			try {
				in >> j;
			}
			catch (...) {
				j = json::array();
			}
		}
	}

	if (!j.is_array()) {
		j = json::array();
	}

	int id = j.size() + 1;

	std::string encrypted_pw = EncryptPassword(password);


	json Entry = {
	{"id", id},
	{"app", app },
	{"password", encrypted_pw}
	};

	j.push_back(Entry);

	std::ofstream out(filepath);
	if (out.is_open()) {
		out << j.dump(4);
	}
}

void ReadPasswordAndApp() {
	std::string path = EnvironmentPath("APPDATA");
	std::string folderpath = path + "\\RC-PWG";
	std::string filepath = folderpath + "\\pwu.json";

	if (fs::exists(filepath)) {
		std::ifstream file(filepath);
		if (file.is_open()) {
			json j;
			try {
				file >> j;

				system("cls");
				Tag();
				for (const auto& i : j) {
					std::string app = i["app"];
					std::string password = i["password"];
					
					std::string decrypted_pw = DecryptPassword(password);

					int id = i["id"];
					std::cout << "\33[38;5;160m ┌──" << "\33[38;5;15m" << id << "\n";
					std::cout << " \33[38;5;160m ┌──\33[38;5;15mApp:" + app + "\n";
					std::cout << " \33[38;5;160m └──\33[38;5;15mPassword:" + decrypted_pw + "\n";
				}
				std::cout << " \n\n\n\n\33[38;5;160m ┌──Press enter\n └──*\33[38;5;15m";
				while (true) {
					char c = _getch();
					if (c == '\r') {
						break;
					}
				}
			}
			catch (...) {

			}
		}
	}
}

void AddPasswordAndApp() {
	while (true) {
		system("cls");
		Tag();

		std::string app;
		std::string password;

		std::cout << " \33[38;5;160m───\33[38;5;15mFor which application will this password be\n\n";
		std::cout << "   \33[38;5;160m*\33[38;5;15m";
		std::cin >> app;

		system("cls");
		Tag();

		std::cout << " \33[38;5;160m┌──\33[38;5;15mFor which application will this password be\n \33[38;5;160m└──\33[38;5;15m" + app + "\n\n";
		std::cout << " \33[38;5;160m───\33[38;5;15mSet the password\n\n";
		std::cout << "   \33[38;5;160m*\33[38;5;15m";
		std::cin >> password;

		system("cls");
		Tag();

		std::cout << " \33[38;5;160m┌──\33[38;5;15mFor which application will this password be\n \33[38;5;160m└──\33[38;5;15m" + app + "\n\n";
		std::cout << " \33[38;5;160m┌──\33[38;5;15mSet the password\n \33[38;5;160m└──\33[38;5;15m" + password + "\n\n\n\n";

		std::cout << " \33[38;5;160m┌──(Save this or not ? (y/n))\n └──*\33[38;5;15m";

		char choice;
		std::cin >> choice;
		if (choice == 'y') {
			StockPasswordAndApp(app, password);
			MainMenu();
		}
		else {
			continue;
		}
	}

}

void ModifyPasswordAndApp() {
	std::string path = EnvironmentPath("APPDATA");
	std::string folderpath = path + "\\RC-PWG";
	std::string filepath = folderpath + "\\pwu.json";

	if (fs::exists(filepath)) {
		std::ifstream file(filepath);
		if (file.is_open()) {
			json j;
			try {
				file >> j;
				system("cls");

				Tag();
				for (const auto& i : j) {
					std::string app = i["app"];
					std::string password = i["password"];
					int id = i["id"];

					std::string decrypted_pw = DecryptPassword(password);

					std::cout << "\33[38;5;160m ┌──" << "\33[38;5;15m" << id << "\n";
					std::cout << " \33[38;5;160m ┌──\33[38;5;15mApp:" + app + "\n";
					std::cout << " \33[38;5;160m └──\33[38;5;15mPassword:" + decrypted_pw + "\n";
				}
				std::cout << " \n\n\n\n\33[38;5;160m ┌──Select the application or password to be modified with its id\n └──*\33[38;5;15m";
				int choice;

				std::cin >> choice;
				
				bool detect;

				for (auto& i : j) {
					int id = i["id"];
					if (id == choice) {
						detect = true;

						system("cls");
						Tag();

						std::string app = i["app"];
						std::string password = i["password"];

						std::string decrypted_pw = DecryptPassword(password);

						std::string newapp = "";
						std::string newpassword = "";

						std::cout << "\33[38;5;160m ┌──Modify App(" << "\33[38;5;15m" << app << "\33[38;5;160m)\33[38;5;15m" << "\n";
						std::cout << "\33[38;5;160m └──*" << "\33[38;5;15m";
						std::cin >> newapp;
						std::cout << "\n";

						std::cout << "\33[38;5;160m ┌──Modify Password(" << "\33[38;5;15m" << decrypted_pw << "\33[38;5;160m)\33[38;5;15m" << "\n";
						std::cout << "\33[38;5;160m └──*" << "\33[38;5;15m";
						std::cin >> newpassword;
						std::cout << "\n\n\n\n";

						std::string encrypted_pw = EncryptPassword(newpassword);

						i["app"] = newapp;
						i["password"] = encrypted_pw;

						std::ofstream out(filepath);
						if (out.is_open()) {
							out << j.dump(4);
						}
						break;
					}
				}
			}
			catch (...) {
				
			}
			ReadPasswordAndApp();
		}
	}
}

void DeletePasswordAndApp() {
	std::string path = EnvironmentPath("APPDATA");
	std::string folderpath = path + "\\RC-PWG";
	std::string filepath = folderpath + "\\pwu.json";

	if (fs::exists(filepath)) {
		std::ifstream file(filepath);
		if (file.is_open()) {
			json j;
			try {
				file >> j;
				system("cls");

				Tag();
				for (const auto& i : j) {
					std::string app = i["app"];
					std::string password = i["password"];
					int id = i["id"];

					std::string decrypted_pw = DecryptPassword(password);

					std::cout << "\33[38;5;160m ┌──" << "\33[38;5;15m" << id << "\n";
					std::cout << " \33[38;5;160m ┌──\33[38;5;15mApp:" + app + "\n";
					std::cout << " \33[38;5;160m └──\33[38;5;15mPassword:" + decrypted_pw + "\n";
				}
				std::cout << " \n\n\n\n\33[38;5;160m ┌──Select the application or password to be deleted with its id\n └──*\33[38;5;15m";
				int choice;

				std::cin >> choice;

				json updated = json::array();
				bool detect = false;

				for (auto& i : j) {
					int id = i["id"];
					if (id != choice) {
						updated.push_back(i);
					}
					else {
						detect = true;
					}
				}

				if (detect) {
					int id = 1;
					for (auto& i : updated) {
						i["id"] = id++;
					}

					std::ofstream out(filepath);
					if (out.is_open()) {
						out << updated.dump(4);
					}
				}
			}
			catch (...) {

			}
			ReadPasswordAndApp();
		}
	}
}