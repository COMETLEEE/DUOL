#include "RenderManager.h"

#include "CullingHelper.h"
#include "DUOLGraphicsEngine/RenderManager/RenderingPipeline/RenderingPipeline.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/PerlinNoise.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Vertex.h"
#include "DUOLGraphicsLibrary/Renderer/ResourceViewLayout.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"
#include "DUOLGraphicsEngine/Util/Hash/Hash.h"
#include "DUOLGraphicsLibrary/FontEngine/IFontEngine.h"
#include "DUOLGraphicsEngine/RenderManager/RenderingPipeline/RenderingPipeline.h"
#include "Renderer/OrderIndependentTransparencyRenderer.h"

DUOLGraphicsEngine::RenderManager::RenderManager(DUOLGraphicsLibrary::Renderer* renderer, DUOLGraphicsLibrary::RenderContext* context, DUOLGraphicsLibrary::Buffer* PerFrameBuffer, DUOLGraphicsLibrary::Buffer* PerCameraBuffer, DUOLGraphicsLibrary::Buffer* PerObjectBuffer) :
	_renderer(renderer)
	, _context(context)
	, _particleDrawCount(0)
	, _perFrameBuffer(PerFrameBuffer)
	, _perCameraBuffer(PerCameraBuffer)
	, _perObjectBuffer(PerObjectBuffer)
{
	DUOLGraphicsLibrary::CommandBufferDesc commandBufferDesc;

	_commandBuffer = _renderer->CreateCommandBuffer(0, commandBufferDesc);
	_buffer = std::make_unique<ByteBuffer>();

	CreatePostProcessingRect();
	ReserveResourceLayout();
	CreateAxis(renderer);
}

void DUOLGraphicsEngine::RenderManager::PushDebugObject(RenderObject* render_object)
{
	_debugRenderQueue.push_back(render_object);
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

void DUOLGraphicsEngine::RenderManager::ReserveResourceLayout()
{
	_perFrameBufferBinder._resourceViews.emplace_back(_perFrameBuffer, 0, static_cast<long>(DUOLGraphicsLibrary::BindFlags::CONSTANTBUFFER), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::COMPUTESTAGE));
	_perCameraBufferBinder._resourceViews.emplace_back(_perCameraBuffer, 1, static_cast<long>(DUOLGraphicsLibrary::BindFlags::CONSTANTBUFFER), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::COMPUTESTAGE));
	_perObjectBufferBinder._resourceViews.emplace_back(_perObjectBuffer, 2, static_cast<long>(DUOLGraphicsLibrary::BindFlags::CONSTANTBUFFER), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::COMPUTESTAGE));

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

