#pragma once
#include <memory>
#include <unordered_map>
#include <set>

#include "DUOLGraphicsLibrary/EntityBase.h"

//template <typename BaseType, typename SubType>
//SubType* TakeOwnershipFromUniquePtr(std::set<std::unique_ptr<BaseType>>& container, std::unique_ptr<SubType> object)
//{
//	auto reference = object.get();
//	container.emplace(std::forward<std::unique_ptr<BaseType>>(object));
//	return reference;
//}
//
//template <typename BaseType, typename SubType>
//bool RemoveFromUniqueSet(std::set<std::unique_ptr<BaseType>>& container, const SubType* object)
//{
//	if(object)
//	{
//		auto comparison = [object](const std::unique_ptr<BaseType>& objectIndex)
//		{
//			return (objectIndex.get() == object);
//		};
//
//		auto foundObj = std::find_if(std::begin(container), std::end(container), comparison);
//		if(foundObj != std::end(container))
//		{
//			container.erase(foundObj);
//			return true;
//		}
//	}
//
//	return false;
//}

template <typename BaseType, typename SubType>
SubType* TakeOwnershipFromUniquePtr(const unsigned __int64& id, std::unordered_map<unsigned __int64, std::unique_ptr<BaseType>>& container, std::unique_ptr<SubType> object)
{

	auto foundObject = container.find(id);

	if (foundObject != container.end())
	{
		//���ο�ɷ� �ٲ��ٱ�.. �ƴϸ� ������ ���ٱ�..
		return static_cast<SubType*>(foundObject->second.get());
	}
	else
	{
		static_assert(std::is_base_of_v<DUOLGraphicsLibrary::EntityBase, SubType>, "SubType must inherited from EntityBase");

		auto reference = object.get();

		reference->SetGUID(id);//���������� �ϴ� ��� ������Ʈ���� entitybase�� ����ϰ������Ƿ�..

		container.emplace(id, std::forward<std::unique_ptr<SubType>>(object));

		return reference;
	}
}

template <typename BaseType, typename SubType>
SubType* TakeOwnershipFromUniquePtrWithMutex(const unsigned __int64& id, std::unordered_map<unsigned __int64, std::unique_ptr<BaseType>>& container, std::unique_ptr<SubType> object)
{
	static std::mutex mutex1;

	mutex1.lock();
	auto foundObject = container.find(id);
	auto endObject = container.end();
	mutex1.unlock();

	if (foundObject != endObject)
	{
		//���ο�ɷ� �ٲ��ٱ�.. �ƴϸ� ������ ���ٱ�..
		return static_cast<SubType*>(foundObject->second.get());
	}
	else
	{
		static_assert(std::is_base_of_v<DUOLGraphicsLibrary::EntityBase, SubType>, "SubType must inherited from EntityBase");

		auto reference = object.get();

		reference->SetGUID(id);//���������� �ϴ� ��� ������Ʈ���� entitybase�� ����ϰ������Ƿ�..

		mutex1.lock();
		container.emplace(id, std::forward<std::unique_ptr<SubType>>(object));
		mutex1.unlock();
		return reference;
	}
}

template <typename BaseType>
bool RemoveFromUniqueMap(std::unordered_map<unsigned __int64, std::unique_ptr<BaseType>>& container, const unsigned __int64& id)
{
	container.erase(id);

	return true;
}
