#include "RenderManager.h"

#include "CullingHelper.h"
#include "RenderingPipeline.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/PerlinNoise.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Vertex.h"
#include "DUOLGraphicsLibrary/Renderer/ResourceViewLayout.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"
#include "DUOLGraphicsEngine/Util/Hash/Hash.h"
#include "DUOLGraphicsLibrary/FontEngine/IFontEngine.h"

DUOLGraphicsEngine::RenderManager::RenderManager(DUOLGraphicsLibrary::Renderer* renderer, DUOLGraphicsLibrary::RenderContext* context, DUOLGraphicsLibrary::Buffer* PerFrameBuffer, DUOLGraphicsLibrary::Buffer* PerObjectBuffer) :
	_renderer(renderer)
	, _context(context)
	, _oitDrawCount(0)
	, _perFrameBuffer(PerFrameBuffer)
	, _perObjectBuffer(PerObjectBuffer)
{
	DUOLGraphicsLibrary::CommandBufferDesc commandBufferDesc;

	_commandBuffer = _renderer->CreateCommandBuffer(0, commandBufferDesc);
	_opaqueRenderQueue.reserve(60);
	_transparencyRenderQueue.reserve(60);

	_buffer = std::make_unique<ByteBuffer>();

	CreatePostProcessingRect();
	ReserveResourceLayout();
	CreateStreamOutBuffer(renderer);
	CreateAxis(renderer);
}

void DUOLGraphicsEngine::RenderManager::CreateAxis(DUOLGraphicsLibrary::Renderer* renderer)
{
	struct DebugVertex
	{
		DUOLMath::Vector3 _position;
		int _color;
	};

	DebugVertex vertice[6];

	vertice[0]._position = DUOLMath::Vector3{ 0.f, 0.f, 0.f };
	vertice[0]._color = 0xff000000;
	vertice[1]._position = DUOLMath::Vector3{ 100.f, 0.f, 0.f };
	vertice[1]._color = 0xff000000;
	vertice[2]._position = DUOLMath::Vector3{ 0.f, 0.f, 0.f };
	vertice[2]._color = 0x00ff0000;
	vertice[3]._position = DUOLMath::Vector3{ 0.f, 100.f, 0.f };
	vertice[3]._color = 0x00ff0000;
	vertice[4]._position = DUOLMath::Vector3{ 0.f, 0.f, 0.f };
	vertice[4]._color = 0x0000ff00;
	vertice[5]._position = DUOLMath::Vector3{ 0.f, 0.f, 100.f };
	vertice[5]._color = 0x0000ff00;

	UINT indice[6] = { 0, 1, 2, 3, 4, 5 };

	DUOLGraphicsLibrary::BufferDesc vetexBufferDesc;

	vetexBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::VERTEXBUFFER);
	vetexBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DYNAMIC;
	vetexBufferDesc._stride = sizeof(DebugVertex);
	vetexBufferDesc._size = vetexBufferDesc._stride * 6;
	vetexBufferDesc._cpuAccessFlags = static_cast<long>(DUOLGraphicsLibrary::CPUAccessFlags::WRITE);

	DUOLGraphicsLibrary::BufferDesc indexBufferDesc;

	indexBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::INDEXBUFFER);
	indexBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DYNAMIC;
	indexBufferDesc._stride = sizeof(unsigned int);
	indexBufferDesc._size = indexBufferDesc._stride * 6;
	indexBufferDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32_UINT;
	indexBufferDesc._cpuAccessFlags = static_cast<long>(DUOLGraphicsLibrary::CPUAccessFlags::WRITE);

	_axisVertex = renderer->CreateBuffer(Hash::Hash64(_T("AxisVertex")), vetexBufferDesc, vertice);
	_axisIndex = renderer->CreateBuffer(Hash::Hash64(_T("Axisindex")), indexBufferDesc, indice);
}

void DUOLGraphicsEngine::RenderManager::SetStreamOutShader(DUOLGraphicsLibrary::PipelineState* streamOut, DUOLGraphicsLibrary::PipelineState* trailPipeline)
{
	_streamOutShader = streamOut;
	_particleTrailShader = trailPipeline;
}