void DUOLGraphicsEngine::RenderManager::ExecuteRenderingPipeline(RenderingPipeline* renderPipeline, const std::vector<DecomposedRenderData>& renderObjects, void* postProcessingData, int dataSize)
{
#if defined(_DEBUG) || defined(DEBUG)
	_renderer->BeginEvent(renderPipeline->GetName().c_str());
#endif
	switch (renderPipeline->GetPipelineType())
	{
	case PipelineType::Render:
	{
		ExecuteRenderPass(renderPipeline, renderObjects);
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
void DUOLGraphicsEngine::RenderManager::ExecuteDebugRenderPass(RenderingPipeline* renderPipeline)
{
#if defined(_DEBUG) || defined(DEBUG)
	_renderer->BeginEvent(renderPipeline->GetName().c_str());
#endif
	auto renderPass = renderPipeline->GetRenderPass();
	_commandBuffer->SetRenderPass(renderPass);
	if (renderPass->_renderTargetViewRefs.size() > 0)
	{
		DUOLGraphicsLibrary::Viewport viewport(renderPass->_renderTargetViewRefs[0]->GetResolution());
		_commandBuffer->SetViewport(viewport);
	}
	const size_t renderQueueSize = _debugRenderQueue.size();

	for (uint32_t renderIndex = 0; renderIndex < renderQueueSize; renderIndex++)
	{
		RenderObject& renderObject = *_debugRenderQueue[renderIndex];

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

		/*{
			_commandBuffer->SetVertexBuffer(_axisVertex);
			_commandBuffer->SetIndexBuffer(_axisIndex);

			Transform transform;

			_buffer->WriteData(&transform, sizeof(Transform));

			_commandBuffer->UpdateBuffer(_perObjectBuffer, 0, _buffer->GetBufferStartPoint(), sizeof(Transform) + 48);
			_commandBuffer->SetResources(renderPipeline->GetTextureResourceViewLayout());
			_commandBuffer->SetResources(renderPipeline->GetSamplerResourceViewLayout());

			_commandBuffer->DrawIndexed(6, 0, 0);
		}*/
	}

#if defined(_DEBUG) || defined(DEBUG)
	_renderer->EndEvent();
#endif

	_debugRenderQueue.clear();
	_commandBuffer->Flush();
}

void DUOLGraphicsEngine::RenderManager::ExecuteDebugRenderTargetPass(RenderingPipeline* renderPipeline)
{
	//_commandBuffer->SetRenderPass(renderPipeline->GetRenderPass());

	//_commandBuffer->SetPipelineState(renderPipeline->GetPipelineState());

	//_commandBuffer->SetVertexBuffer(_postProcessingRectVertex);
	//_commandBuffer->SetIndexBuffer(_postProcessingRectIndex);

	//_commandBuffer->SetResources(renderPipeline->GetTextureResourceViewLayout());

	//for (int renderTargets = 0; renderTargets < 4; renderTargets++)
	//{
	//	_commandBuffer->UpdateBuffer(_perObjectBuffer, 0, &renderTargets, sizeof(renderTargets));

	//	_commandBuffer->DrawIndexed(GetNumIndicesFromBuffer(_postProcessingRectIndex), 0, 0);
	//}

	//_commandBuffer->Flush();
}

void DUOLGraphicsEngine::RenderManager::OcclusionCulling(
	DUOLGraphicsEngine::OcclusionCulling* occlusionCulling,
	const std::vector<DecomposedRenderData>& inObjects
	, std::vector<DecomposedRenderData>& outObjects)
{
#if defined(_DEBUG) || defined(DEBUG)
	_renderer->BeginEvent(L"OcclusionCulling");
#endif

	//outObjects = inObjects;
	//DownSampling
	auto& renderTargetViews = occlusionCulling->GetMipmapRenderTargets();
	auto renderDepth = occlusionCulling->GetRenderDepth();
	auto mipmaptexture = occlusionCulling->GetMipmapDepth();

	_commandBuffer->SetPipelineState(occlusionCulling->GetDownSampling());

	_commandBuffer->SetVertexBuffer(_postProcessingRectVertex);
	_commandBuffer->SetIndexBuffer(_postProcessingRectIndex);

	_commandBuffer->SetResource(renderDepth, 0, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::PIXELSTAGE));

	_commandBuffer->SetResource(occlusionCulling->GetLinearSampler(), 0, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SAMPLER), static_cast<long>(DUOLGraphicsLibrary::StageFlags::PIXELSTAGE));

	auto currentRenderTarget = renderTargetViews.at(0);

	_commandBuffer->SetRenderTarget(currentRenderTarget, nullptr, 0);
	_commandBuffer->SetViewport(currentRenderTarget->GetResolution());
	_commandBuffer->DrawIndexed(GetNumIndicesFromBuffer(_postProcessingRectIndex), 0, 0);
	_commandBuffer->Flush();

	//밉맵텍스쳐 생성
	_renderer->GenerateMips(mipmaptexture);

	_commandBuffer->SetPipelineState(occlusionCulling->GetOcclusionCullingPipeline());
	_commandBuffer->SetResource(occlusionCulling->GetResultBuffer(), 0, static_cast<long>(DUOLGraphicsLibrary::BindFlags::UNORDEREDACCESS), static_cast<long>(DUOLGraphicsLibrary::StageFlags::COMPUTESTAGE));

	_commandBuffer->SetPipelineState(occlusionCulling->GetOcclusionCullingPipeline());

	//256개의 오브젝트를 처리합니다.
	long bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::UNORDEREDACCESS);
	long stageFlags = static_cast<long>(DUOLGraphicsLibrary::StageFlags::COMPUTESTAGE);

	auto extendBuffer = occlusionCulling->GetExtentBuffer();

	_commandBuffer->SetResource(occlusionCulling->GetResultBuffer(), 0, bindFlags, stageFlags);

	bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);
	_commandBuffer->SetResource(occlusionCulling->GetMipmapDepth(), 1, bindFlags, stageFlags);

	bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::SAMPLER);
	_commandBuffer->SetResource(occlusionCulling->GetLinearSampler(), 0, bindFlags, stageFlags);

	int objectCount = inObjects.size();
	int previousIdx = 0;

	for (int objectIdx = 0; objectIdx < objectCount; objectIdx += 256)
	{
		int endIdx = std::min<int>(objectCount, objectIdx + 256);
		int dataSizeIter = 0;

		for (int objidx = objectIdx; objidx < endIdx; objidx++)
		{
			auto objData = inObjects.at(objidx);

			_buffer->WriteData(&objData._worldTranslatedBoundingBox, sizeof(DecomposedRenderData::BoundingBox), sizeof(DecomposedRenderData::BoundingBox) * dataSizeIter);
			++dataSizeIter;
		}

		_commandBuffer->UpdateBuffer(extendBuffer, 0, _buffer->GetBufferStartPoint(), sizeof(DecomposedRenderData::BoundingBox) * dataSizeIter);
		bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);

		_commandBuffer->SetResource(extendBuffer, 0, bindFlags, stageFlags, 0);

		_commandBuffer->Dispatch(ceil(float(dataSizeIter) / 64.f), 1, 1);

		_commandBuffer->CopyBuffer(occlusionCulling->GetCpuBuffer(), 0, occlusionCulling->GetResultBuffer(), 0, 64 * 256);

		auto bufferPtr = _renderer->MapBuffer(occlusionCulling->GetCpuBuffer(), DUOLGraphicsLibrary::CPUAccessFlags::READ);

		struct DebugData
		{
			float screenSpaceExtent[4];
			float screenSpaceCenter[4];
			float xys[4];
			float maxDepth;
			float closestDepth;
			float LOD;
			float ID;
		};

		auto data = static_cast<DebugData*>(bufferPtr);

		int bufferIdx = 0;
		for (int objidx = 0; objidx < dataSizeIter; objidx++)
		{
			float isNotCulled = data[objidx].ID;
			if (isNotCulled > 0)
			{
				outObjects.emplace_back(inObjects.at(previousIdx + objidx));
			}
		}
		_renderer->UnmapBuffer(occlusionCulling->GetCpuBuffer());

		previousIdx = endIdx;
	}

	_commandBuffer->Flush();
