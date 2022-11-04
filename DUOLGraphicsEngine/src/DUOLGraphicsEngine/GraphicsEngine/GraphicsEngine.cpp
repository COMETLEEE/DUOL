#include "DUOLGraphicsLibrary/Renderer/Renderer.h"

#include "DUOLGraphicsEngine/GraphicsEngine/GraphicsEngine.h"
#include "DUOLGraphicsEngine/ResourceManager/ResourceManager.h"
#include "DUOLGraphicsEngine/RenderManager/RenderManager.h"
#include "DUOLGraphicsEngine/Util/Hash/Hash.h"
#include "DUOLGraphicsEngine/RenderManager/RenderPipieline.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderContantBuffer.h"

#include <tchar.h>


namespace DUOLGraphicsEngine
{
	GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& engineDesc)
	{
		_renderer = DUOLGraphicsLibrary::Renderer::CreateRenderer(engineDesc._rendererDesc);

		_context = _renderer->CreateRenderContext(engineDesc._contextDesc);
		//_resourceManager = std::make_unique<ResourceManager>(_renderer);
		//_renderManager = std::make_unique<RenderManager>(_renderer, _context);

		Initialize();
		CreateDefaultRenderPass(engineDesc._contextDesc._screenDesc);
	}

	GraphicsEngine::~GraphicsEngine()
	{
		DUOLGraphicsLibrary::Renderer::DeleteRenderer(_renderer);
	}

	void GraphicsEngine::CreateDefaultRenderPass(const DUOLGraphicsLibrary::ScreenDesc& screenDesc)
	{
		DUOLCommon::tstring name = _T("default");

		RenderTargetDesc color[2];
		color[0]._textureDesc._type = DUOLGraphicsLibrary::TextureType::TEXTURE2D;
		color[0]._textureDesc._textureExtend = { screenDesc._screenSize.x, screenDesc._screenSize.y, 0 };
		color[0]._textureDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
		color[0]._textureDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32G32B32A32_FLOAT;
		color[0]._textureDesc._bindFlags |= static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);
		color[0]._textureDesc._bindFlags |= static_cast<long>(DUOLGraphicsLibrary::BindFlags::RENDERTARGET);
		color[0]._textureDesc._cpuAccessFlags |= static_cast<long>(DUOLGraphicsLibrary::CPUAccessFlags::WRITE);

		color[1]._textureDesc._type = DUOLGraphicsLibrary::TextureType::TEXTURE2D;
		color[1]._textureDesc._textureExtend = { screenDesc._screenSize.x, screenDesc._screenSize.y, 0 };
		color[1]._textureDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
		color[1]._textureDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R8G8B8A8_UNORM;
		color[1]._textureDesc._bindFlags |= static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);
		color[1]._textureDesc._bindFlags |= static_cast<long>(DUOLGraphicsLibrary::BindFlags::RENDERTARGET);
		color[1]._textureDesc._cpuAccessFlags |= static_cast<long>(DUOLGraphicsLibrary::CPUAccessFlags::WRITE);

		RenderTargetDesc depth;

		depth._textureDesc._type = DUOLGraphicsLibrary::TextureType::TEXTURE2D;
		depth._textureDesc._textureExtend = { screenDesc._screenSize.x, screenDesc._screenSize.y, 0 };
		depth._textureDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
		depth._textureDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R24G8_TYPELESS;
		depth._textureDesc._bindFlags |= static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);
		depth._textureDesc._bindFlags |= static_cast<long>(DUOLGraphicsLibrary::BindFlags::DEPTHSTENCIL);

		depth._renderTargetDesc._type = DUOLGraphicsLibrary::RenderTargetType::DepthStencil;

		DUOLGraphicsLibrary::RenderPassDesc renderPassDesc;

		for (int textureIndex = 0; textureIndex < 2; textureIndex++)
		{
			//DUOLCommon::tstring strTextureID = name + std::to_wstring(textureIndex);
			//auto textureID = Hash::Hash64(strTextureID);
			//color[textureIndex]._renderTargetDesc._texture = _resourceManager->CreateTexture(textureID, color[textureIndex]._textureDesc);
			//renderPassDesc._renderTargetView.push_back(_resourceManager->CreateRenderTarget(color[textureIndex]._renderTargetDesc));
		}

		renderPassDesc._renderTargetView.push_back(_context->GetBackBufferRenderTarget());

		{
			//texture naming 
			DUOLCommon::tstring strTextureID = name + (_T("Depth"));
			auto textureID = Hash::Hash64(strTextureID);

			depth._renderTargetDesc._texture = _resourceManager->CreateTexture(textureID, depth._textureDesc);
			renderPassDesc._depthStencilView = _resourceManager->CreateRenderTarget(depth._renderTargetDesc);
		}

		//
		DUOLGraphicsLibrary::PipelineStateDesc pipelineStateDesc;

		pipelineStateDesc._primitiveTopology = DUOLGraphicsLibrary::PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		DUOLGraphicsLibrary::ShaderDesc vsDesc;
		vsDesc._entryPoint = "VSMain";
		vsDesc._profile = "vs_5_0";
		vsDesc._source = "shader/VertexShader_Mesh.hlsl";
		vsDesc._type = DUOLGraphicsLibrary::ShaderType::VERTEX;


		DUOLGraphicsLibrary::ShaderDesc psDesc;
		psDesc._entryPoint = "PSMain";
		psDesc._profile = "ps_5_0";
		psDesc._source = "shader/PixelShader_Mesh.hlsl";
		psDesc._type = DUOLGraphicsLibrary::ShaderType::PIXEL;

		auto hashv = Hash::Hash64(_T("defaultVS"));
		auto hashp = Hash::Hash64(_T("defaultPS"));

		pipelineStateDesc._vertexShader = _resourceManager->CreateShader(Hash::Hash64(_T("defaultVS")), vsDesc);
		pipelineStateDesc._pixelShader = _resourceManager->CreateShader(Hash::Hash64(_T("defaultPS")), psDesc);
		pipelineStateDesc._rasterizerStateDesc._cullMode = DUOLGraphicsLibrary::RasterizerStateDesc::CullMode::CULL_NONE;
		pipelineStateDesc._rasterizerStateDesc._fillMode = DUOLGraphicsLibrary::RasterizerStateDesc::FillMode::SOLID;
		pipelineStateDesc._rasterizerStateDesc._frontCounterClockWise = true;

		pipeline = new RenderPipieline(
			name
			, _resourceManager.get()
			, _perFrameBuffer
			, _perObjectBuffer
			, renderPassDesc
			, pipelineStateDesc
		);

	}

	void GraphicsEngine::Initialize()
	{
		_resourceManager = std::make_unique<ResourceManager>(_renderer);
		_renderManager = std::make_unique<RenderManager>(_renderer, _context);

		DUOLGraphicsLibrary::BufferDesc perFrameBufferDesc;
		perFrameBufferDesc._size = sizeof(ConstantBufferPerFrame);
		perFrameBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DYNAMIC;
		perFrameBufferDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_UNKNOWN;
		perFrameBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::CONSTANTBUFFER);
		perFrameBufferDesc._cpuAccessFlags = static_cast<long>(DUOLGraphicsLibrary::CPUAccessFlags::WRITE);

		_perFrameBuffer = _resourceManager->CreateEmptyBuffer(_T("perFrameBuffer"), perFrameBufferDesc);

		DUOLGraphicsLibrary::BufferDesc perObjectBufferDesc;
		perObjectBufferDesc._size = sizeof(Transfrom) + 32;
		perObjectBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DYNAMIC;
		perObjectBufferDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_UNKNOWN;
		perObjectBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::CONSTANTBUFFER);
		perObjectBufferDesc._cpuAccessFlags = static_cast<long>(DUOLGraphicsLibrary::CPUAccessFlags::WRITE);

		_perObjectBuffer = _resourceManager->CreateEmptyBuffer(_T("perObjectBuffer"), perObjectBufferDesc);

	}

	void GraphicsEngine::RenderObject(const DUOLGraphicsEngine::RenderObject* object)
	{
		_renderManager->Render(*object);
	}

	void GraphicsEngine::Excute(const ConstantBufferPerFrame& perFrameInfo, DUOLMath::Vector2 resolution)
	{
		_renderManager->ExecuteRenderPass(pipeline, resolution, perFrameInfo);
	}

	void GraphicsEngine::Present()
	{
		_renderManager->Present();
	}

	Mesh* GraphicsEngine::LoadMesh(const DUOLCommon::tstring& objectID)
	{
		return _resourceManager->CreateMesh(objectID, _T(""));
	}

	Material* GraphicsEngine::LoadMaterial(const DUOLCommon::tstring& objectID)
	{
		return _resourceManager->GetMaterial(objectID);
	}

	void GraphicsEngine::Test()
	{
		_resourceManager->CreateMesh(_T("box"), _T("resource/mesh/box.fbx"));

	}
}
