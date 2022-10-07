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
		RenderTargetType _type = RenderTargetType::Color;

		//텍스쳐를 생성할때 bind flag로 RenderTarget을 지정할 것.
		Texture* _texture = nullptr;

		int _mipLevel = 0;

		int _arrayLayer = 0;
	};


}