#if defined(_DEBUG) || defined(DEBUG)
	_renderer->EndEvent();
#endif
}

void DUOLGraphicsEngine::RenderManager::RenderSkyBox(RenderingPipeline* skyBox, DUOLGraphicsLibrary::Buffer* vertices, DUOLGraphicsLibrary::Buffer* indices, const Camera& cameraInfo)
{
#if defined(_DEBUG) || defined(DEBUG)
	_renderer->BeginEvent(_T("SkyBox"));
#endif

	auto renderPass = skyBox->GetRenderPass();

	DUOLGraphicsLibrary::Viewport viewport(renderPass->_renderTargetViewRefs[0]->GetResolution());
	_commandBuffer->SetViewport(viewport);

	_commandBuffer->SetRenderPass(skyBox->GetRenderPass());

	_commandBuffer->SetPipelineState(skyBox->GetPipelineState());

	_commandBuffer->SetVertexBuffer(vertices);

	_commandBuffer->SetIndexBuffer(indices);

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

void DUOLGraphicsEngine::RenderManager::RenderCascadeShadow(DUOLGraphicsLibrary::PipelineState* shadowMesh, DUOLGraphicsLibrary::PipelineState* shadowSkinnedMesh, DUOLGraphicsLibrary::RenderTarget* shadowRenderTarget,
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
		auto objtype = renderObject->_mesh->GetMeshType();

		if (objtype == MeshBase::MeshType::Particle)
		{
			continue;
		}

		renderObject->_renderInfo->BindPipeline(_buffer.get());
		int renderObjectBufferSize = renderObject->_renderInfo->GetInfoStructureSize();

		_commandBuffer->UpdateBuffer(_perObjectBuffer, 0, _buffer->GetBufferStartPoint(), renderObjectBufferSize);

		_commandBuffer->SetVertexBuffer(renderObject->_mesh->_vertexBuffer);

		if (objtype == MeshBase::MeshType::Mesh)
		{
			_commandBuffer->SetPipelineState(shadowMesh);
		}
		else if (objtype == MeshBase::MeshType::SkinnedMesh)
		{
			_commandBuffer->SetPipelineState(shadowSkinnedMesh);
		}

		for (unsigned int submeshIndex = 0; submeshIndex < renderObject->_materials->size(); submeshIndex++)
		{
			if (renderObject->_mesh->GetSubMesh(submeshIndex) == nullptr)
				break;

			_commandBuffer->SetIndexBuffer(renderObject->_mesh->_subMeshs[submeshIndex]._indexBuffer);

			_commandBuffer->SetResources(_perObjectBufferBinder);
			_commandBuffer->SetResources(_currentBindTextures);

			_commandBuffer->DrawIndexed(renderObject->_mesh->_subMeshs[submeshIndex]._drawIndex, 0, 0);
		}

	}
#if defined(_DEBUG) || defined(DEBUG)
	_renderer->EndEvent();
#endif
}