void DUOLGraphicsEngine::RenderManager::CreateStreamOutBuffer(DUOLGraphicsLibrary::Renderer* renderer)
{
	DUOLGraphicsLibrary::BufferDesc streamOutBufferDesc;

	streamOutBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::STREAMOUTPUTBUFFER) | static_cast<long>(DUOLGraphicsLibrary::BindFlags::VERTEXBUFFER);
	streamOutBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
	streamOutBufferDesc._stride = 0;
	streamOutBufferDesc._size = 500000;
	streamOutBufferDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_UNKNOWN;
	streamOutBufferDesc._cpuAccessFlags = 0;

	_streamOutBuffer = renderer->CreateBuffer(Hash::Hash64(_T("streamOutBuffer")), streamOutBufferDesc, nullptr);

	DUOLMath::Vector4 randomValues[1024];

	for (int i = 0; i < 1024; ++i)
	{
		randomValues[i].x = DUOLMath::MathHelper::RandF(-1.0f, 1.0f);
		randomValues[i].y = DUOLMath::MathHelper::RandF(-1.0f, 1.0f);
		randomValues[i].z = DUOLMath::MathHelper::RandF(-1.0f, 1.0f);
		randomValues[i].w = DUOLMath::MathHelper::RandF(-1.0f, 1.0f);
	}

	DUOLGraphicsLibrary::TextureDesc textureDesc;

	textureDesc._initData = randomValues;
	textureDesc._type = DUOLGraphicsLibrary::TextureType::TEXTURE1D;
	textureDesc._size = 1024 * sizeof(DUOLMath::Vector4);
	textureDesc._mipLevels = 1;
	textureDesc._textureExtent = DUOLMath::Vector3{ 1024.f, 0.f, 0.f };
	textureDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_IMMUTABLE;
	textureDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32G32B32A32_FLOAT;
	textureDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);
	textureDesc._cpuAccessFlags = 0;
	textureDesc._arraySize = 1;

	_particleRandomTexture = renderer->CreateTexture(Hash::Hash64(_T("ParticleRandomTextrue")), textureDesc);

}

void DUOLGraphicsEngine::RenderManager::ReserveResourceLayout()
{
	_currentBindBuffer._resourceViews.reserve(2);
	_currentBindBuffer._resourceViews.emplace_back(_perFrameBuffer, 0, static_cast<long>(DUOLGraphicsLibrary::BindFlags::CONSTANTBUFFER), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE));
	_currentBindBuffer._resourceViews.emplace_back(_perObjectBuffer, 1, static_cast<long>(DUOLGraphicsLibrary::BindFlags::CONSTANTBUFFER), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE));

	_currentBindTextures._resourceViews.reserve(8);
	//texture
	_currentBindTextures._resourceViews.emplace_back(nullptr, 0, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE));
	_currentBindTextures._resourceViews.emplace_back(nullptr, 1, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE));
	_currentBindTextures._resourceViews.emplace_back(nullptr, 2, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE));
	_currentBindTextures._resourceViews.emplace_back(nullptr, 3, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE));
	_currentBindTextures._resourceViews.emplace_back(nullptr, 4, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE));
	_currentBindTextures._resourceViews.emplace_back(nullptr, 5, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE));
	_currentBindTextures._resourceViews.emplace_back(nullptr, 6, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE));
	_currentBindTextures._resourceViews.emplace_back(nullptr, 7, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE));
}

void DUOLGraphicsEngine::RenderManager::ExecuteRenderingPipeline(RenderingPipeline* renderPipeline, void* postProcessingData, int dataSize)
{
#if defined(_DEBUG) || defined(DEBUG)
	_renderer->BeginEvent(renderPipeline->GetName().c_str());
#endif
	switch (renderPipeline->GetPipelineType())
	{
	case PipelineType::Render:
	{
		ExecuteRenderPass(renderPipeline);
		break;
	}
	case PipelineType::OrderIndependentTransparency:
	{
		ExecuteOrderIndependentTransparencyPass(renderPipeline);
		break;
	}
	case PipelineType::PostProcessing:
	{
		ExecutePostProcessingPass(renderPipeline, postProcessingData, dataSize);
		break;
	}
	default:;
	}
#if defined(_DEBUG) || defined(DEBUG)
	_renderer->EndEvent();
#endif
	_commandBuffer->Flush();
}

void DUOLGraphicsEngine::RenderManager::ExecuteRenderPass(RenderingPipeline* renderPipeline)
{
	_commandBuffer->SetRenderPass(renderPipeline->GetRenderPass());

	const size_t renderQueueSize = _opaqueRenderQueue.size();

	for (uint32_t renderIndex = 0; renderIndex < renderQueueSize; renderIndex++)
	{
		RenderObject* renderObject = _opaqueRenderQueue[renderIndex];

		RenderMesh(*renderObject, renderPipeline);
	}
}

