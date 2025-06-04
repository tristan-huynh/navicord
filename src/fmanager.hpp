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
        cout << "File manager initialized." << endl;
    }

    ~fmanager() {
        cout << "File manager destroyed." << endl;
    }

    void parseYAML() {
         cout << "filler";
    }

    string getValue(const string& key) const {
        cout << "filler";
    }
};



