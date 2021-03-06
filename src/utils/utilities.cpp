#include <string.h>
#include <switch.h>
#include <sys/statvfs.h>
#include <utils/settings.h>
#include <utils/utilities.h>

#include <algorithm>
#include <nlohmann/json.hpp>
#include <pages/main_page.hpp>
#include <string>

#include "switch/services/psm.h"

std::string months[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

std::string json_load_value_string(nlohmann::json json, std::string key)
{
    if (json.contains(key))
        return json[key].get<std::string>();
    else
        return "---";
}

std::string parse_version(std::string version)
{
    if (!version.empty())
    {
        if (version.at(version.length() - 1) == 'd')
            version = version.substr(0, version.length() - 1);
        if (version.at(0) == 'v')
            version = version.substr(1);
    }

    if (!version.empty())
        return version;
    else
        return "0";
}

bool is_number(const std::string& s)
{
    return (s.length() > 0 && strspn(s.c_str(), "-.0123456789") == s.size());
}

bool vector_contains(std::vector<std::string> vec, std::string str)
{
    if (vec.empty())
        return false;
    else
        return (std::find(vec.begin(), vec.end(), str) != vec.end());
}

bool compare_by_name(const app_entry& a, const app_entry& b)
{
    std::string _a = a.name;
    transform(_a.begin(), _a.end(), _a.begin(), ::tolower);
    std::string _b = b.name;
    transform(_b.begin(), _b.end(), _b.begin(), ::tolower);

    if (a.favorite == b.favorite)
    {
        if (_a != _b)
            return (_a.compare(_b) < 0);
        else
            return (a.version.compare(b.version) > 0);
    }
    else
    {
        if (a.favorite)
            return true;
        else
            return false;
    }
}

std::string pad_string_with_spaces(std::string initial, int ending, unsigned int padding_amount)
{
    std::string str = "(" + std::to_string(ending) + ")";
    while ((str.length()) < padding_amount)
        str = " " + str;
    str = initial + str;
    return str;
}

void print_debug(std::string str)
{
    if (get_setting_true(setting_debug))
    {
        str = "[DETAILS] " + str;
        printf(str.c_str());
    }
}

void string_replace(std::string& str, const std::string& from, const std::string& to)
{
    if (from.empty())
        return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

std::uint32_t get_battery_percent()
{
    std::uint32_t batteryCharge = 0;
    psmGetBatteryChargePercentage(&batteryCharge);
    return batteryCharge;
}

std::string get_battery_status()
{
    ChargerType chargerType;
    // Not Charging, Charging Via Power, Charging via USB
    std::string chargerTypes[3] = { std::string(""), std::string(" Charging"), std::string(" via USB") };
    psmGetChargerType(&chargerType);
    // Error by Default
    std::string chargeStatus = "";
    if ((int)chargerType >= 0 && (int)chargerType < 3)
        chargeStatus = chargerTypes[chargerType];
    return chargeStatus;
}

std::string digits_string(int value, int numDigits)
{
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(numDigits) << value;
    return oss.str();
}

std::string get_time()
{
    auto t  = std::time(nullptr);
    auto tm = *std::localtime(&t);
    return (digits_string(tm.tm_hour, 2) + ":" + digits_string(tm.tm_min, 2) + ":" + digits_string(tm.tm_sec, 2));
}

std::string get_date()
{
    auto t  = std::time(nullptr);
    auto tm = *std::localtime(&t);
    return (months[tm.tm_mon] + " " + digits_string(tm.tm_mday, 2) + ", " + std::to_string(1900 + tm.tm_year));
}

std::string get_resource_path(std::string str)
{
    return "sdmc:/config/homebrew_details/assets/" + str;
}

std::string to_megabytes(unsigned int size)
{
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << (size / 1024. / 1024.);
    std::string str = stream.str();
    return str;
}

std::string to_gigabytes(uint64_t size)
{
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << (size / 1024. / 1024. / 1024.);
    std::string str = stream.str();
    return str;
}

std::string get_free_space()
{
    struct statvfs st;
    if (::statvfs("sdmc:/", &st) != 0)
    {
        return "-1";
    }
    else
    {
        uint64_t freeSpace = static_cast<std::uint64_t>(st.f_bsize) * st.f_bfree;
        return (to_gigabytes(freeSpace) + " GB\n");
    }
}