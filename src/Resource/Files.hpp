#pragma once
#include "../stdafx.h"
#include "../Common/Common.h"

struct FileUtils
{
    static std::string getPathToFile(const std::string& filename, const std::string& filetype);
};