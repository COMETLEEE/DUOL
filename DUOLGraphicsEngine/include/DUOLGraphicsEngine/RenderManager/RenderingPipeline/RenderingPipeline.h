#pragma once
#include "DUOLGraphicsEngine/Export.h"
#include "DUOLCommon/StringHelper.h"
#include "DUOLGraphicsLibrary/PipelineStateFlags.h"
#include "DUOLGraphicsLibrary/RenderPassFlags.h"
#include "DUOLGraphicsLibrary/Renderer/ResourceViewLayout.h"
#include "DUOLGraphicsLibrary/RenderTargetFlags.h"
#include "DUOLGraphicsLibrary/TextureFlags.h"
#include "DUOLGraphicsLibrary/Renderer/RenderPass.h"
#include <vector>

namespace DUOLGraphicsLibrary
{
	class RenderTarget;
	class Shader;
	class PipelineState;
	class Renderer;
	class Texture;
	class Buffer;
}



namespace DUOLGraphicsEngine
{
	struct RenderObject;
	class ResourceManager;

	enum class PipelineType
	{
		Render,
		PostProcessing
	};

	class DUOLGRAPHICSENGINE_EXPORT RenderingPipeline
	{
	public:
		RenderingPipeline(
			DUOLGraphicsEngine::ResourceManager* resourceManager
			, const PipelineType& pipelineType
			, const DUOLGraphicsLibrary::RenderPass& renderPass
			, const DUOLGraphicsLibrary::ResourceViewLayout& textureResourceViewLayout
			, const DUOLGraphicsLibrary::ResourceViewLayout& samplerResourceViewLayout);

	public:
		void SetName(const DUOLCommon::tstring& name);

		const DUOLCommon::tstring& GetName();

		DUOLGraphicsEngine::PipelineType GetPipelineType() const;

		void SetPipelineType(DUOLGraphicsEngine::PipelineType pipelineType);

		DUOLGraphicsLibrary::RenderPass* GetRenderPass();

		void SetRenderPass(DUOLGraphicsLibrary::RenderPass renderPass);

		DUOLGraphicsLibrary::ResourceViewLayout& GetSamplerResourceViewLayout();

		void SetSamplerResourceViewLayout(const DUOLGraphicsLibrary::ResourceViewLayout& samplerResourceViewLayout);

		DUOLGraphicsLibrary::ResourceViewLayout& GetTextureResourceViewLayout();

		void SetTextureResourceViewLayout(const DUOLGraphicsLibrary::ResourceViewLayout& textureResourceViewLayout);

		DUOLGraphicsLibrary::PipelineState* GetPipelineState() const;

		void SetPipelineState(DUOLGraphicsLibrary::PipelineState* pipelineState);

		void SetTexture(DUOLGraphicsLibrary::Texture* texture, int slot);
		
	private:
		//파이프라인 타입. Render Or PostProcess로 나눈다.
		DUOLGraphicsEngine::PipelineType _pipelineType;

		//렌더링 파이프라인 렌더타겟
		DUOLGraphicsLibrary::RenderPass _renderPass;
		
		//Constant Buffer는 Large한것 두개를 돌려쓴다. (perobj와 perframe)
		DUOLGraphicsLibrary::ResourceViewLayout _samplerResourceViewLayout;

		//todo::: 포스트프로세싱은 위쪽의 일부만 필요함. 다른식으로 빼던가 해보자.
		//이 아래의 두개는 흠... 머터리얼 정보로 빼고싶다. 머터리얼을 상속한 postprocess용 머터리얼로 만들고싶음
		//렌더링 파이프라인 Resources slot for 포스트프로세싱 텍스쳐 바인드
		//렌더링 파이프라인 쉐이더 코드 for 포스트프로세싱
		DUOLGraphicsLibrary::ResourceViewLayout _textureResourceViewLayout;

		DUOLGraphicsLibrary::PipelineState* _pipelineState;

		DUOLCommon::tstring _pipelineName;

	};
}
