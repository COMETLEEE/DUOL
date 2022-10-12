#pragma once

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

	struct RenderTargetDesc
	{
		RenderTargetDesc():
			_type(RenderTargetType::Color)
			, _texture(nullptr)
			, _mipLevel(0)
			, _arrayLayer(0)
		{

		}

		RenderTargetType _type;

		//�ؽ��ĸ� �����Ҷ� bind flag�� RenderTarget�� ������ ��.
		Texture* _texture = nullptr;

		int _mipLevel = 0;

		int _arrayLayer = 0;
	};
}