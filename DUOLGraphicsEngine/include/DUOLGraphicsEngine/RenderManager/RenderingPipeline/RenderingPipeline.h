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
		//���������� Ÿ��. Render Or PostProcess�� ������.
		DUOLGraphicsEngine::PipelineType _pipelineType;

		//������ ���������� ����Ÿ��
		DUOLGraphicsLibrary::RenderPass _renderPass;
		
		//Constant Buffer�� Large�Ѱ� �ΰ��� ��������. (perobj�� perframe)
		DUOLGraphicsLibrary::ResourceViewLayout _samplerResourceViewLayout;

		//todo::: ����Ʈ���μ����� ������ �Ϻθ� �ʿ���. �ٸ������� ������ �غ���.
		//�� �Ʒ��� �ΰ��� ��... ���͸��� ������ ����ʹ�. ���͸����� ����� postprocess�� ���͸���� ��������
		//������ ���������� Resources slot for ����Ʈ���μ��� �ؽ��� ���ε�
		//������ ���������� ���̴� �ڵ� for ����Ʈ���μ���
		DUOLGraphicsLibrary::ResourceViewLayout _textureResourceViewLayout;

		DUOLGraphicsLibrary::PipelineState* _pipelineState;

		DUOLCommon::tstring _pipelineName;

	};
}
