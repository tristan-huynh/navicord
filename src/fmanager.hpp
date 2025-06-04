#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>

using namespace std;

class fmanager {
public:
    string pathForYAML;
    unordered_map<string, string> yamlData;
    fmanager(const string& pathForYAML) : pathForYAML(pathForYAML) {
        cout << "File manager initialized with yaml: " << pathForYAML << endl;
    }

    ~fmanager() {
        cout << "File manager destroyed." << endl;
    }

    void parseYAML() {
        ifstream file(pathForYAML);
        if (!file.is_open()) {
            cerr << "Error opening file: " << pathForYAML << endl;
            return;
        }

        auto trim = [](const string &s) -> string {
            size_t start = s.find_first_not_of(" \t");
            size_t end = s.find_last_not_of(" \t");
            return (start == string::npos) ? "" : s.substr(start, end - start + 1);
        };

        string line;
        while (getline(file, line)) {
            // Remove inline comments
            size_t commentPos = line.find('#');
            if(commentPos != string::npos)
                line = line.substr(0, commentPos);

            line = trim(line);
            if(line.empty())
                continue;

            size_t pos = line.find(':');
            if (pos == string::npos)
                continue;

            string key = trim(line.substr(0, pos));
            string value = trim(line.substr(pos + 1));

            if (!key.empty() && !value.empty())
                yamlData[key] = value;
        }
    }

    string getValue(const string& key) const {
        auto it = yamlData.find(key);
        return (it != yamlData.end()) ? it->second : "";
    }
};



