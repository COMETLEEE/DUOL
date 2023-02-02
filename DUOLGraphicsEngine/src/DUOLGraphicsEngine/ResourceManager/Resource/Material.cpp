#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"

#include "DUOLGraphicsEngine/RenderManager/RenderingPipeline.h"
#include "DUOLGraphicsEngine/Util/ByteBuffer.h"
#include "DUOLGraphicsLibrary/Renderer/ResourceViewLayout.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"

bool DUOLGraphicsEngine::Material::BindPipeline(ByteBuffer* buffer, int bufferOffset, DUOLGraphicsLibrary::ResourceViewLayout* resourceViewLayout)
{
	buffer->WriteData(&_materialData, sizeof(Material::BindData), bufferOffset);

	resourceViewLayout->_resourceViews[0]._resource = _albedoMap;
	resourceViewLayout->_resourceViews[1]._resource = _normalMap;
	resourceViewLayout->_resourceViews[2]._resource = _metallicRoughnessMap;

	return true;
}

int DUOLGraphicsEngine::Material::GetBindDataSize()
{
	return sizeof(Material::BindData);
}

DUOLGraphicsLibrary::PipelineState* DUOLGraphicsEngine::Material::GetPipelineState()
{
	return _pipelineState;
}

DUOLGraphicsEngine::RenderingPipeline* DUOLGraphicsEngine::Material::GetRenderingPipeline()
{
	return _renderingPipeline;
}

void DUOLGraphicsEngine::Material::SetAlbedo(DUOLMath::Vector4 albedo)
{
	_materialData._albedo = albedo;
}

void DUOLGraphicsEngine::Material::SetEmissive(DUOLMath::Vector3 emissive)
{
	_materialData._emissive = emissive;
}

void DUOLGraphicsEngine::Material::SetMetallic(float value)
{
	_materialData._metallic = value;
}

void DUOLGraphicsEngine::Material::SetRoughness(float value)
{
	_materialData._roughness = value;
}

void DUOLGraphicsEngine::Material::SetSpecular(float value)
{
	_materialData._specular = value;
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
	_metallicRoughnessMap = MSAmap;
}

void DUOLGraphicsEngine::Material::SetPipelineState(DUOLGraphicsLibrary::PipelineState* pipelineState)
{
	_pipelineState = pipelineState;
}

void DUOLGraphicsEngine::Material::SetRenderingPipeline(DUOLGraphicsEngine::RenderingPipeline* renderingPipeline)
{
	_renderingPipeline = renderingPipeline;
}