void DUOLGraphicsEngine::RenderManager::ExecuteDebugRenderPass(RenderingPipeline* renderPipeline)
{
	_commandBuffer->SetRenderPass(renderPipeline->GetRenderPass());

	const size_t renderQueueSize = _renderDebugQueue.size();

	for (uint32_t renderIndex = 0; renderIndex < renderQueueSize; renderIndex++)
	{
		RenderObject& renderObject = *_renderDebugQueue[renderIndex];

		renderObject._renderInfo->BindPipeline(_buffer.get());

		for (unsigned int submeshIndex = 0; submeshIndex < renderObject._materials->size(); submeshIndex++)
		{
			auto currentMaterial = renderObject._materials->at(submeshIndex);

			_commandBuffer->SetPipelineState(currentMaterial->GetPipelineState());

			_commandBuffer->SetVertexBuffer(renderObject._mesh->_vertexBuffer);
			_commandBuffer->SetIndexBuffer(renderObject._mesh->_subMeshs[submeshIndex]._indexBuffer);

			_commandBuffer->UpdateBuffer(_perObjectBuffer, 0, _buffer->GetBufferStartPoint(), sizeof(Transform));
			_commandBuffer->SetResources(renderPipeline->GetTextureResourceViewLayout());
			_commandBuffer->SetResources(renderPipeline->GetSamplerResourceViewLayout());

			_commandBuffer->DrawIndexed(renderObject._mesh->_subMeshs[submeshIndex]._drawIndex, 0, 0);
		}

		{
			_commandBuffer->SetVertexBuffer(_axisVertex);
			_commandBuffer->SetIndexBuffer(_axisIndex);

			Transform transform;

			_buffer->WriteData(&transform, sizeof(Transform));

			_commandBuffer->UpdateBuffer(_perObjectBuffer, 0, _buffer->GetBufferStartPoint(), sizeof(Transform) + 48);
			_commandBuffer->SetResources(renderPipeline->GetTextureResourceViewLayout());
			_commandBuffer->SetResources(renderPipeline->GetSamplerResourceViewLayout());

			_commandBuffer->DrawIndexed(6, 0, 0);
		}
	}


	_commandBuffer->Flush();
}

void DUOLGraphicsEngine::RenderManager::ExecuteDebugRenderTargetPass(RenderingPipeline* renderPipeline)
{
	_commandBuffer->SetRenderPass(renderPipeline->GetRenderPass());

	_commandBuffer->SetPipelineState(renderPipeline->GetPipelineState());

	_commandBuffer->SetVertexBuffer(_postProcessingRectVertex);
	_commandBuffer->SetIndexBuffer(_postProcessingRectIndex);

	_commandBuffer->SetResources(renderPipeline->GetTextureResourceViewLayout());

	for (int renderTargets = 0; renderTargets < 4; renderTargets++)
	{
		_commandBuffer->UpdateBuffer(_perObjectBuffer, 0, &renderTargets, sizeof(renderTargets));

		_commandBuffer->DrawIndexed(GetNumIndicesFromBuffer(_postProcessingRectIndex), 0, 0);
	}

	_commandBuffer->Flush();
}

void DUOLGraphicsEngine::RenderManager::RenderSkyBox(RenderingPipeline* skyBox, DUOLGraphicsLibrary::Texture* skyboxCubemap, DUOLGraphicsLibrary::Buffer* vertices, DUOLGraphicsLibrary::Buffer* indices, const Camera& cameraInfo)
{
#if defined(_DEBUG) || defined(DEBUG)
	_renderer->BeginEvent(_T("SkyBox"));
#endif

	const size_t renderQueueSize = _renderDebugQueue.size();
	_commandBuffer->SetRenderPass(skyBox->GetRenderPass());
	_commandBuffer->SetPipelineState(skyBox->GetPipelineState());

	_commandBuffer->SetVertexBuffer(vertices);
	_commandBuffer->SetIndexBuffer(indices);

	skyBox->ChangeTexture(skyboxCubemap, 0);

	DUOLMath::Matrix skyBoxMat = DUOLMath::Matrix::CreateScale(cameraInfo._cameraFar, cameraInfo._cameraFar, cameraInfo._cameraFar) * DUOLMath::Matrix::CreateTranslation(cameraInfo._cameraPosition);

	_commandBuffer->UpdateBuffer(_perObjectBuffer, 0, &skyBoxMat, sizeof(DUOLMath::Matrix));

	_commandBuffer->SetResources(skyBox->GetTextureResourceViewLayout());
	_commandBuffer->SetResources(skyBox->GetSamplerResourceViewLayout());

	_commandBuffer->DrawIndexed(indices->GetBufferDesc()._size / indices->GetBufferDesc()._stride, 0, 0);

	_commandBuffer->Flush();

#if defined(_DEBUG) || defined(DEBUG)
	_renderer->EndEvent();
#endif
}

