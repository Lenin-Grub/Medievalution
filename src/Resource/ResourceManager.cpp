#include "../stdafx.h"
#include "ResourceManager.hpp"


ResourceManager& ResourceManager::getInstance()
{
    static ResourceManager single_instance;
    return single_instance;
}