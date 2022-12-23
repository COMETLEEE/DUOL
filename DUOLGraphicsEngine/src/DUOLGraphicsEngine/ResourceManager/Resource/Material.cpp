#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"

#include "DUOLGraphicsEngine/RenderManager/RenderingPipeline.h"
#include "DUOLGraphicsLibrary/Renderer/ResourceViewLayout.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"

bool DUOLGraphicsEngine::Material::BindPipeline(void* bufferStartPoint,
	DUOLGraphicsLibrary::ResourceViewLayout* resourceViewLayout)
{
	memcpy(bufferStartPoint, &_albedo, 48);

	resourceViewLayout->_resourceViews[0]._resource = _albedoMap;
	resourceViewLayout->_resourceViews[1]._resource = _normalMap;
	resourceViewLayout->_resourceViews[2]._resource = _metallicSmoothnessMap;

	return true;
}

DUOLGraphicsLibrary::PipelineState* DUOLGraphicsEngine::Material::GetPipelineState()
{
	return _pipelineState;
}

DUOLGraphicsEngine::RenderingPipeline* DUOLGraphicsEngine::Material::GetRenderingPipeline()
{
	return _renderingPipeline;
}

void DUOLGraphicsEngine::Material::SetAlbedoMap(DUOLGraphicsLibrary::Texture* albedo)
{
	_albedoMap = albedo;
}

void DUOLGraphicsEngine::Material::SetNormalMap(DUOLGraphicsLibrary::Texture* normal)
{
	_normalMap = normal;
}

void DUOLGraphicsEngine::Material::SetMetallicSmoothnessAOMap(DUOLGraphicsLibrary::Texture* MSAmap)
{
	_metallicSmoothnessMap = MSAmap;
}

void DUOLGraphicsEngine::Material::SetPipelineState(DUOLGraphicsLibrary::PipelineState* pipelineState)
{
	_pipelineState = pipelineState;
}

void DUOLGraphicsEngine::Material::SetRenderingPipeline(DUOLGraphicsEngine::RenderingPipeline* renderingPipeline)
{
	_renderingPipeline = renderingPipeline;
}
