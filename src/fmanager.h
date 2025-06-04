#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>

class fmanager {
public:
    std::string pathForYAML;
    std::unordered_map<std::string, std::string> yamlData;

    fmanager(const std::string& pathForYAML) : pathForYAML(pathForYAML) {
        std::cout << "File manager initialized." << std::endl;
    }

    ~fmanager() {
        std::cout << "File manager destroyed." << std::endl;
    }

    void parseYAML() {
        std::ifstream file(pathForYAML);
        if (!file.is_open()) {
            std::cerr << "Failed to open YAML file." << std::endl;
            return;
        }
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            std::istringstream iss(line);
            std::string key, value;
            if (std::getline(iss, key, ':')) {
                if (std::getline(iss, value)) {
                    // Remove leading/trailing whitespace
                    key.erase(0, key.find_first_not_of(" \t"));
                    key.erase(key.find_last_not_of(" \t") + 1);
                    value.erase(0, value.find_first_not_of(" \t"));
                    value.erase(value.find_last_not_of(" \t") + 1);
                    yamlData[key] = value;
                }
            }
        }
        file.close();
    }

    std::string getValue(const std::string& key) const {
        auto it = yamlData.find(key);
        if (it != yamlData.end()) {
            return it->second;
        }
        return "";
    }
};



