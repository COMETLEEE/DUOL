#include "DUOLGraphicsEngine/ResourceManager/Resource/SkyBox.h"
#include "DUOLGraphicsEngine/GeometryGenerator.h"
#include "DUOLGraphicsEngine/GraphicsEngine/GraphicsEngine.h"
#include "DUOLGraphicsEngine/RenderManager/RenderManager.h"
#include "DUOLGraphicsEngine/ResourceManager/ResourceManager.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Vertex.h"
#include "DUOLGraphicsEngine/Util/Hash/Hash.h"

#include "DUOLGraphicsLibrary/BufferFlags.h"
#include "DUOLGraphicsLibrary/ResourceFlags.h"
#include "DUOLGraphicsLibrary/TextureFlags.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"

namespace DUOLGraphicsEngine
{
	SkyBox::SkyBox(DUOLGraphicsEngine::GraphicsEngine* const graphicsEngine, const DUOLCommon::tstring& skyboxPath, DUOLGraphicsEngine::RenderingPipeline* skyboxPipeline) :
		_skyboxPipeline(skyboxPipeline)
		, _skyboxTexture(nullptr)
		, _skyboxPreFilteredTexture(nullptr)
		, _skyboxCubeMap(nullptr)
		, _skyboxIrradianceTexture(nullptr)
		, _skyboxBRDFLookUpTexture(nullptr)
	{
		auto resourceManager = graphicsEngine->GetResourceManager();
		auto renderManager = graphicsEngine->GetRenderManager();

		CreateSkyboxMesh(resourceManager);
		_skyboxBRDFLookUpTexture = BakeBRDFLookUpTable(resourceManager, renderManager, 256, 256);

		SetSkyboxTexture(skyboxPath, graphicsEngine);
	}

	bool SkyBox::SetSkyboxTexture(const DUOLCommon::tstring& skyboxPath, DUOLGraphicsEngine::GraphicsEngine* const graphicsEngine)
	{
		DUOLGraphicsLibrary::TextureDesc skyboxTextureDesc;

		auto resourceManager = graphicsEngine->GetResourceManager();
		auto renderManger = graphicsEngine->GetRenderManager();
		auto renderer = graphicsEngine->GetRenderer();

		auto path = DUOLCommon::StringHelper::ToString(skyboxPath);

		skyboxTextureDesc._texturePath = path.c_str();

		if (resourceManager->GetTexture(_T("SkyBoxTexture")) == nullptr)
		{
			_skyboxTexture = resourceManager->CreateTexture(Hash::Hash64(_T("SkyBoxTexture")), skyboxTextureDesc);
		}
		else
		{
			//만약 텍스쳐가 이미 있으면 지우고 다시만든다.
			resourceManager->DeleteTexture(_T("SkyBoxTexture"));
			_skyboxTexture = resourceManager->CreateTexture(Hash::Hash64(_T("SkyBoxTexture")), skyboxTextureDesc);
		}

		//texture type이 2d면... 이녀석은 panorama texture이므로 cubemap으로 바꿔주어야 한다.
		if (_skyboxTexture->GetTextureDesc()._type == DUOLGraphicsLibrary::TextureType::TEXTURE2D)
		{
			_skyboxCubeMap = CreateCubeMapFromPanoramaImage(resourceManager, renderManger, _skyboxTexture, 512, 512);
			renderer->GenerateMips(_skyboxCubeMap);
		}
		else
		{
			_skyboxCubeMap = _skyboxTexture;
		}

		_skyboxIrradianceTexture = BakeIBLIrradianceMap(resourceManager, renderManger, _skyboxCubeMap, 512, 512);
		_skyboxPreFilteredTexture = BakeIBLPreFilteredMap(resourceManager, renderManger, _skyboxCubeMap, 5, 512, 512);

		_skyboxPipeline->SetTexture(_skyboxCubeMap, 0);

		//텍스쳐를 다만들었으므로 원본 텍스쳐는 삭제합니다.
		if (_skyboxTexture->GetTextureDesc()._type == DUOLGraphicsLibrary::TextureType::TEXTURE2D)
		{
			resourceManager->DeleteTexture(_T("SkyBoxTexture"));
		}

		return true;
	}

