#include "../stdafx.h"
#include "ResourceManager.hpp"
#include "Files.hpp"

#include <unordered_map>

template<class T>
T* ResourceManager::get(const std::string& name, bool remove) 
{
	static std::unordered_map<std::string, T> resources;

	if (remove)
	{
		auto found = resources.find(name);

		if(found != resources.end())	
			resources.erase(found);

		return nullptr;
	}

	if(name.empty())
		return nullptr;

	auto found = resources.find(name);

	if(found != resources.end())	
		return &found->second;

	std::string filepath = FileUtils::getPathToFile(name);

	if( ! filepath.empty() )
	{
		auto [iterator, result] = resources.try_emplace(name);

		if(result)
		{
			if( ! iterator->second.loadFromFile(filepath) )
			{
				resources.erase(name);
				return nullptr;	
			}
		}
		return &iterator->second;
	}

	return nullptr;
}