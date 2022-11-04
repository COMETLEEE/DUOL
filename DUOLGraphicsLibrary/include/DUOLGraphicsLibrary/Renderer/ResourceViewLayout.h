#pragma once
#include <vector>
#include "DUOLGraphicsLibrary/ResourceFlags.h"



namespace DUOLGraphicsLibrary
{
	class Sampler;
	class Buffer;
	class Texture;

	class ResourceViewLayout
	{
	public:
		ResourceViewLayout(const std::vector<ResourceViewDesc>& resourceViewDesc);

	public:
		std::vector<ResourceViewDesc> _resourceViews;
	};
}