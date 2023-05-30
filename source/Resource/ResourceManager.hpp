#pragma once
class ResourceManager
{
public:
    ResourceManager();
    ~ResourceManager();

    template<class T>
    static T* get(const std::string& name, bool remove = false);
};
#include "ResourceManager.inl"