#include <iostream>
#include <conio.h>
#include <nlohmann/json.hpp>

#include "Cryptography.h"
#include "PasswordAndKey.h"
#include "Password.h"

using json = nlohmann::json;

void Tag() {
	std::cout <<
		"\33[38;5;160m"
		"██████╗  ██████╗              ██████╗ ██╗    ██╗███╗   ███╗\n"
		"██╔══██╗██╔════╝              ██╔══██╗██║    ██║████╗ ████║\n"
		"██████╔╝██║         █████╗    ██████╔╝██║ █╗ ██║██╔████╔██║\n"
		"██╔══██╗██║         ╚════╝    ██╔═══╝ ██║███╗██║██║╚██╔╝██║\n"
		"██║  ██║╚██████╗              ██║     ╚███╔███╔╝██║ ╚═╝ ██║\n"
		"╚═╝  ╚═╝ ╚═════╝              ╚═╝      ╚══╝╚══╝ ╚═╝     ╚═╝\33[0m\n\n\n\n";
}

void MainMenu() {
	int choice;

	while (true) {
		system("cls");
		Tag();

		std::cout <<
			"\33[38;5;160m  ┌──\33[38;5;15m [1] Add a password\n" <<
			" \33[38;5;160m ├──\33[38;5;15m [2] Delete a password\n" <<
			" \33[38;5;160m ├──\33[38;5;15m [3] Modify a password\n" <<
			" \33[38;5;160m ├──\33[38;5;15m [4] Show all passwords\n" <<
			" \33[38;5;160m └──\33[38;5;15m [5] Quit\n\n\n\n";

		std::cout << "  \33[38;5;160m*\33[38;5;15m";

		std::cin >> choice;

		if (choice == 1) {
			AddPasswordAndApp();
		}
		else if (choice == 2) {
			DeletePasswordAndApp();
		}
		else if (choice == 3) {
			ModifyPasswordAndApp();
		}
		else if (choice == 4) {
			ReadPasswordAndApp();
		}
		else if (choice == 5) {
			exit(0);
		}
		else {
			continue;
		}
	}
}


void RegisterMenu() {
	std::string password = "";

	Tag();

	std::cout <<
		" \33[38;5;160m┌──\33[38;5;15m Welcome, please create a password to access the password manager\n" <<
		" \33[38;5;160m│\n" <<
		" └──\33[38;5;15m Password: ";

	std::cin.ignore();
	std::getline(std::cin, password);

	std::string salt = generate_salt();
	std::string hash = sha256(password + salt);

	StockPassword(hash, salt);

	MainMenu();
}

void LoginMenu() {
	while (true) {
		system("cls");
		std::string password = "";

		Tag();

		std::cout <<
			" \33[38;5;160m┌──\33[38;5;15m Welcome, please enter your password to access the password manager\n" <<
			" \33[38;5;160m│\n" <<
			" └──\33[38;5;15m Password: ";

		std::getline(std::cin, password);

		std::vector<Password> v_Password = ReadPassword();
		std::string j_hash = v_Password[0].hash;
		std::string j_salt = v_Password[0].salt;

		std::string hash = sha256(password + j_salt);

		if (hash == j_hash) {
			MainMenu();
		}
		else {
			std::cout << "\33[38;5;160m │\n";
			std::cout << " └── Wrong password, press enter\n\n";
			std::cout << " *";
			while (true) {
				char c = _getch();
				if (c == '\r') {
					break;
				}
			}
			continue;
		}
	}
}