void DUOLGraphicsEngine::RenderManager::RenderCascadeShadow(DUOLGraphicsEngine::RenderingPipeline* cascadeShadow, DUOLGraphicsLibrary::PipelineState* shadowMesh, DUOLGraphicsLibrary::PipelineState* shadowSkinnedMesh, DUOLGraphicsLibrary::RenderTarget* shadowRenderTarget,
	const ConstantBufferPerFrame& perFrameInfo, const std::vector<RenderObject*>& renderObjects)
{
#if defined(_DEBUG) || defined(DEBUG)
	_renderer->BeginEvent(_T("CascadeShadow"));
#endif

	const size_t renderQueueSize = renderObjects.size();
	_commandBuffer->SetRenderTarget(nullptr, shadowRenderTarget, 0);
	_commandBuffer->SetViewport(shadowRenderTarget->GetResolution());

	for (uint32_t renderIndex = 0; renderIndex < renderQueueSize; renderIndex++)
	{
		RenderObject* renderObject = renderObjects[renderIndex];

		renderObject->_renderInfo->BindPipeline(_buffer.get());
		int renderObjectBufferSize = renderObject->_renderInfo->GetInfoStructureSize();

		_commandBuffer->UpdateBuffer(_perObjectBuffer, 0, _buffer->GetBufferStartPoint(), renderObjectBufferSize);

		_commandBuffer->SetVertexBuffer(renderObject->_mesh->_vertexBuffer);

		if (renderObject->_mesh->GetMeshType() == MeshBase::MeshType::Mesh)
		{
			_commandBuffer->SetPipelineState(shadowMesh);
		}
		else if (renderObject->_mesh->GetMeshType() == MeshBase::MeshType::SkinnedMesh)
		{
			_commandBuffer->SetPipelineState(shadowSkinnedMesh);
		}

		for (unsigned int submeshIndex = 0; submeshIndex < renderObject->_materials->size(); submeshIndex++)
		{
			if (renderObject->_mesh->GetSubMesh(submeshIndex) == nullptr)
				break;

			_commandBuffer->SetIndexBuffer(renderObject->_mesh->_subMeshs[submeshIndex]._indexBuffer);

			_commandBuffer->SetResources(_currentBindBuffer);
			_commandBuffer->SetResources(_currentBindTextures);

			_commandBuffer->DrawIndexed(renderObject->_mesh->_subMeshs[submeshIndex]._drawIndex, 0, 0);
		}

	}
#if defined(_DEBUG) || defined(DEBUG)
	_renderer->EndEvent();
#endif
}

void DUOLGraphicsEngine::RenderManager::ExecutePostProcessingPass(RenderingPipeline* renderPipeline, void* postProcessingData, int dataSize)
{
	_commandBuffer->SetRenderPass(renderPipeline->GetRenderPass());

	_commandBuffer->SetPipelineState(renderPipeline->GetPipelineState());
	if (postProcessingData != nullptr)
	{
		if (dataSize > 0)
		{
			//todo:: buffer을 가변적으로 바꿀때 바꿔줘야하는 코드입니다.
			_buffer->WriteData(postProcessingData, dataSize);
			_commandBuffer->UpdateBuffer(_perObjectBuffer, 0, _buffer->GetBufferStartPoint(), dataSize);
		}
	}

	_commandBuffer->SetVertexBuffer(_postProcessingRectVertex);
	_commandBuffer->SetIndexBuffer(_postProcessingRectIndex);

	_commandBuffer->SetResources(renderPipeline->GetTextureResourceViewLayout());
	_commandBuffer->SetResources(renderPipeline->GetSamplerResourceViewLayout());

	_commandBuffer->DrawIndexed(GetNumIndicesFromBuffer(_postProcessingRectIndex), 0, 0);
}

void DUOLGraphicsEngine::RenderManager::ExecuteOrderIndependentTransparencyPass(RenderingPipeline* renderPipeline)
{
	/// OIT?? ???? Pass ?????? ???? ??? ????.
	// ??????? 2?? ????? ?? ??????..?
	_commandBuffer->SetRenderPass(renderPipeline->GetRenderPass());

	const size_t renderQueueSize = _transparencyRenderQueue.size();

	for (uint32_t renderIndex = 0; renderIndex < renderQueueSize; renderIndex++)
	{
		RenderObject* renderObject = _transparencyRenderQueue[renderIndex];

		RenderParticle(*renderObject, renderPipeline);
	}
	_oitDrawCount++;
	if (_oitDrawCount == 6)
		_oitDrawCount = 0;
}

