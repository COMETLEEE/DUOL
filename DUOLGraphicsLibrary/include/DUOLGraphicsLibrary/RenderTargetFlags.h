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

	//struct RenderTargetComponentDesc
	//{
	//	RenderTargetComponentDesc() :
	//		_type(RenderTargetType::Color)
	//		, _texture(nullptr)
	//		, _mipLevel(0)
	//		, _arrayLayer(0)
	//	{

	//	}

	//	RenderTargetType _type;

	//	//�ؽ��ĸ� �����Ҷ� bind flag�� RenderTarget�� ������ ��.
	//	Texture* _texture = nullptr;

	//	int _mipLevel = 0;

	//	int _arrayLayer = 0;
	//};

	struct RenderTargetDesc
	{
		RenderTargetDesc() :
			_resolution()
			, _sample(1)
			, _type(RenderTargetType::Color)
			, _texture(nullptr)
			, _mipLevel(0)
			, _arrayLayer(0)
		{

		}
		DUOLMath::Vector2 _resolution;

		unsigned int _sample;

		RenderTargetType _type;

		//�ؽ��ĸ� �����Ҷ� bind flag�� RenderTarget�� ������ ��.
		Texture* _texture;

		int _mipLevel;

		int _arrayLayer;
	};

}
