#include "DUOLGameEngine/ECS/Object/Material.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::Material>("Material")
	.constructor<>()
	(

	);
}

namespace DUOLGameEngine
{
	Material::Material(const DUOLCommon::tstring& name) :
		DUOLGameEngine::ObjectBase(name, ObjectType::Resource)
	{

	}

	Material::~Material()
	{

	}

	void Material::SetAlbedo(const DUOLMath::Vector4& albedo)
	{
		_primitiveMaterial->SetAlbedo(albedo);
	}

	void Material::SetMetaillic(const float metallic)
	{
		_primitiveMaterial->SetMetallic(metallic);
	}

	void Material::SetRoughness(const float roughness)
	{
		_primitiveMaterial->SetRoughness(roughness);
	}

	void Material::SetGPUInstancing(bool value)
	{
		_primitiveMaterial->SetInstanceRendering(value);
	}

	void Material::SetTiling(const DUOLMath::Vector2& value)
	{
		_primitiveMaterial->SetTiling(value);
	}

	void Material::SetOffset(const DUOLMath::Vector2& value)
	{
		_primitiveMaterial->SetOffset(value);
	}

	void Material::SetSurfaceType(SurfaceType surfaceType, DUOLGraphicsEngine::RenderObjectType objectType)
	{
		if (_primitiveMaterial == nullptr)
			return;

		switch (surfaceType) {
		case SurfaceType::Opaque:
			{
				_primitiveMaterial->SetRenderingMode(DUOLGraphicsEngine::Material::RenderingMode::Opaque, objectType);
			}
		break;
		case SurfaceType::Transparency:
			{
			_primitiveMaterial->SetRenderingMode(DUOLGraphicsEngine::Material::RenderingMode::Transparency, objectType);
			}
		break;
		default:;
		}
	}

	SurfaceType Material::GetSurfaceType()
	{
		switch (_primitiveMaterial->GetRenderingMode()) {
		case DUOLGraphicsEngine::Material::RenderingMode::Transparency:
			{
				return SurfaceType::Transparency;
			}
		case DUOLGraphicsEngine::Material::RenderingMode::Opaque:
		default: ;
		}

		return SurfaceType::Opaque;
	}

	bool Material::GetGPUInstancing()
	{
		return _primitiveMaterial->IsInstanceRendering();
	}

	const DUOLMath::Vector2& Material::GetTiling()
	{
		return _primitiveMaterial->GetTiling();
	}

	const DUOLMath::Vector2& Material::GetOffset()
	{
		return _primitiveMaterial->GetOffset();
	}

	const DUOLMath::Vector4& Material::GetAlbedo()
	{
		return _primitiveMaterial->GetAlbedo();
	}

	const float& Material::GetMetallic()
	{
		return _primitiveMaterial->GetMetallic();
	}

	const float& Material::GetRoughness()
	{
		return _primitiveMaterial->GetRoughness();
	}
}