void DUOLGraphicsEngine::RenderManager::RenderMesh(RenderObject& renderObject, RenderingPipeline* renderPipeline)
{
	renderObject._renderInfo->BindPipeline(_buffer.get());

	int renderObjectBufferSize = renderObject._renderInfo->GetInfoStructureSize();

	_commandBuffer->SetVertexBuffer(renderObject._mesh->_vertexBuffer);

	for (unsigned int submeshIndex = 0; submeshIndex < renderObject._materials->size(); submeshIndex++)
	{
		if (renderObject._mesh->GetSubMesh(submeshIndex) == nullptr)
			break;

		if (renderObject._materials->at(submeshIndex) == nullptr)
			continue;

		auto currentMaterial = renderObject._materials->at(submeshIndex);

		//인덱스 버퍼 바인딩
		_commandBuffer->SetIndexBuffer(renderObject._mesh->_subMeshs[submeshIndex]._indexBuffer);

		//메테리얼 정보를 버퍼에 추가함.
		_commandBuffer->SetPipelineState(currentMaterial->GetPipelineState());
		currentMaterial->BindPipeline(_buffer.get(), renderObjectBufferSize, &_currentBindTextures);

		//constantBuffer Update
		_commandBuffer->UpdateBuffer(_perObjectBuffer, 0, _buffer->GetBufferStartPoint(), renderObjectBufferSize + currentMaterial->GetBindDataSize());

		_commandBuffer->SetResources(renderPipeline->GetSamplerResourceViewLayout());
		_commandBuffer->SetResources(_currentBindBuffer);
		_commandBuffer->SetResources(_currentBindTextures);

		_commandBuffer->DrawIndexed(renderObject._mesh->_subMeshs[submeshIndex]._drawIndex, 0, 0);
	}
}

void DUOLGraphicsEngine::RenderManager::RenderParticle(RenderObject& renderObject, RenderingPipeline* renderPipeline)
{
	renderObject._renderInfo->BindPipeline(_buffer.get());

	int renderObjectBufferSize = renderObject._renderInfo->GetInfoStructureSize();

	auto particleObject = static_cast<ParticleBuffer*>(renderObject._mesh);
	auto particleInfo = static_cast<ParticleInfo*>(renderObject._renderInfo);

	if (particleInfo->_particleData._commonInfo._firstRun)
	{
		_commandBuffer->SetVertexBuffer(particleObject->_initBuffer);
	}
	else
	{
		_commandBuffer->SetVertexBuffer(particleObject->_vertexBuffer);
	}

	for (unsigned int submeshIndex = 0; submeshIndex < renderObject._materials->size(); submeshIndex++)
	{
		_commandBuffer->SetIndexBuffer(renderObject._mesh->_subMeshs[submeshIndex]._indexBuffer);
		_commandBuffer->UpdateBuffer(_perObjectBuffer, 0, _buffer->GetBufferStartPoint(), renderObjectBufferSize);

		if (_oitDrawCount == 0)
		{
			_commandBuffer->SetPipelineState(_streamOutShader);

			_currentBindTextures._resourceViews[0]._resource = _particleRandomTexture;
			_currentBindTextures._resourceViews[2]._resource = renderObject._materials->at(submeshIndex)->GetMetallicRoughnessMap();
			//2에 바인딩한 이유 -> 

			_commandBuffer->SetResources(renderPipeline->GetSamplerResourceViewLayout());
			_commandBuffer->SetResources(_currentBindBuffer);
			_commandBuffer->SetResources(_currentBindTextures);

			_commandBuffer->BeginStreamOutput(1, &particleObject->_streamOutBuffer);
			if (particleInfo->_particleData._commonInfo._firstRun)
			{
				_commandBuffer->Draw(renderObject._mesh->_subMeshs[submeshIndex]._drawIndex, 0);
				particleInfo->_particleData._commonInfo._firstRun = false;
			}
			else
			{
				_commandBuffer->DrawAuto();
			}
			_commandBuffer->EndStreamOutput();

			std::swap(particleObject->_vertexBuffer, particleObject->_streamOutBuffer);
		}

		auto currentMaterial = renderObject._materials->at(submeshIndex);
		currentMaterial->BindPipeline(_buffer.get(), renderObjectBufferSize, &_currentBindTextures);

		renderPipeline->ChangeTexture(static_cast<DUOLGraphicsLibrary::Texture*>(_currentBindTextures._resourceViews[0]._resource), 0);

		_commandBuffer->SetPipelineState(renderObject._materials->at(submeshIndex)->GetPipelineState());
		_commandBuffer->SetResources(renderPipeline->GetTextureResourceViewLayout());
		_commandBuffer->SetResources(renderPipeline->GetSamplerResourceViewLayout());

		_commandBuffer->SetVertexBuffer(particleObject->_vertexBuffer);
		_commandBuffer->DrawAuto();

		if ((particleInfo->_particleData.GetFlag() & static_cast<unsigned int>(BasicParticle::Flags::Trails)))
		{
			_commandBuffer->SetPipelineState(_particleTrailShader);

			_commandBuffer->DrawAuto();
		}
	}
}

