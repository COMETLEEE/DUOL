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

		//�ؽ��ĸ� �����Ҷ� bind flag�� RenderTarget�� ������ ��.
		Texture* _texture = nullptr;

		int _mipLevel = 0;

		int _arrayLayer = 0;
	};


}