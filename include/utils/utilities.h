#pragma once
#include <main.h>

#include <nlohmann/json.hpp>
#include <string>

bool vector_contains(std::vector<std::string> vec, std::string str);
bool is_number(const std::string& s);
bool compare_by_name(const app_entry& a, const app_entry& b);
std::string pad_string_with_spaces(std::string initial, int ending, unsigned int padding_amount);
void print_debug(std::string str);
std::string json_load_value_string(nlohmann::json json, std::string key);
std::string parse_version(std::string version);
void string_replace(std::string& str, const std::string& from, const std::string& to);
std::uint32_t get_battery_percent();
std::string get_battery_status();
std::string digits_string(int value, int digitsCount);
std::string get_time();
std::string get_date();
std::string get_resource_path(std::string str);
std::string to_megabytes(unsigned int size);
std::string to_gigabytes(uint64_t size);
std::string get_free_space();
