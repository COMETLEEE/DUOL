#pragma once
#include "ResourceHolder.h"
#include "DUOLGraphicsLibrary/TextureFlags.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"

namespace DUOLGraphicsLibrary
{
	class Texture;
}

namespace DUOLGraphicsEngine
{
	class TextureHolder : public ResourceHolder<DUOLGraphicsLibrary::Texture>
	{
	public:
		virtual DUOLGraphicsLibrary::Texture* LoadResource(DUOLGraphicsLibrary::Renderer* _renderer, const DUOLCommon::tstring& path, const DUOLCommon::tstring& id) override;

				virtual void UnloadResource(DUOLGraphicsLibrary::Renderer* _renderer, const DUOLCommon::tstring& id) override;

	public:
	};

	DUOLGraphicsLibrary::Texture* DUOLGraphicsEngine::TextureHolder::LoadResource(DUOLGraphicsLibrary::Renderer* _renderer,
		const DUOLCommon::tstring& path, const DUOLCommon::tstring& id)
	{
		DUOLGraphicsLibrary::TextureDesc textureDesc;

		//wchar�� char�� ĳ������ �ѹ� ��ģ��.. ���� ������ �𸣴ϱ�
		//�ʿ��Ѱ� char string
		std::string castedString(path.begin(), path.end());
		textureDesc._texturePath = castedString.c_str();

		auto texture = _renderer->CreateTexture(textureDesc);

		_resources.emplace(id, texture);

	}

	void DUOLGraphicsEngine::TextureHolder::UnloadResource(DUOLGraphicsLibrary::Renderer* _renderer,
		const DUOLCommon::tstring& id)
	{
		auto foundedResource = _resources.find(id);
		if (foundedResource == _resources.end())
		{
			return;
		}

		_resources.erase(id);
		_renderer->Release(foundedResource->second);

	}

}