#include "Config.hpp"

#include <fstream>
#include <sstream>

namespace Logger {

bool Config::load(std::string const &filename) {
  std::string line;
  std::string key;
  std::string value;

  std::ifstream file(filename);
  if (!file.is_open()) return (false);
  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') continue;
    std::istringstream iss(line);
    if (std::getline(iss, key, '=') && std::getline(iss, value)) {
      key.erase(0, key.find_first_not_of(" \t"));
      key.erase(key.find_last_not_of(" \t") + 1);
      value.erase(0, value.find_first_not_of(" \t"));
      value.erase(value.find_last_not_of(" \t") + 1);
      data_[key] = value;
    }
  }
  return (true);
}

std::string Config::get(std::string const &key,
                        std::string const &defaultValue) const {
  auto it = data_.find(key);
  return ((it != data_.end()) ? it->second : defaultValue);
}

int Config::getInt(std::string const &key, int defaultValue) const {
  std::string value;

  value = get(key, "");
  if (value.empty()) return (defaultValue);
  for (char ch : value) {
    if (!std::isdigit(static_cast<unsigned char>(ch))) return (defaultValue);
  }
  return (std::stoi(value));
}

}  // namespace Logger