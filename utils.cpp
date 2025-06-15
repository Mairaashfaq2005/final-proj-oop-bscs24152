#define _CRT_SECURE_NO_WARNINGS
#include "utils.h"
#include <fstream>
#include <ctime>
using namespace std; 

void log_action(const string& msg) {
    ofstream log_file("log.txt", ios::app);
    if (log_file.is_open()) {
        time_t now = time(0);
        char* dt = ctime(&now);
        log_file << "[" << dt << "] " << msg << endl;
        log_file.close();
    }
}

template <typename T>
void safe_write(const string& filename, T* data, size_t count) {
    try {
        ofstream file(filename, ios::binary);
        if (!file) {
            throw runtime_error("cannot open file for writing");
        }
        file.write(reinterpret_cast<const char*>(data), sizeof(T) * count);
        file.close();
    }
    catch (const exception& e) {
        log_action(string("Error writing file "));
    }
}
template void safe_write<char>(const string&, char*, size_t);

template <typename T>
void safe_read(const string& filename, T* data, size_t count) {
    try {
        ifstream file(filename, ios::binary);
        if (!file) {
            throw runtime_error("cannot open file for reading");
        }
        file.read(reinterpret_cast<char*>(data), sizeof(T) * count);
        file.close();
    }
    catch (const exception& e) {
        log_action(string("Error reading file"));
    }
}
template void safe_read<char>(const string&, char*, size_t);