void DUOLGraphicsEngine::RenderManager::BindBackBuffer(DUOLGraphicsLibrary::RenderPass* renderPass)
{
	//for IMGUI
	_commandBuffer->SetRenderPass(renderPass);
}

void DUOLGraphicsEngine::RenderManager::CreateCubeMapFromPanoramaImage(DUOLGraphicsLibrary::Texture* panorama, DUOLGraphicsLibrary::RenderTarget* cubeMap[6], DUOLGraphicsLibrary::PipelineState* pipelineState, DUOLGraphicsLibrary::RenderTarget* depth, DUOLGraphicsLibrary::Buffer* perObject, DUOLGraphicsLibrary::Sampler* LinearSampler)
{
	DUOLGraphicsLibrary::Viewport viewport(cubeMap[0]->GetResolution());
	_commandBuffer->SetViewport(viewport);

	_commandBuffer->SetVertexBuffer(_postProcessingRectVertex);
	_commandBuffer->SetIndexBuffer(_postProcessingRectIndex);

	DUOLGraphicsLibrary::ResourceViewLayout layout;
	layout._resourceViews.reserve(3);

	layout._resourceViews.emplace_back(perObject, 1, static_cast<long>(DUOLGraphicsLibrary::BindFlags::CONSTANTBUFFER), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));
	layout._resourceViews.emplace_back(panorama, 0, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));
	layout._resourceViews.emplace_back(LinearSampler, 0, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SAMPLER), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));

	DUOLGraphicsLibrary::RenderPass renderPass;
	renderPass._renderTargetViewRefs.resize(1);

	for (int idx = 0; idx < 6; idx++)
	{
		renderPass._renderTargetViewRefs[0] = cubeMap[idx];

		_commandBuffer->SetRenderTarget(cubeMap[idx], nullptr, 0);

		_commandBuffer->SetPipelineState(pipelineState);
		_commandBuffer->UpdateBuffer(perObject, 0, &idx, sizeof(int));
		_commandBuffer->SetResources(layout);

		_commandBuffer->DrawIndexed(GetNumIndicesFromBuffer(_postProcessingRectIndex), 0, 0);
	}

	_commandBuffer->Flush();
}

void DUOLGraphicsEngine::RenderManager::CreatePreFilteredMapFromCubeImage(
	DUOLGraphicsLibrary::Texture* cubeMap
	, DUOLGraphicsLibrary::RenderTarget** RadianceMap
	, DUOLGraphicsLibrary::PipelineState* pipelineState
	, DUOLGraphicsLibrary::RenderTarget* depth
	, DUOLGraphicsLibrary::Buffer* perObject, DUOLGraphicsLibrary::Sampler* linearSampler, UINT mipmapSize, float width, float
	height)
{
	_commandBuffer->SetVertexBuffer(_postProcessingRectVertex);
	_commandBuffer->SetIndexBuffer(_postProcessingRectIndex);

	DUOLGraphicsLibrary::ResourceViewLayout layout;
	layout._resourceViews.reserve(3);

	layout._resourceViews.emplace_back(perObject, 1, static_cast<long>(DUOLGraphicsLibrary::BindFlags::CONSTANTBUFFER), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));
	layout._resourceViews.emplace_back(cubeMap, 0, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));
	layout._resourceViews.emplace_back(linearSampler, 0, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SAMPLER), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));

	struct radianceData
	{
		int idx;
		float roughness;
	};

	for (UINT mipIdx = 0; mipIdx < mipmapSize; mipIdx++)
	{
		float texWidth = static_cast<float>(width * pow(0.5f, mipIdx));
		float texHeight = static_cast<float>(height * pow(0.5f, mipIdx));

		DUOLGraphicsLibrary::Viewport viewport({ texWidth, texHeight });
		_commandBuffer->SetViewport(viewport);

		for (int idx = 0; idx < 6; idx++)
		{
			radianceData data;

			data.idx = idx;
			data.roughness = static_cast<float>(mipIdx) / static_cast<float>(mipmapSize);

			_commandBuffer->SetRenderTarget(RadianceMap[6 * mipIdx + idx], nullptr, 0);

			_commandBuffer->SetPipelineState(pipelineState);
			_commandBuffer->UpdateBuffer(perObject, 0, &data, sizeof(radianceData));
			_commandBuffer->SetResources(layout);

			_commandBuffer->DrawIndexed(GetNumIndicesFromBuffer(_postProcessingRectIndex), 0, 0);
		}

		_commandBuffer->Flush();
	}
}

