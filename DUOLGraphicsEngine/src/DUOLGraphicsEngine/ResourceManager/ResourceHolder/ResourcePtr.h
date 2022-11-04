#pragma once
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"

namespace DUOLGraphicsLibrary
{
	class Renderer;
}

namespace DUOLGraphicsEngine
{
	//template <typename T>
	//class ResourceDeleter
	//{
	//public:
	//	inline void operator()(const DUOLGraphicsLibrary::Renderer* _renderer, T* ptr)
	//	{
	//		_renderer->Release(ptr);
	//		ptr = nullptr;
	//	}
	//};

	//template <typename T>
	//class ResourcePtr 
	//{
	//public:
	//	ResourcePtr(const DUOLGraphicsLibrary::Renderer* renderer, T* ptr)
	//		:_ref(0)
	//	{
	//		auto deleter = std::bind(ResourceDeleter<T>(), renderer, std::placeholders::_1);

	//		_ptr = std::make_shared<T, deleter>();
	//	}

	//	~ResourcePtr()
	//	{

	//	}

	//private:
	//	std::shared_ptr<T> _ptr;

	//	unsigned int _ref;
	//};
}