void DUOLGraphicsEngine::RenderManager::RenderSpotShadow(DUOLGraphicsLibrary::PipelineState* shadowMesh,
	DUOLGraphicsLibrary::PipelineState* shadowSkinnedMesh, DUOLGraphicsLibrary::RenderTarget* shadowRenderTarget,
	const std::vector<RenderObject*>& renderObjects, int shadowIdx)
{
#if defined(_DEBUG) || defined(DEBUG)
	_renderer->BeginEvent(_T("Shadow"));
#endif

	const size_t renderQueueSize = renderObjects.size();
	_commandBuffer->SetRenderTarget(nullptr, shadowRenderTarget, 0);
	_commandBuffer->SetViewport(shadowRenderTarget->GetResolution());

	for (uint32_t renderIndex = 0; renderIndex < renderQueueSize; renderIndex++)
	{
		RenderObject* renderObject = renderObjects[renderIndex];

		renderObject->_renderInfo->BindPipeline(_buffer.get());
		int renderObjectBufferSize = renderObject->_renderInfo->GetInfoStructureSize();

		_buffer->WriteData(&shadowIdx, 4, 0);

		_commandBuffer->UpdateBuffer(_perObjectBuffer, 0, _buffer->GetBufferStartPoint(), renderObjectBufferSize);
		_commandBuffer->SetResources(_perObjectBufferBinder);

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
			_commandBuffer->DrawIndexed(renderObject->_mesh->_subMeshs[submeshIndex]._drawIndex, 0, 0);
		}
	}
#if defined(_DEBUG) || defined(DEBUG)
	_renderer->EndEvent();
#endif
}

void DUOLGraphicsEngine::RenderManager::RenderCanvas(RenderingPipeline* uiRenderer,
	DUOLGraphicsLibrary::Texture* canvas)
{
	auto renderPass = uiRenderer->GetRenderPass();
	DUOLGraphicsLibrary::Viewport viewport(renderPass->_renderTargetViewRefs[0]->GetResolution());
	_commandBuffer->SetViewport(viewport);
	_commandBuffer->SetRenderPass(renderPass);

	_commandBuffer->SetPipelineState(uiRenderer->GetPipelineState());

	_commandBuffer->SetVertexBuffer(_postProcessingRectVertex);
	_commandBuffer->SetIndexBuffer(_postProcessingRectIndex);

	_commandBuffer->SetResources(uiRenderer->GetSamplerResourceViewLayout());
	_commandBuffer->SetResource(canvas, 0, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::PIXELSTAGE));

	_commandBuffer->DrawIndexed(GetNumIndicesFromBuffer(_postProcessingRectIndex), 0, 0);
}

