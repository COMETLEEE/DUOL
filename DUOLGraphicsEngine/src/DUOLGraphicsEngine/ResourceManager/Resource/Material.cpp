#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"

#include "DUOLGraphicsEngine/RenderManager/RenderingPipeline//RenderingPipeline.h"
#include "DUOLGraphicsEngine/ResourceManager/ResourceManager.h"
#include "DUOLGraphicsEngine/Util/ByteBuffer.h"
#include "DUOLGraphicsEngine/Util/Hash/Hash.h"
#include "DUOLGraphicsLibrary/Renderer/ResourceViewLayout.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"

bool DUOLGraphicsEngine::Material::BindPipeline(ByteBuffer* buffer, DUOLGraphicsLibrary::ResourceViewLayout* resourceViewLayout, int bufferOffset)
{
	buffer->WriteData(&_materialData, sizeof(Material::BindData), bufferOffset);

	int textureSize = _textures.size();
	int resourceViewSize = resourceViewLayout->_resourceViews.size();

	if (resourceViewSize < textureSize)
	{
		resourceViewLayout->_resourceViews.resize(textureSize);
	}

	for (int textureIdx = 0; textureIdx < textureSize; ++textureIdx)
	{
		resourceViewLayout->_resourceViews[textureIdx]._resource = _textures[textureIdx];
	}

	return true;
}

bool DUOLGraphicsEngine::Material::BindTexture(DUOLGraphicsLibrary::ResourceViewLayout* resourceViewLayout)
{
	int textureSize = _textures.size();
	int resourceViewSize = resourceViewLayout->_resourceViews.size();

	if (resourceViewSize < textureSize)
	{
		resourceViewLayout->_resourceViews.resize(textureSize);
	}

	for (int textureIdx = 0; textureIdx < textureSize; ++textureIdx)
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
	emissive.Clamp({ 0, 0, 0 }, { 255.f, 255.f, 255.f });
	
	_materialData._emissive = emissive / 255.f;
}