void DUOLGraphicsEngine::RenderManager::CreateBRDFLookUpTable(DUOLGraphicsLibrary::RenderTarget* BRDFLokUp, DUOLGraphicsLibrary::PipelineState* pipelineState, DUOLGraphicsLibrary::RenderTarget* depth, DUOLGraphicsLibrary::Buffer* perObject, UINT width, UINT height)
{
	DUOLGraphicsLibrary::Viewport viewport({ (float)width, (float)height });

	_commandBuffer->SetVertexBuffer(_postProcessingRectVertex);
	_commandBuffer->SetIndexBuffer(_postProcessingRectIndex);

	_commandBuffer->SetViewport(viewport);
	_commandBuffer->SetRenderTarget(BRDFLokUp, nullptr, 0);
	_commandBuffer->SetPipelineState(pipelineState);

	_commandBuffer->DrawIndexed(GetNumIndicesFromBuffer(_postProcessingRectIndex), 0, 0);

	_commandBuffer->Flush();
}

bool DUOLGraphicsEngine::RenderManager::GetRenderData(DUOLGraphicsLibrary::QueryInfo& outData)
{
	return _commandBuffer->GetData(outData);
}

void DUOLGraphicsEngine::RenderManager::OnResize(const DUOLMath::Vector2& resolution)
{
	_commandBuffer->SetViewport(resolution);
}

void DUOLGraphicsEngine::RenderManager::CopyTexture(DUOLGraphicsLibrary::Texture* destTexture,
	DUOLGraphicsLibrary::Texture* srcTexture)
{
	DUOLGraphicsLibrary::TextureLocation empty;

	_commandBuffer->CopyTexture(destTexture, empty, srcTexture, empty);
}

void DUOLGraphicsEngine::RenderManager::RenderDebug(RenderObject* object)
{
	_renderDebugQueue.emplace_back(object);
}

void DUOLGraphicsEngine::RenderManager::RegisterRenderQueue(const std::vector<RenderObject*>& renderObjects, const DUOLGraphicsEngine::ConstantBufferPerFrame& perFrameInfo)
{
	//TODO::
	//컬링 옵션에 따라 소프트 컬링(절두체)
	//혹은 정렬까지.. 여기서 하면 될 것 같은데?
	//근데 그림자 그리는건 어떻하지?
	// 렌더 큐를 등록하기 전, 기존 그래픽스 엔진에 있던 내역들을 모두 제거합니다.
	_transparencyRenderQueue.clear();
	_opaqueRenderQueue.clear();
	_renderDebugQueue.clear();

	Frustum frustum;

	CullingHelper::CreateFrustumFromCamera(perFrameInfo._camera, frustum);

	// 일단은 그냥 바로 큐에 넣는다.
	for (auto& renderObject : renderObjects)
	{
		switch (renderObject->_mesh->GetMeshType())
		{
		case MeshBase::MeshType::Particle:
		{
			_transparencyRenderQueue.emplace_back(renderObject);
			break;
		}
		case MeshBase::MeshType::Mesh:
		{
			auto info = static_cast<MeshInfo*>(renderObject->_renderInfo);
			auto transform = info->GetTransformPointer();

  			if(CullingHelper::ViewFrustumCulling(transform->_world, renderObject->_mesh->_halfExtents, frustum))
			{
				_opaqueRenderQueue.emplace_back(renderObject);
			}

			break;
		}
		case MeshBase::MeshType::SkinnedMesh:
		{
			auto info = static_cast<SkinnedMeshInfo*>(renderObject->_renderInfo);
			auto transform = info->GetTransformPointer();

			if (CullingHelper::ViewFrustumCulling(transform->_world, renderObject->_mesh->_halfExtents, frustum))
			{
				_opaqueRenderQueue.emplace_back(renderObject);
			}

			break;
		}
		default:;
		}
	}
}

void DUOLGraphicsEngine::RenderManager::Present()
{
	_context->Present();
}

void DUOLGraphicsEngine::RenderManager::Begin()
{
	_commandBuffer->Begin();
}

void DUOLGraphicsEngine::RenderManager::End()
{
	_commandBuffer->End();
}

