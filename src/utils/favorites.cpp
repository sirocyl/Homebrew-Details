#include <main.h>
#include <utils/base64.h>
#include <utils/favorites.h>
#include <utils/utilities.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

void read_favorites()
{
    std::ifstream inputFile("sdmc:/config/homebrew_details/favorites.txt");
    if (inputFile)
    {
        int index = 0;
        while (inputFile)
        {
            char line[513];
            inputFile.getline(line, 512);
            favorites.push_back(base64_decode(line));
            print_debug(line);
            print_debug("\n");
            index += 1;
        }
        inputFile.close();
    }
    else
        print_debug("Can't find favorites file.\n");
}

void write_favorites()
{
    if (!fs::exists("sdmc:/config/"))
        fs::create_directory("sdmc:/config/");
    if (!fs::exists("sdmc:/config/homebrew_details/"))
        fs::create_directory("sdmc:/config/homebrew_details/");

    remove("sdmc:/config/homebrew_details/favorites.txt");

    if (!favorites.empty())
    {
        std::ofstream outputFile("sdmc:/config/homebrew_details/favorites.txt");
        if (outputFile)
        {
            unsigned int index = 0;
            while (index < favorites.size())
            {
                outputFile << (base64_encode(favorites.at(index))).c_str();
                if (index != favorites.size() - 1)
                    outputFile << std::endl;
                index += 1;
            }
            outputFile.close();
        }
        else
            print_debug("Can't open favorites.\n");
    }
}

void add_favorite(std::string str)
{
    favorites.push_back(str);
    write_favorites();
}

void remove_favorite(std::string str)
{
    favorites.erase(std::remove(favorites.begin(), favorites.end(), str), favorites.end());
    write_favorites();
}