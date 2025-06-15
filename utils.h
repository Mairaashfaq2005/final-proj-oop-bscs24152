#pragma once
#include <string>

void log_action(const std::string& msg);

template <typename T>
void safe_write(const std::string& filename, T* data, size_t count);

template <typename T>
void safe_read(const std::string& filename, T* data, size_t count);