bool DUOLGraphicsEngine::RenderManager::GetPipelineQueryInfo(DUOLGraphicsLibrary::QueryInfo& info)
{
	return _commandBuffer->GetData(info);
}

int DUOLGraphicsEngine::RenderManager::GetNumIndicesFromBuffer(DUOLGraphicsLibrary::Buffer* indexBuffer)
{
	auto bufferDesc = indexBuffer->GetBufferDesc();

	int ret = bufferDesc._size / bufferDesc._stride;

	return ret;
}

void DUOLGraphicsEngine::RenderManager::SetPerFrameBuffer(DUOLGraphicsLibrary::Buffer* frameBuffer, const ConstantBufferPerFrame& buffer)
{
	ConstantBufferPerFrame Infos = buffer;

	//Calc CascadeShadow
	//temp code
	//todo:: 라이팅 구조 개선할 것. (디렉셔널.. 포인트.. 스팟.. 라이트를 분리해야할 것 같음!)
	int lightIdx = 0;
	for (lightIdx = 0; lightIdx < 30; ++lightIdx)
	{
		if (buffer._light[lightIdx]._lightType == LightType::Direction)
			break;
	}

	float cascadeOffset[4]{ 0.08f, 0.2f, 0.6f, 1.0f };

	CascadeShadowSlice slice[4];
	ShadowHelper::CalculateCascadeShadowSlices(Infos, buffer._camera._cameraNear, buffer._camera._cameraFar, buffer._camera._cameraVerticalFOV, buffer._camera._aspectRatio, cascadeOffset, slice);
	for (int sliceIdx = 0; sliceIdx < 4; ++sliceIdx)
		ShadowHelper::CalcuateViewProjectionMatrixFromCascadeSlice(slice[sliceIdx], buffer._light[lightIdx]._direction, Infos._cascadeShadowInfo.shadowMatrix[sliceIdx]);

	Infos._cascadeShadowInfo._cascadeSliceOffset[0] = cascadeOffset[0];
	Infos._cascadeShadowInfo._cascadeSliceOffset[1] = cascadeOffset[1];
	Infos._cascadeShadowInfo._cascadeSliceOffset[2] = cascadeOffset[2];
	Infos._cascadeShadowInfo._cascadeSliceOffset[3] = cascadeOffset[3];
	_commandBuffer->UpdateBuffer(frameBuffer, 0, &Infos, sizeof(ConstantBufferPerFrame));
}

void DUOLGraphicsEngine::RenderManager::CreatePostProcessingRect()
{
	//TODO:: ??????? RESOUCEMANAGER?? ???????

	struct Rect
	{
		DUOLMath::Vector3 _position;
		DUOLMath::Vector2 _texcoord;
	};

	Rect rect[4];

	rect[0]._position = DUOLMath::Vector3{ -1.f, +1.f, +0.f };
	rect[1]._position = DUOLMath::Vector3{ +1.f, +1.f, +0.f };
	rect[2]._position = DUOLMath::Vector3{ -1.f, -1.f, +0.f };
	rect[3]._position = DUOLMath::Vector3{ +1.f, -1.f, +0.f };

	rect[0]._texcoord = DUOLMath::Vector2{ +0.f, +0.f };
	rect[1]._texcoord = DUOLMath::Vector2{ +1.f, +0.f };
	rect[2]._texcoord = DUOLMath::Vector2{ +0.f, +1.f };
	rect[3]._texcoord = DUOLMath::Vector2{ +1.f, +1.f };

	DUOLGraphicsLibrary::BufferDesc vertexbufferDesc;
	vertexbufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
	vertexbufferDesc._stride = sizeof(Rect);
	vertexbufferDesc._size = vertexbufferDesc._stride * 4;
	vertexbufferDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_UNKNOWN;
	vertexbufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::VERTEXBUFFER);

	_postProcessingRectVertex = _renderer->CreateBuffer(Hash::Hash64(_T("postProcessingRectVertex")), vertexbufferDesc, static_cast<void*>(rect));

	UINT32 indices[6] = { 0,3,2, 0,1,3 };

	DUOLGraphicsLibrary::BufferDesc indexbufferDesc;

	indexbufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
	indexbufferDesc._stride = sizeof(unsigned int);
	indexbufferDesc._size = indexbufferDesc._stride * 6;
	indexbufferDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32_UINT;
	indexbufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::INDEXBUFFER);

	_postProcessingRectIndex = _renderer->CreateBuffer(Hash::Hash64(_T("postProcessingRectIndex")), indexbufferDesc, static_cast<void*>(indices));
}
