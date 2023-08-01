#include "../stdafx.h"
#include "Files.hpp"


std::string FileUtils::getPathToFile(const std::string& filename)
{   
    static const std::string texture_folder("resources/");

    for (auto& file : std::filesystem::recursive_directory_iterator(texture_folder))
        if (file.path().stem().string() == filename)
        {
            std::string filepath = file.path().string();
            std::replace(filepath.begin(), filepath.end(), '\\', '/');
            return filepath;
        }           
    return {};
}