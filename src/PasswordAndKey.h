#include <iostream>

std::string EnvironmentPath(const std::string& variable);

void StockPassword(const std::string& hash, const std::string& salt);

struct Password {
	std::string hash;
	std::string salt;
};

std::vector<Password> ReadPassword();