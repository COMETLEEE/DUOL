#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"

#include "DUOLGraphicsEngine/RenderManager/RenderingPipeline//RenderingPipeline.h"
#include "DUOLGraphicsEngine/Util/ByteBuffer.h"
#include "DUOLGraphicsLibrary/Renderer/ResourceViewLayout.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"

bool DUOLGraphicsEngine::Material::BindPipeline(ByteBuffer* buffer, int bufferOffset, DUOLGraphicsLibrary::ResourceViewLayout* resourceViewLayout)
{
	buffer->WriteData(&_materialData, sizeof(Material::BindData), bufferOffset);

	int textureSize = _textures.size();
	int resourceViewSize = resourceViewLayout->_resourceViews.size();

	if(resourceViewSize < textureSize)
	{
		resourceViewLayout->_resourceViews.resize(textureSize);
	}

	for(int textureIdx = 0; textureIdx < textureSize; ++textureIdx)
	{
		resourceViewLayout->_resourceViews[textureIdx]._resource = _textures[textureIdx];
	}

	return true;
}

int DUOLGraphicsEngine::Material::GetBindDataSize()
{
	return sizeof(Material::BindData);
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
	_materialData._metallic = std::clamp(value, 0.f, 1.f);
}

void DUOLGraphicsEngine::Material::SetRoughness(float value)
{
	_materialData._roughness = std::clamp(value, 0.f, 1.f);;
}

void DUOLGraphicsEngine::Material::SetSpecular(float value)
{
	_materialData._specular = value;
}

const DUOLMath::Vector4& DUOLGraphicsEngine::Material::GetAlbedo()
{
	return _materialData._albedo;
}

const float& DUOLGraphicsEngine::Material::GetMetallic()
{
	return _materialData._metallic;
}

const float& DUOLGraphicsEngine::Material::GetRoughness()
{
	return _materialData._roughness;
}

void DUOLGraphicsEngine::Material::SetTexture(DUOLGraphicsLibrary::Texture* texture, unsigned slot)
{
	auto texturesSize = _textures.size();


}

void DUOLGraphicsEngine::Material::SetAlbedoMap(DUOLGraphicsLibrary::Texture* albedo)
{
	_textures[0] = albedo;
}

void DUOLGraphicsEngine::Material::SetNormalMap(DUOLGraphicsLibrary::Texture* normal)
{
	_textures[1] = normal;
}

void DUOLGraphicsEngine::Material::SetMetallicSmoothnessAOMap(DUOLGraphicsLibrary::Texture* MSAmap)
{
	_textures[2] = MSAmap;
}

DUOLGraphicsEngine::Material::RenderingMode DUOLGraphicsEngine::Material::GetRenderingMode() const
{
	return _renderingMode;
}

void DUOLGraphicsEngine::Material::SetMaterialData(const BindData& material_data)
{
	_materialData = material_data;
}


void DUOLGraphicsEngine::Material::SetPipelineState(DUOLGraphicsLibrary::PipelineState* pipelineState)
{
	_pipelineState = pipelineState;
}

std::vector<DUOLGraphicsLibrary::Texture*> DUOLGraphicsEngine::Material::GetTextures() const
{
	return _textures;
}

DUOLGraphicsLibrary::PipelineState* DUOLGraphicsEngine::Material::GetPipelineState() const
{
	return _pipelineState;
}

