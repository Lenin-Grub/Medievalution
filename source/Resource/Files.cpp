#include "../stdafx.h"
#include "Files.hpp"


std::string FileUtils::getPathToFile(const std::string& filename, const std::string& filetype)
{   
    static const std::string recource_folder("resources/");

    for (auto& file : std::filesystem::recursive_directory_iterator(recource_folder))
    {
        if (file.path().stem().string() == filename && file.path().stem().extension().string() == filetype)
        {
            std::string filepath = file.path().string();
            std::replace(filepath.begin(), filepath.end(), '\\', '/');
            return filepath;
        }
        return {};
    }
}