	void SkyBox::Test(DUOLGraphicsEngine::ResourceManager* const resourceManager,
		DUOLGraphicsEngine::RenderManager* const renderManager)
	{
		CreateCubeMapFromPanoramaImage(resourceManager, renderManager, _skyboxTexture, 512, 512);
		BakeIBLIrradianceMap(resourceManager, renderManager, _skyboxCubeMap, 512, 512);
	}

	bool SkyBox::CreateSkyboxMesh(DUOLGraphicsEngine::ResourceManager* const resourceManager)
	{
		std::vector<StaticMeshVertex> vertices;
		std::vector<UINT32> indices;

		GeometryGenerator::CreateSphere(1, 24, 24, vertices, indices);

		DUOLGraphicsLibrary::BufferDesc vetexBufferDesc;

		vetexBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::VERTEXBUFFER);
		vetexBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DYNAMIC;
		vetexBufferDesc._stride = sizeof(StaticMeshVertex);
		vetexBufferDesc._size = static_cast<int>(vetexBufferDesc._stride * vertices.size());
		vetexBufferDesc._cpuAccessFlags = static_cast<long>(DUOLGraphicsLibrary::CPUAccessFlags::WRITE);

		DUOLGraphicsLibrary::BufferDesc indexBufferDesc;

		indexBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::INDEXBUFFER);
		indexBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DYNAMIC;
		indexBufferDesc._stride = sizeof(unsigned int);
		indexBufferDesc._size = static_cast<int>(indexBufferDesc._stride * indices.size());
		indexBufferDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32_UINT;
		indexBufferDesc._cpuAccessFlags = static_cast<long>(DUOLGraphicsLibrary::CPUAccessFlags::WRITE);

		DUOLCommon::tstring id = _T("SkyBoxSphere");

		_skyboxSphereMesh = resourceManager->CreateMesh(
			id,
			static_cast<void*>(vertices.data()),
			static_cast<unsigned int>(vertices.size()),
			sizeof(StaticMeshVertex),
			indices.data(),
			indices.size());

		return true;
	}

	DUOLGraphicsLibrary::Texture* SkyBox::BakeIBLIrradianceMap(DUOLGraphicsEngine::ResourceManager* const resourceManager, DUOLGraphicsEngine::RenderManager* const renderManager, DUOLGraphicsLibrary::Texture* cubeMap, float width,
		float height)
	{
		auto originTexture = cubeMap;

		auto& originTextureDesc = originTexture->GetTextureDesc();
		DUOLGraphicsLibrary::TextureDesc textureDesc;

		//if (originTextureDesc._type == DUOLGraphicsLibrary::TextureType::TEXTURECUBE)
		{
			textureDesc._textureExtent = DUOLMath::Vector3{ width, height, 0 };
			textureDesc._arraySize = 6;
			textureDesc._type = DUOLGraphicsLibrary::TextureType::TEXTURECUBE;
			textureDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32G32B32A32_FLOAT;
			textureDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::RENDERTARGET) | static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);

			DUOLCommon::tstring textureID = _T("SkyBoxIrradianceMap");

			auto irradianceMap = resourceManager->CreateTexture(textureID, textureDesc);

			DUOLGraphicsLibrary::RenderTargetDesc renderTargetDesc;

			renderTargetDesc._type = DUOLGraphicsLibrary::RenderTargetType::Color;
			renderTargetDesc._mipLevel = 0;
			renderTargetDesc._texture = irradianceMap;

			DUOLGraphicsLibrary::RenderTarget* cubeIrradianceRenderTarget[6];

			for (int i = 0; i < 6; ++i)
			{
				renderTargetDesc._arrayLayer = i;
				DUOLCommon::tstring renderTargetID = textureID + DUOLCommon::StringHelper::ToTString(i);
				cubeIrradianceRenderTarget[i] = resourceManager->CreateRenderTarget(renderTargetID, renderTargetDesc);
			}

			auto pipeline = resourceManager->GetPipelineState(Hash::Hash64(_T("CubeMapToIrradianceMap")));
			auto depth = resourceManager->GetRenderTarget(Hash::Hash64(_T("DefaultDepth")));
			auto sampler = resourceManager->GetSampler(Hash::Hash64(_T("SamTriLinear")));

			renderManager->CreateCubeMapFromPanoramaImage(originTexture, cubeIrradianceRenderTarget, pipeline, resourceManager->GetPerObjectBuffer(), sampler);

			for (int i = 0; i < 6; ++i)
			{
				DUOLCommon::tstring renderTargetID = textureID + DUOLCommon::StringHelper::ToTString(i);
				resourceManager->DeleteRenderTarget(renderTargetID);
			}

			return irradianceMap;
		}

		return nullptr;
	}

	DUOLGraphicsLibrary::Texture* SkyBox::BakeIBLPreFilteredMap(DUOLGraphicsEngine::ResourceManager* const resourceManager, DUOLGraphicsEngine::RenderManager* const renderManager, DUOLGraphicsLibrary::Texture* cubeMap, int mipSize,
		float width, float height)
	{
		auto originTexture = cubeMap;

		auto& originTextureDesc = originTexture->GetTextureDesc();
		DUOLGraphicsLibrary::TextureDesc textureDesc;

		//if (originTextureDesc._type == DUOLGraphicsLibrary::TextureType::TEXTURECUBE)
		{
			textureDesc._textureExtent = DUOLMath::Vector3{ width, height, 0 };
			textureDesc._arraySize = 6;
			textureDesc._type = DUOLGraphicsLibrary::TextureType::TEXTURECUBE;
			textureDesc._format = originTextureDesc._format;
			textureDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::RENDERTARGET) | static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);
			textureDesc._mipLevels = mipSize;

			DUOLCommon::tstring textureID = _T("SkyBoxRadianceMap");

			auto radianceMap = resourceManager->CreateTexture(textureID, textureDesc);

			DUOLGraphicsLibrary::RenderTargetDesc renderTargetDesc;

			renderTargetDesc._type = DUOLGraphicsLibrary::RenderTargetType::Color;
			renderTargetDesc._texture = radianceMap;

			DUOLGraphicsLibrary::RenderTarget** radianceRenderTarget = new DUOLGraphicsLibrary::RenderTarget * [6 * mipSize];

			for (int j = 0; j < mipSize; ++j)
			{
				for (int i = 0; i < 6; ++i)
				{
					renderTargetDesc._mipLevel = j;
					renderTargetDesc._arrayLayer = i;
					int arrayIdx = 6 * j + i;
					DUOLCommon::tstring renderTargetID = textureID + DUOLCommon::StringHelper::ToTString(arrayIdx);
					radianceRenderTarget[arrayIdx] = resourceManager->CreateRenderTarget(renderTargetID, renderTargetDesc);
				}
			}

			auto pipeline = resourceManager->GetPipelineState(Hash::Hash64(_T("CubeMapToRadianceMap")));
			auto depth = resourceManager->GetRenderTarget(Hash::Hash64(_T("DefaultDepth")));
			auto sampler = resourceManager->GetSampler(Hash::Hash64(_T("SamLinear")));

			renderManager->CreatePreFilteredMapFromCubeImage(originTexture, radianceRenderTarget, pipeline, depth, resourceManager->GetPerObjectBuffer(), sampler, mipSize, width, height);

			for (int j = 0; j < mipSize; ++j)
			{
				for (int i = 0; i < 6; ++i)
				{
					int arrayIdx = 6 * j + i;
					DUOLCommon::tstring renderTargetID = textureID + DUOLCommon::StringHelper::ToTString(arrayIdx);
					resourceManager->DeleteRenderTarget(renderTargetID);
				}
			}

			delete[] radianceRenderTarget;

			return radianceMap;
		}

		return nullptr;
	}

	DUOLGraphicsLibrary::Texture* SkyBox::BakeBRDFLookUpTable(DUOLGraphicsEngine::ResourceManager* const resourceManager, DUOLGraphicsEngine::RenderManager* const renderManager, float width, float height)
	{
		DUOLGraphicsLibrary::TextureDesc textureDesc;

		textureDesc._textureExtent = DUOLMath::Vector3{ width, height, 0 };
		textureDesc._arraySize = 1;
		textureDesc._type = DUOLGraphicsLibrary::TextureType::TEXTURE2D;
		textureDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32_FLOAT;
		textureDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::RENDERTARGET) | static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);

		DUOLCommon::tstring textureID = _T("SkyBoxBRDFLookUpTable");

		auto _skyboxBRDFLookUpTexture = resourceManager->CreateTexture(textureID, textureDesc);

		DUOLGraphicsLibrary::RenderTargetDesc renderTargetDesc;

		renderTargetDesc._type = DUOLGraphicsLibrary::RenderTargetType::Color;
		renderTargetDesc._mipLevel = 0;
		renderTargetDesc._texture = _skyboxBRDFLookUpTexture;

		DUOLGraphicsLibrary::RenderTarget* BRDFRenderTarget;
		BRDFRenderTarget = resourceManager->CreateRenderTarget(textureID, renderTargetDesc);

		auto pipeline = resourceManager->GetPipelineState(Hash::Hash64(_T("BRDFLookUpTable")));
		auto depth = resourceManager->GetRenderTarget(Hash::Hash64(_T("DefaultDepth")));

		renderManager->CreateBRDFLookUpTable(BRDFRenderTarget, pipeline, depth, resourceManager->GetPerObjectBuffer(), static_cast<UINT>(width), static_cast<UINT>(height));
		resourceManager->DeleteRenderTarget(textureID);

		return _skyboxBRDFLookUpTexture;
	}

	DUOLGraphicsLibrary::Texture* SkyBox::CreateCubeMapFromPanoramaImage(DUOLGraphicsEngine::ResourceManager* const resourceManager, DUOLGraphicsEngine::RenderManager* const renderManager, DUOLGraphicsLibrary::Texture* panorama, float width, float height)
	{
		auto originTexture = panorama;

		auto& originTextureDesc = originTexture->GetTextureDesc();
		DUOLGraphicsLibrary::TextureDesc textureDesc;

		if (originTextureDesc._type == DUOLGraphicsLibrary::TextureType::TEXTURE2D)
		{
			textureDesc._textureExtent = DUOLMath::Vector3{ width, height, 0 };
			textureDesc._arraySize = 6;
			textureDesc._type = DUOLGraphicsLibrary::TextureType::TEXTURECUBE;
			textureDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32G32B32A32_FLOAT;
			textureDesc._mipLevels = 0;
			textureDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::RENDERTARGET) | static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);

			DUOLCommon::tstring textureID = _T("SkyboxCubeMap");

			auto skyBox = resourceManager->CreateTexture(textureID, textureDesc);

			DUOLGraphicsLibrary::RenderTargetDesc renderTargetDesc;

			renderTargetDesc._type = DUOLGraphicsLibrary::RenderTargetType::Color;
			renderTargetDesc._mipLevel = 0;
			renderTargetDesc._texture = skyBox;

			DUOLGraphicsLibrary::RenderTarget* skyBoxRenderTargets[6];

			for (int i = 0; i < 6; ++i)
			{
				renderTargetDesc._arrayLayer = i;
				DUOLCommon::tstring renderTargetID = textureID + DUOLCommon::StringHelper::ToTString(i);
				skyBoxRenderTargets[i] = resourceManager->CreateRenderTarget(renderTargetID, renderTargetDesc);
			}

			auto pipeline = resourceManager->GetPipelineState(Hash::Hash64(_T("PanoramaToCubeMap")));
			auto sampler = resourceManager->GetSampler(Hash::Hash64(_T("SamLinear")));

			renderManager->CreateCubeMapFromPanoramaImage(panorama, skyBoxRenderTargets, pipeline, resourceManager->GetPerObjectBuffer(), sampler);

			for (int i = 0; i < 6; ++i)
			{
				DUOLCommon::tstring renderTargetID = textureID + DUOLCommon::StringHelper::ToTString(i);
				resourceManager->DeleteRenderTarget(renderTargetID);
			}

			return skyBox;
		}

		return nullptr;
	}
}