void DUOLGraphicsEngine::RenderManager::ExecutePostProcessingPass(RenderingPipeline* renderPipeline, void* postProcessingData, int dataSize)
{

	auto renderPass = renderPipeline->GetRenderPass();
	DUOLGraphicsLibrary::Viewport viewport(renderPass->_renderTargetViewRefs[0]->GetResolution());
	_commandBuffer->SetViewport(viewport);
	_commandBuffer->SetRenderPass(renderPass);

	_commandBuffer->SetPipelineState(renderPipeline->GetPipelineState());
	if (postProcessingData != nullptr)
	{
		if (dataSize > 0)
		{
			//todo:: buffer을 가변적으로 바꿀때 바꿔줘야하는 코드입니다.
			_buffer->WriteData(postProcessingData, dataSize);
			_commandBuffer->UpdateBuffer(_perObjectBuffer, 0, _buffer->GetBufferStartPoint(), dataSize);
			_commandBuffer->SetResources(_perObjectBufferBinder);
		}
	}
	_commandBuffer->SetVertexBuffer(_postProcessingRectVertex);
	_commandBuffer->SetIndexBuffer(_postProcessingRectIndex);

	_commandBuffer->SetResources(renderPipeline->GetTextureResourceViewLayout());
	_commandBuffer->SetResources(renderPipeline->GetSamplerResourceViewLayout());

	_commandBuffer->DrawIndexed(GetNumIndicesFromBuffer(_postProcessingRectIndex), 0, 0);

}

void DUOLGraphicsEngine::RenderManager::ExecuteTransparencyPass(RenderingPipeline* renderPipeline)
{
	///// OIT?? ???? Pass ?????? ???? ??? ????.
	//// ??????? 2?? ????? ?? ??????..?
	//_commandBuffer->SetRenderPass(renderPipeline->GetRenderPass());

	//const size_t renderQueueSize = _transparencyRenderQueue.size();

	//for (uint32_t renderIndex = 0; renderIndex < renderQueueSize; renderIndex++)
	//{
	//	RenderObject* renderObject = _transparencyRenderQueue[renderIndex];

	//	RenderParticle(*renderObject, renderPipeline);
	//}
	//_oitDrawCount++;
	//if (_oitDrawCount == 6)
	//	_oitDrawCount = 0;
}

void DUOLGraphicsEngine::RenderManager::RenderMesh(
	DecomposedRenderData& renderObject
	, RenderingPipeline* renderPipeline)
{
	int renderObjectBufferSize = renderObject._renderInfo->GetInfoStructureSize();

	//버텍스 버퍼 바인딩
	_commandBuffer->SetVertexBuffer(renderObject._mesh->_vertexBuffer);

	//인덱스 버퍼 바인딩
	_commandBuffer->SetIndexBuffer(renderObject._subMesh->_indexBuffer);

	//셰이더 바인딩
	_commandBuffer->SetPipelineState(renderObject._material->GetPipelineState());

	//constantBuffer Update
	renderObject._renderInfo->BindPipeline(_buffer.get());
	renderObject._material->BindPipeline(_buffer.get(), renderObjectBufferSize, &_currentBindTextures);
	_commandBuffer->UpdateBuffer(_perObjectBuffer, 0, _buffer->GetBufferStartPoint(), renderObjectBufferSize + renderObject._material->GetBindDataSize());

	//머터리얼에 등록된 텍스쳐를 바인딩한다.
	_commandBuffer->SetResources(_currentBindTextures);
	//perObjectBuffer Binding
	_commandBuffer->SetResources(_perObjectBufferBinder);

	_commandBuffer->DrawIndexed(renderObject._subMesh->_drawIndex, 0, 0);

}

