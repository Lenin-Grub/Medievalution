#pragma once
#include "Files.hpp"

class ResourceManager
{
public:

    template<class T>
    static T* get(const std::string& name, bool remove = false);
};
#include "ResourceManager.inl"