void DUOLGraphicsEngine::Material::SetEmissivePower(float emissviePower)
{
	_materialData._emissivePower = emissviePower / 255.f;
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

void DUOLGraphicsEngine::Material::SetTiling(DUOLMath::Vector2 value)
{
	_materialData._tiling = value;
}

void DUOLGraphicsEngine::Material::SetOffset(DUOLMath::Vector2 value)
{
	_materialData._offset = value;
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


const DUOLMath::Vector2& DUOLGraphicsEngine::Material::GetTiling()
{
	return _materialData._tiling;
}

const DUOLMath::Vector2& DUOLGraphicsEngine::Material::GetOffset()
{
	return _materialData._offset;
}

const DUOLMath::Vector3 DUOLGraphicsEngine::Material::GetEmissive()
{
	return _materialData._emissive * 255.f;
}

float DUOLGraphicsEngine::Material::GetEmissivePower()
{
	return _materialData._emissivePower * 255.f;
}

void DUOLGraphicsEngine::Material::SetTexture(DUOLGraphicsLibrary::Texture* texture, unsigned slot)
{
	auto texturesSize = _textures.size();

	if (texturesSize <= slot)
		_textures.resize(slot + 1);

	_textures[slot] = texture;
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

void DUOLGraphicsEngine::Material::SetEmissiveMap(DUOLGraphicsLibrary::Texture* emissive)
{
	_textures[3] = emissive;
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

bool DUOLGraphicsEngine::Material::IsInstanceRendering() const
{
	return _instanceRendering;
}

void DUOLGraphicsEngine::Material::SetInstanceRendering(bool instanceRendering)
{
	_instanceRendering = instanceRendering;
}

void DUOLGraphicsEngine::Material::SetRenderingMode(RenderingMode renderingMode, RenderObjectType renderObjectType)
{
	_renderingMode = renderingMode;

	if (renderObjectType == RenderObjectType::Mesh)
	{
		switch (renderingMode)
		{
		case RenderingMode::Opaque:
		{
			if (_materialDesc._isAlbedo && _materialDesc._isNormal && (_materialDesc._isMetallic || _materialDesc._isRoughness) && _materialDesc._isEmissive)
			{
				_materialDesc._pipelineState = _T("AlbedoNormalMRAEmissive");
			}
			else if (_materialDesc._isAlbedo && _materialDesc._isNormal && (_materialDesc._isMetallic || _materialDesc._isRoughness))
			{
				_materialDesc._pipelineState = _T("AlbedoNormalMRA");
			}
			else if (_materialDesc._isAlbedo && _materialDesc._isNormal)
			{
				_materialDesc._pipelineState = _T("AlbedoNormal");
			}
			else if (_materialDesc._isAlbedo)
			{
				_materialDesc._pipelineState = _T("Albedo");
			}
			else
			{
				_materialDesc._pipelineState = _T("Default");
			}
		}
		break;
		case RenderingMode::Transparency:
		{
			if (_materialDesc._isAlbedo && _materialDesc._isNormal && (_materialDesc._isMetallic || _materialDesc._isRoughness))
			{
				_materialDesc._pipelineState = _T("TransparencyAlbedoNormalMRA");
				_secondPassPipelineState = _resourceManager->GetPipelineState(Hash::Hash64(_T("GBufferTransparencyAlbedoNormalMRA")));
			}
			else if (_materialDesc._isAlbedo && _materialDesc._isNormal)
			{
				_materialDesc._pipelineState = _T("TransparencyAlbedoNormal");
				_secondPassPipelineState = _resourceManager->GetPipelineState(Hash::Hash64(_T("GBufferTransparencyAlbedoNormal")));
			}
			else if (_materialDesc._isAlbedo)
			{
				_materialDesc._pipelineState = _T("TransparencyAlbedo");
				_secondPassPipelineState = _resourceManager->GetPipelineState(Hash::Hash64(_T("GBufferTransparencyAlbedo")));
			}
			else
			{
				_materialDesc._pipelineState = _T("TransparencyDefault");
				_secondPassPipelineState = _resourceManager->GetPipelineState(Hash::Hash64(_T("GBufferTransparencyDefault")));
			}
		}
		break;
		default:;
		}
	}
	else if (renderObjectType == RenderObjectType::Skinned)
	{
		switch (renderingMode)
		{
		case RenderingMode::Opaque:
		{
			if (_materialDesc._isAlbedo && _materialDesc._isNormal && (_materialDesc._isMetallic || _materialDesc._isRoughness) && _materialDesc._isEmissive)
			{
				_materialDesc._pipelineState = _T("SkinnedAlbedoNormalMRAEmissive");
			}
			else if (_materialDesc._isAlbedo && _materialDesc._isNormal && (_materialDesc._isMetallic || _materialDesc._isRoughness))
			{
				_materialDesc._pipelineState = _T("SkinnedAlbedoNormalMRA");
				_secondPassPipelineState = _resourceManager->GetPipelineState(Hash::Hash64(_T("GBufferSkinnedAlbedoNormalMRA")));
			}
			else if (_materialDesc._isAlbedo && _materialDesc._isNormal)
			{
				_materialDesc._pipelineState = _T("SkinnedAlbedoNormal");
				_secondPassPipelineState = _resourceManager->GetPipelineState(Hash::Hash64(_T("GBufferSkinnedAlbedoNormal")));
			}
			else if (_materialDesc._isAlbedo)
			{
				_materialDesc._pipelineState = _T("SkinnedAlbedo");
				_secondPassPipelineState = _resourceManager->GetPipelineState(Hash::Hash64(_T("GBufferSkinnedAlbedo")));
			}
			else
			{
				_materialDesc._pipelineState = _T("SkinnedDefault");
				_secondPassPipelineState = _resourceManager->GetPipelineState(Hash::Hash64(_T("GBufferSkinnedDefault")));
			}
		}
		break;
		case RenderingMode::Transparency:
		{
		}
		break;
		default:;
		}
	}

	auto foundObj = _resourceManager->GetPipelineState(Hash::Hash64(_materialDesc._pipelineState));
	if (foundObj != nullptr)
	{
	}

	_pipelineState = foundObj;
}

DUOLGraphicsLibrary::PipelineState* DUOLGraphicsEngine::Material::GetSecondPassPipelineState() const
{
	return _secondPassPipelineState;
}

void DUOLGraphicsEngine::Material::SetSecondPassPipelineState(
	DUOLGraphicsLibrary::PipelineState* secondPassPipelineState)
{
	_secondPassPipelineState = secondPassPipelineState;
}

DUOLGraphicsEngine::MaterialDesc DUOLGraphicsEngine::Material::GetMaterialDesc() const
{
	return _materialDesc;
}

DUOLGraphicsLibrary::PipelineState* DUOLGraphicsEngine::Material::GetPipelineState() const
{
	return _pipelineState;
}