void DUOLGraphicsEngine::RenderManager::RenderParticle(DecomposedRenderData& renderObject,
                                                       RenderingPipeline* renderPipeline)
{
	auto particleMesh = static_cast<ParticleBuffer*>(renderObject._mesh);
	auto particleInfo = static_cast<ParticleInfo*>(renderObject._renderInfo);

	_renderer->ClearUnorderedAccessView(particleMesh->_counterBuffer);
	// Update stage
	unsigned int flag = particleInfo->_particleData._flag;
	if (!(flag & static_cast<unsigned int>(ParticleFlags::ParticleSystemCommonInfo))) return;
	if (!(flag & static_cast<unsigned int>(ParticleFlags::Emission))) return;
	if (!(flag & static_cast<unsigned int>(ParticleFlags::Renderer))) return;

	//리소스 바인딩
	auto& updateLayout = _oitRenderer->GetParticleUpdateLayout();

	updateLayout._resourceViews[1]._resource = renderObject._material[0].GetTextures()[2];
	updateLayout._resourceViews[2]._resource = particleMesh->_particleBuffer;
	updateLayout._resourceViews[3]._resource = particleMesh->_counterBuffer;

	int renderObjectBufferSize = renderObject._renderInfo->GetInfoStructureSize();
	renderObject._renderInfo->BindPipeline(_buffer.get());
	_commandBuffer->UpdateBuffer(_perObjectBuffer, 0, _buffer->GetBufferStartPoint(), renderObjectBufferSize);
	_commandBuffer->SetResources(_perObjectBufferBinder);

	_commandBuffer->SetResources(updateLayout);
	_commandBuffer->SetResources(renderPipeline->GetSamplerResourceViewLayout());

	_commandBuffer->SetPipelineState(renderObject._material->GetPipelineState());

	_commandBuffer->Dispatch(particleInfo->_particleData._dim, particleInfo->_particleData._dim, particleInfo->_particleData._dim);

	_commandBuffer->Flush();

	//draw Stage
	auto& drawLayout = _oitRenderer->GetParticleDrawLayout();

	drawLayout._resourceViews[1]._resource = renderObject._material[0].GetTextures()[2];
	drawLayout._resourceViews[2]._resource = particleMesh->_particleBuffer;
	drawLayout._resourceViews[3]._resource = renderObject._material[0].GetTextures()[0];

	if (_particleDrawCount == 0)
	{
		drawLayout._resourceViews[5]._initCount = 0;
		drawLayout._resourceViews[6]._initCount = 0;

	}
	else
	{
		drawLayout._resourceViews[5]._initCount = -1;
		drawLayout._resourceViews[6]._initCount = -1;
	}

	_commandBuffer->SetResources(drawLayout);

	//particle
	{
		_commandBuffer->SetResources(renderPipeline->GetSamplerResourceViewLayout());
		_commandBuffer->SetVertexBuffer(renderObject._mesh->_vertexBuffer);
		_commandBuffer->SetPipelineState(_oitRenderer->GetParticleShader());
		_commandBuffer->Draw(particleInfo->_particleData._commonInfo.gMaxParticles, 0);
	}

	//trail
	if(flag & static_cast<unsigned int>(ParticleFlags::Trails))
	{
		_commandBuffer->SetPipelineState(_oitRenderer->GetParticleTrailShader());
		_commandBuffer->Draw(particleInfo->_particleData._commonInfo.gMaxParticles, 0);
	}

	//바인딩해제
	_commandBuffer->Flush();
	_particleDrawCount++;
}

void DUOLGraphicsEngine::RenderManager::BindBackBuffer(DUOLGraphicsLibrary::RenderPass* renderPass)
{
	//for IMGUI
	DUOLGraphicsLibrary::Viewport viewport(renderPass->_renderTargetViewRefs[0]->GetResolution());
	_commandBuffer->SetViewport(viewport);
	_commandBuffer->SetRenderPass(renderPass);
}

void DUOLGraphicsEngine::RenderManager::CreateCubeMapFromPanoramaImage(DUOLGraphicsLibrary::Texture* panorama, DUOLGraphicsLibrary::RenderTarget* cubeMap[6], DUOLGraphicsLibrary::PipelineState* pipelineState, DUOLGraphicsLibrary::Buffer* perObject, DUOLGraphicsLibrary::Sampler* LinearSampler)
{
	DUOLGraphicsLibrary::Viewport viewport(cubeMap[0]->GetResolution());
	_commandBuffer->SetViewport(viewport);

	_commandBuffer->SetVertexBuffer(_postProcessingRectVertex);
	_commandBuffer->SetIndexBuffer(_postProcessingRectIndex);

	DUOLGraphicsLibrary::ResourceViewLayout layout;
	layout._resourceViews.reserve(2);

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
		_commandBuffer->SetResources(_perObjectBufferBinder);

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
	layout._resourceViews.reserve(2);

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
			_commandBuffer->SetResources(_perObjectBufferBinder);

			_commandBuffer->DrawIndexed(GetNumIndicesFromBuffer(_postProcessingRectIndex), 0, 0);
		}
	}

	_commandBuffer->Flush();
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

