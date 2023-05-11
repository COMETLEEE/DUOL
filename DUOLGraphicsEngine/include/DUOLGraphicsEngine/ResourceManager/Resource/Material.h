#pragma once
#include <DUOLCommon/StringHelper.h>
#include "DUOLGraphicsEngine/Export.h"
#include "DUOLMath/DUOLMath.h"

#include <boost/serialization/string.hpp>

typedef  unsigned __int64 uint64;

namespace DUOLGraphicsLibrary
{
	class Texture;
	class PipelineState;
	class ResourceViewLayout;	
}

namespace DUOLGraphicsEngine
{
	class ByteBuffer;
	class RenderingPipeline;

	//TODO: 바인드함수를 노출시키고싶지 않으므로 나중에 렌더매니저와 friend 등록시키자
	class IMaterial
	{
	public:
		virtual bool BindPipeline(ByteBuffer* buffer, DUOLGraphicsLibrary::ResourceViewLayout* resourceViewLayout, int bufferOffset) abstract;

		virtual int GetBindDataSize() abstract;
	};

	struct DUOLGRAPHICSENGINE_EXPORT DUOLMath::Vector4;
	struct DUOLGRAPHICSENGINE_EXPORT DUOLMath::Vector3;
	struct DUOLGRAPHICSENGINE_EXPORT DUOLMath::Vector2;

	class DUOLGRAPHICSENGINE_EXPORT Material : public IMaterial
	{
	public:
		enum class RenderingMode
		{
			Opaque
			, Transparency
		};

		struct BindData
		{
			BindData() :
				_albedo(1.f, 1.f, 1.f, 1.f)
				,_metallic(0.5f)
				,_roughness(0.5f)
				,_specular(0.5f)
			{
				
			}

		public:
			DUOLMath::Vector4 _albedo;

			DUOLMath::Vector3 _emissive;

			float _metallic;

			float _roughness;

			float _specular;

			DUOLMath::Vector2  _tiling;
		};

	public:
		Material() :
			_renderingMode(RenderingMode::Opaque)
			, _instanceRendering(false)
			, _materialData()
		{
			_textures.resize(3);
		}
	public:
		virtual bool BindPipeline(ByteBuffer* buffer, DUOLGraphicsLibrary::ResourceViewLayout* resourceViewLayout, int bufferOffset = 0) override;

		virtual int GetBindDataSize() override;

		void SetAlbedo(DUOLMath::Vector4 albedo);

		void SetEmissive(DUOLMath::Vector3 emissive);

		void SetMetallic(float value);

		void SetRoughness(float value);

		void SetSpecular(float value);

		const DUOLMath::Vector4& GetAlbedo();

		const float& GetMetallic();

		const float& GetRoughness();

		void SetTexture(DUOLGraphicsLibrary::Texture* texture, unsigned int slot);

		void SetAlbedoMap(DUOLGraphicsLibrary::Texture* albedo);

		void SetNormalMap(DUOLGraphicsLibrary::Texture* normal);

		void SetMetallicSmoothnessAOMap(DUOLGraphicsLibrary::Texture* MSAmap);

		RenderingMode GetRenderingMode() const;

		void SetMaterialData(const BindData& material_data);

		DUOLGraphicsLibrary::PipelineState* GetPipelineState() const;

		void SetPipelineState(DUOLGraphicsLibrary::PipelineState* pipelineState);

		std::vector<DUOLGraphicsLibrary::Texture*> GetTextures() const;

		bool IsInstanceRendering() const;

		void SetInstanceRendering(bool instanceRendering);


	private:

		BindData _materialData;

		//순서대로 바인딩됩니다.
		std::vector<DUOLGraphicsLibrary::Texture*> _textures;

		RenderingMode _renderingMode;

		bool _instanceRendering;

		DUOLGraphicsLibrary::PipelineState* _pipelineState;

	};

	struct MaterialDesc
	{
	public:
		MaterialDesc() :
			_materialName("")
			, _albedo(1.f, 1.f, 1.f, 1.f)
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

		uint64 _materialID;

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
			ar& _materialID;

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