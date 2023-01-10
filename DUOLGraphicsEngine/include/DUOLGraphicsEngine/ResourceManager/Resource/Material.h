#pragma once
#include <DUOLCommon/StringHelper.h>
#include "DUOLGraphicsEngine/Export.h"
#include "DUOLMath/DUOLMath.h"

#include <boost/serialization/string.hpp>

namespace DUOLGraphicsLibrary
{
	class Texture;
	class PipelineState;
	class ResourceViewLayout;
}

namespace DUOLGraphicsEngine
{
	class RenderingPipeline;

	//TODO: 바인드함수를 노출시키고싶지 않으므로 나중에 렌더매니저와 friend 등록시키자
	class IMaterial
	{
	public:
		virtual bool BindPipeline(void* bufferStartPoint, DUOLGraphicsLibrary::ResourceViewLayout* resourceViewLayout) abstract;

		virtual DUOLGraphicsLibrary::PipelineState* GetPipelineState() abstract;

		//renderPass
		virtual DUOLGraphicsEngine::RenderingPipeline* GetRenderingPipeline() abstract;

	};

	class DUOLGRAPHICSENGINE_EXPORT Material : public IMaterial
	{
	public:
		Material() :
			 _albedo(1.f, 1.f, 1.f, 1.f)
			, _albedoMap(nullptr)
			, _metallic(0.5f)
			, _roughness(0.5f)
			, _specular(0.5f)
			, _metallicRoughnessMap(nullptr)
			, _normalMap(nullptr)
		{

		}
	public:
		virtual bool BindPipeline(void* bufferStartPoint, DUOLGraphicsLibrary::ResourceViewLayout* resourceViewLayout) override;

		virtual DUOLGraphicsLibrary::PipelineState* GetPipelineState() override;

		//renderPass
		virtual DUOLGraphicsEngine::RenderingPipeline* GetRenderingPipeline() override;

		void SetAlbedo(DUOLMath::Vector4 albedo);

		void SetEmissive(DUOLMath::Vector3 emissive);

		void SetMetallic(float value);

		void SetRoughness(float value);

		void SetSpecular(float value);

		void SetAlbedoMap(DUOLGraphicsLibrary::Texture* albedo);

		void SetNormalMap(DUOLGraphicsLibrary::Texture* normal);

		void SetMetallicSmoothnessAOMap(DUOLGraphicsLibrary::Texture* MSAmap);

		void SetPipelineState(DUOLGraphicsLibrary::PipelineState* pipelineState);

		void SetRenderingPipeline(DUOLGraphicsEngine::RenderingPipeline* renderingPipeline);

	private:
		DUOLMath::Vector4 _albedo;

		DUOLMath::Vector3 _emissive;

		float _metallic;

		float _roughness;

		float _specular;

		DUOLMath::Vector2  _tiling;

		DUOLGraphicsLibrary::Texture* _albedoMap;

		DUOLGraphicsLibrary::Texture* _metallicRoughnessMap;

		DUOLGraphicsLibrary::Texture* _normalMap;

		//shader;
		DUOLGraphicsLibrary::PipelineState* _pipelineState;

		//renderPass
		DUOLGraphicsEngine::RenderingPipeline* _renderingPipeline;
	};

	struct MaterialDesc
	{
	public:
		MaterialDesc() :
			_materialName("")
			,_albedo(1.f, 1.f, 1.f, 1.f)
			, _albedoMap()
			, _metallic(0.5f)
			, _specular(0.5f)
			, _roughness(0.5f)
			, _isAlbedo(false)
			, _isNormal(false)
			, _isMetallic(false)
			, _isRoughness(false)
			, _metallicRoughnessMap()
			, _normalMap()
		{

		}

		friend class boost::serialization::access;

		std::string _materialName;

		DUOLMath::Vector4 _albedo;

		DUOLMath::Vector3 _emissive;

		float _metallic;

		float _roughness;

		float _specular;

		bool _isAlbedo;

		bool _isNormal;

		bool _isMetallic;

		bool _isRoughness;

		DUOLCommon::tstring _albedoMap;

		DUOLCommon::tstring _metallicRoughnessMap;

		DUOLCommon::tstring _normalMap;

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			// 연산자 재정의를 한듯
			ar& _materialName;

			ar& _isAlbedo;
			ar& _isNormal;
			ar& _isMetallic;
			ar& _isRoughness;

			ar& _albedoMap;
			ar& _normalMap;
			ar& _metallicRoughnessMap;

			ar& _albedo;
			ar& _emissive;

			ar& _metallic;
			ar& _roughness;
			ar& _specular;
		}

		//shader;
		DUOLCommon::tstring  _pipelineState;

		//renderPass
		DUOLCommon::tstring  _renderPipeline;

	};
}