void DUOLGraphicsEngine::RenderManager::SetPerCameraBuffer(ConstantBufferPerCamera& perCameraBuffer, const ConstantBufferPerFrame& perFrameBuffer)
{
	_commandBuffer->UpdateBuffer(_perCameraBuffer, 0, &perCameraBuffer, sizeof(ConstantBufferPerCamera));
	_commandBuffer->SetResource(_perCameraBuffer, 1, static_cast<long>(DUOLGraphicsLibrary::BindFlags::CONSTANTBUFFER), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::COMPUTESTAGE));
}

void DUOLGraphicsEngine::RenderManager::OnResize(const DUOLMath::Vector2& resolution)
{
	_commandBuffer->Flush();
}

void DUOLGraphicsEngine::RenderManager::CopyTexture(DUOLGraphicsLibrary::Texture* destTexture,
	DUOLGraphicsLibrary::Texture* srcTexture)
{
	DUOLGraphicsLibrary::TextureLocation empty;

	_commandBuffer->CopyTexture(destTexture, empty, srcTexture, empty);
}

void DUOLGraphicsEngine::RenderManager::Present()
{
	_context->Present();
}

void DUOLGraphicsEngine::RenderManager::Begin()
{
	_commandBuffer->Begin();
}

void DUOLGraphicsEngine::RenderManager::ClearState()
{
	_commandBuffer->ClearState();
}

void DUOLGraphicsEngine::RenderManager::End()
{
	_commandBuffer->End();
}

void DUOLGraphicsEngine::RenderManager::SetOITRenderer(
	DUOLGraphicsEngine::OrderIndependentTransparencyRenderer* oitRenderer)
{
	_oitRenderer = oitRenderer;
}

void DUOLGraphicsEngine::RenderManager::ClearOITUAVs()
{
#if defined(_DEBUG) || defined(DEBUG)
	_renderer->BeginEvent(L"ClearOITUAV");
#endif

	unsigned int clearNum[4] = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };

	_renderer->ClearUnorderedAccessView(_oitRenderer->GetOITLayerBuffer(), clearNum);
	_renderer->ClearUnorderedAccessView(_oitRenderer->GetFirstOffsetBuffer(), clearNum);

	_particleDrawCount = 0;

#if defined(_DEBUG) || defined(DEBUG)
	_renderer->EndEvent();
#endif
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

void DUOLGraphicsEngine::RenderManager::SetPerFrameBuffer(const ConstantBufferPerFrame& buffer)
{
	_commandBuffer->UpdateBuffer(_perFrameBuffer, 0, &buffer, sizeof(ConstantBufferPerFrame));
	_commandBuffer->SetResource(_perFrameBuffer, 0, static_cast<long>(DUOLGraphicsLibrary::BindFlags::CONSTANTBUFFER), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::COMPUTESTAGE));
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

void DUOLGraphicsEngine::RenderManager::ExecuteRenderPass(
	RenderingPipeline* renderPipeline,
	const std::vector<DecomposedRenderData>& renderObjects)
{

	auto renderPass = renderPipeline->GetRenderPass();
	if (renderPass->_renderTargetViewRefs.size() > 0)
	{
		DUOLGraphicsLibrary::Viewport viewport(renderPass->_renderTargetViewRefs[0]->GetResolution());
		_commandBuffer->SetViewport(viewport);
	}
	_commandBuffer->SetRenderPass(renderPass);
	_commandBuffer->SetResources(renderPipeline->GetSamplerResourceViewLayout());

	const size_t renderQueueSize = renderObjects.size();

	for (uint32_t renderIndex = 0; renderIndex < renderQueueSize; renderIndex++)
	{
		auto renderObject = renderObjects[renderIndex];

		switch (renderObject._renderInfo->GetRenderObjectType())
		{
		case RenderObjectType::Mesh:
		case RenderObjectType::Skinned:
		{
			RenderMesh(renderObject, renderPipeline);
			break;
		}
		case RenderObjectType::Particle:
		{
			RenderParticle(renderObject, renderPipeline);
			break;
		}
		default:;
		}
	}

}

