#pragma once
#include <memory>
#include <set>

template <typename BaseType, typename SubType>
SubType* TakeOwnershipFromUniquePtr(std::set<std::unique_ptr<BaseType>>& container, std::unique_ptr<SubType> object)
{
	auto reference = object.get();
	container.emplace(std::forward<std::unique_ptr<BaseType>>(object));
	return reference;
}

template <typename BaseType, typename SubType>
bool RemoveFromUniqueSet(std::set<std::unique_ptr<BaseType>>& container, const SubType* object)
{
	if(object)
	{
		auto comparison = [object](const std::unique_ptr<BaseType>& objectIndex)
		{
			return (objectIndex.get() == object);
		};

		auto foundObj = std::find_if(std::begin(container), std::end(container), comparison);
		if(foundObj != std::end(container))
		{
			container.erase(foundObj);
			return true;
		}
	}

	return false;
}
