#pragma once
#include <DUOLCommon/StringHelper.h>
#include "DUOLGraphicsEngine/Export.h"
#include "DUOLMath/DUOLMath.h"

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
			, _metallic(0.f)
			, _roughness(0.5f)
			, _metallicSmoothnessMap(nullptr)
			, _normalMap(nullptr)
		{

		}
	public:
		virtual bool BindPipeline(void* bufferStartPoint, DUOLGraphicsLibrary::ResourceViewLayout* resourceViewLayout) override;

		virtual DUOLGraphicsLibrary::PipelineState* GetPipelineState() override;

		//renderPass
		virtual DUOLGraphicsEngine::RenderingPipeline* GetRenderingPipeline() override;

		void SetAlbedo(DUOLMath::Vector4 albedo);

		void SetAlbedoMap(DUOLGraphicsLibrary::Texture* albedo);

		void SetNormalMap(DUOLGraphicsLibrary::Texture* normal);

		void SetMetallicSmoothnessAOMap(DUOLGraphicsLibrary::Texture* MSAmap);

		void SetPipelineState(DUOLGraphicsLibrary::PipelineState* pipelineState);

		void SetRenderingPipeline(DUOLGraphicsEngine::RenderingPipeline* renderingPipeline);

	private:
		DUOLMath::Vector4 _albedo;

		DUOLMath::Vector4 _emissive;

		float _metallic;

		float _roughness;

		DUOLMath::Vector2  _tiling;

		DUOLGraphicsLibrary::Texture* _albedoMap;

		DUOLGraphicsLibrary::Texture* _metallicSmoothnessMap;

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
			_albedo(1.f, 1.f, 1.f, 1.f)
			, _albedoMap()
			, _metallic(0.5f)
			, _smoothness(0.5f)
			, _metallicSmoothnessMap()
			, _normalMap()
		{

		}

		DUOLMath::Vector4 _albedo;

		DUOLCommon::tstring _albedoMap;

		float _metallic;

		float _smoothness;

		DUOLCommon::tstring _metallicSmoothnessMap;

		DUOLCommon::tstring _normalMap;

		//shader;
		DUOLCommon::tstring  _pipelineState;

		//renderPass
		DUOLCommon::tstring  _renderPipeline;
	};
}