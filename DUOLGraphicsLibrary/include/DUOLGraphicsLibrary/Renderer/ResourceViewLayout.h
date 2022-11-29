#pragma once
#include <vector>
#include "DUOLGraphicsLibrary/ResourceFlags.h"

namespace DUOLGraphicsLibrary
{
	class Sampler;
	class Buffer;
	class Texture;

	class DUOLGRAPHICSLIBRARY_EXPORT ResourceViewLayout
	{
	public:
		ResourceViewLayout(const std::vector<ResourceViewDesc>& resourceViewDesc);

		ResourceViewLayout() = default;

	public:
		std::vector<ResourceViewDesc> _resourceViews;
	};
}