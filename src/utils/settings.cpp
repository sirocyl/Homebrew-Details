#include <utils/launching.h>
#include <utils/nacp_utils.h>
#include <utils/reboot_to_payload.h>
#include <utils/settings.h>
#include <utils/utilities.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

std::string settings[settings_num + 1];

std::string config_path = "sdmc:/config/homebrew_details/";

void file_load_settings()
{
    std::ifstream inputFile(config_path + "config.txt");
    if (inputFile)
    {
        int index = 0;
        while (inputFile)
        {
            char line[513];
            inputFile.getline(line, 512);
            settings[index] = line;
            print_debug(line);
            print_debug("\n");
            index += 1;
        }
        inputFile.close();
    }
    else
        print_debug("Can't find loadfile.\n");
}

void file_save_settings()
{
    if (!fs::exists("sdmc:/config/"))
        fs::create_directory("sdmc:/config/");
    if (!fs::exists("sdmc:/config/homebrew_details/"))
        fs::create_directory("sdmc:/config/homebrew_details/");

    remove((config_path + "config.txt").c_str());
    std::ofstream outputFile(config_path + "config.txt");
    if (outputFile)
    {
        int index = 0;
        while (index < settings_num)
        {
            outputFile << settings[index].c_str();
            if (index != settings_num - 1)
                outputFile << std::endl;
            index += 1;
        }
        outputFile.close();
    }
    else
        print_debug("Can't open savefile.\n");
}

void set_setting(int setting, std::string value)
{
    settings[setting] = base64_encode(value);
    print_debug(("Set " + std::to_string(setting) + " to " + value + "\n").c_str());
    file_save_settings();
}

std::string get_setting(int setting)
{
    return base64_decode(settings[setting]);
}

bool get_setting_true(int setting)
{
    return (get_setting(setting) == "true");
}

void init_settings()
{
    if (get_setting(setting_search_subfolders) == "")
    {
        set_setting(setting_search_subfolders, "true");
    }

    if (get_setting(setting_search_root) == "")
    {
        set_setting(setting_search_root, "false");
    }

    if (get_setting(setting_scan_full_card) == "")
    {
        set_setting(setting_scan_full_card, "false");
    }

    if (get_setting(setting_autoscan) == "")
    {
        set_setting(setting_autoscan, "false");
    }

    if (get_setting(setting_debug) == "")
    {
        set_setting(setting_debug, "false");
    }

    if (get_setting_true(setting_debug))
        set_setting(setting_local_version, std::string(APP_VERSION) + "d");
    else
        set_setting(setting_local_version, APP_VERSION);

    if (get_setting(setting_control_scheme) == "")
    {
        set_setting(setting_control_scheme, "0");
    }

    if (get_setting(setting_lax_store_compare) == "")
    {
        set_setting(setting_lax_store_compare, "false");
    }

    if (get_setting(setting_scan_settings_changed) == "")
    {
        set_setting(setting_scan_settings_changed, "true");
    }

    if (get_setting(setting_previous_num_files) == "")
    {
        set_setting(setting_previous_num_files, "1");
    }
}
