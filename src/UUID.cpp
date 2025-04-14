#include <windows.h>
#include <sstream>

std::string GetUUID() {
	FILE* pipe = _popen("powershell -Command \"(Get-CimInstance Win32_ComputerSystemProduct).UUID\"", "r");
	if (!pipe) {
		return "";
	}

	char buffer[128];
	std::string result = "";

	while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
		result += buffer;
	}
	_pclose(pipe);

	size_t pos = result.find("UUID");
	if (pos != std::string::npos) {
		result = result.substr(pos + 4);
	}

	result.erase(std::remove_if(result.begin(), result.end(), ::isspace), result.end());
	return result;
}