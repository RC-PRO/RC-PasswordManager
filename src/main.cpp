#include <Windows.h>
#include <iostream>
#include <filesystem>
#include <fstream>

#include "Console.h"
#include "PasswordAndKey.h"
#include "UUID.h"

namespace fs = std::filesystem;

void Begin() {
	std::string path = EnvironmentPath("APPDATA");
	std::string folderpath = path + "\\RC-PWG";
	std::string filepath = folderpath + "\\pw.json";

	if (!fs::exists(filepath)) {
		RegisterMenu();
	}
	else {
		LoginMenu();
	}
}

int main(){

	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

	Begin();

}
