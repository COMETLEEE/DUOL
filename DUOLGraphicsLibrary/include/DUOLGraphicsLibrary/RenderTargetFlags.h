#pragma once
#include <vector>

namespace DUOLGraphicsLibrary
{
	class Texture;

	enum class RenderTargetType
	{
		Color,
		Depth,
		Stencil,
		DepthStencil
	};

	struct DUOLGRAPHICSLIBRARY_EXPORT RenderTargetDesc
	{
		RenderTargetDesc() :
			_resolution()
			, _sample(1)
			, _type(RenderTargetType::Color)
			, _texture(nullptr)
			, _mipLevel(0)
			, _arrayLayer(0)
			, _arraySize(1)
		{

		}
		DUOLMath::Vector2 _resolution;

		unsigned int _sample;

		RenderTargetType _type;

		//�ؽ��ĸ� �����Ҷ� bind flag�� RenderTarget�� ������ ��.
		Texture* _texture;

		int _mipLevel;

		int _arrayLayer;

		int _arraySize;
	};

}
