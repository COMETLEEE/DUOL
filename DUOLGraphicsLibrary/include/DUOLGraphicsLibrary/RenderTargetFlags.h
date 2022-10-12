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

		//텍스쳐를 생성할때 bind flag로 RenderTarget을 지정할 것.
		Texture* _texture = nullptr;

		int _mipLevel = 0;

		int _arrayLayer = 0;
	};
}