#pragma once
#include <DUOLCommon/StringHelper.h>
#include "DUOLGraphicsEngine/Export.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderObject.h"
#include "DUOLMath/DUOLMath.h"

typedef  unsigned __int64 uint64;

namespace DUOLGraphicsLibrary
{
	class Texture;
	class PipelineState;
	class ResourceViewLayout;
}

namespace DUOLGraphicsEngine
{
	class ResourceManager;
	class ByteBuffer;
	class RenderingPipeline;

	struct MaterialDesc
	{
	public:
		MaterialDesc() :
			_materialName("")
			, _albedo(1.f, 1.f, 1.f, 1.f)
			, _metallic(0.5f)
			, _specular(0.5f)
			, _roughness(0.5f)
			, _isAlbedo(false)
			, _isNormal(false)
			, _isMetallic(false)
			, _isRoughness(false)
			, _isEmissive(false)
			, _albedoMap()
			, _metallicRoughnessMap()
			, _normalMap()
			, _emissiveMap()
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

		bool _isEmissive;

		DUOLCommon::tstring _albedoMap;

		DUOLCommon::tstring _metallicRoughnessMap;

		DUOLCommon::tstring _normalMap;

		DUOLCommon::tstring _emissiveMap;

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
			ar& _isEmissive;

			ar& _albedoMap;
			ar& _normalMap;
			ar& _metallicRoughnessMap;
			ar& _emissiveMap;

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
				, _metallic(0.5f)
				, _roughness(0.5f)
				, _specular(0.5f)
				, _tiling(1.f, 1.f)
				, _offset(0.f, 0.f)
				, _emissive(0.f, 0.f, 0.f)
				, _emissivePower(0.0039)
			{

			}

		public:
			DUOLMath::Vector4 _albedo;

			DUOLMath::Vector3 _emissive;

			float _emissivePower;

			float _roughness;

			float _specular;

			float _metallic;

			float _pad;

			DUOLMath::Vector2  _tiling;

			DUOLMath::Vector2 _offset;
		};

	public:
		Material(ResourceManager* resourceManager, MaterialDesc matDesc) :
			_renderingMode(RenderingMode::Opaque)
			, _instanceRendering(false)
			, _materialData()
			, _resourceManager(resourceManager)
			, _materialDesc(matDesc)
		{
			_textures.resize(4);
		}
	public:
		virtual bool BindPipeline(ByteBuffer* buffer, DUOLGraphicsLibrary::ResourceViewLayout* resourceViewLayout, int bufferOffset = 0) override;

		virtual bool BindTexture(DUOLGraphicsLibrary::ResourceViewLayout* resourceViewLayout);

		virtual int GetBindDataSize() override;

		void SetAlbedo(DUOLMath::Vector4 albedo);

		void SetEmissive(DUOLMath::Vector3 emissive);

		void SetEmissivePower(float emissviePower);

		void SetMetallic(float value);

		void SetRoughness(float value);

		void SetSpecular(float value);

		void SetTiling(DUOLMath::Vector2 value);

		void SetOffset(DUOLMath::Vector2 value);

		const DUOLMath::Vector4& GetAlbedo();

		const float& GetMetallic();

		const float& GetRoughness();

		const DUOLMath::Vector2& GetTiling();

		const DUOLMath::Vector2& GetOffset();

		const DUOLMath::Vector3& GetEmissive();

		float GetEmissivePower();

		void SetTexture(DUOLGraphicsLibrary::Texture* texture, unsigned int slot);

		void SetAlbedoMap(DUOLGraphicsLibrary::Texture* albedo);

		void SetNormalMap(DUOLGraphicsLibrary::Texture* normal);

		void SetMetallicSmoothnessAOMap(DUOLGraphicsLibrary::Texture* MSAmap);

		void SetEmissiveMap(DUOLGraphicsLibrary::Texture* emissive);

		RenderingMode GetRenderingMode() const;

		void SetMaterialData(const BindData& material_data);

		DUOLGraphicsLibrary::PipelineState* GetPipelineState() const;

		DUOLGraphicsLibrary::PipelineState* GetSecondPassPipelineState() const;

		void SetPipelineState(DUOLGraphicsLibrary::PipelineState* pipelineState);

		std::vector<DUOLGraphicsLibrary::Texture*> GetTextures() const;

		bool IsInstanceRendering() const;

		void SetInstanceRendering(bool instanceRendering);

		void SetRenderingMode(RenderingMode renderingMode, RenderObjectType renderObjectType);

		void SetSecondPassPipelineState(DUOLGraphicsLibrary::PipelineState* secondPassPipelineState);

		MaterialDesc GetMaterialDesc() const;

	private:

		BindData _materialData;

		//순서대로 바인딩됩니다.
		std::vector<DUOLGraphicsLibrary::Texture*> _textures;

		RenderingMode _renderingMode;

		bool _instanceRendering;

		DUOLGraphicsLibrary::PipelineState* _pipelineState;
		//for OIT 2PASS
		DUOLGraphicsLibrary::PipelineState* _secondPassPipelineState;

		MaterialDesc _materialDesc;

	private:
		DUOLGraphicsEngine::ResourceManager* _resourceManager;
	};

}