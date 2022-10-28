#pragma once
#include <vector>

#include "DUOLGraphicsLibrary/PipelineStateFlags.h"
#include "DUOLGraphicsLibrary/Renderer/ResourceViewLayout.h"

namespace DUOLGraphicsLibrary
{
	class RenderTarget;
	class Shader;
	class PipelineState;
	class Renderer;
	class Texture;
	class RenderPass;
}

namespace DUOLGraphicsEngine
{
	class RenderPipieline
	{
	public:
		RenderPipieline(DUOLGraphicsLibrary::Renderer* renderer,const DUOLGraphicsLibrary::PipelineStateDesc& pipelineDesv);

	private:
		//std::vector<DUOLGraphicsLibrary::Texture*> _renderTargetTextures;

		//std::vector<DUOLGraphicsLibrary::RenderTarget*> _renderTarget;

		//������ ���������� ����Ÿ��
		std::vector<DUOLGraphicsLibrary::RenderPass*> _renderPass;

		//������ ���������� PSO
		DUOLGraphicsLibrary::PipelineState* _pipielineState;

		DUOLGraphicsLibrary::PipelineStateDesc _pipelineDesc;

		//DUOLGraphicsLibrary::Shader* _vertexShader;

		//DUOLGraphicsLibrary::Shader* _pixelShader;

		//DUOLGraphicsLibrary::Shader* _hullShader;

		//DUOLGraphicsLibrary::Shader* _domainShader;

		//DUOLGraphicsLibrary::Shader* _geometryShader;

		//������ ���������� Resources slot
		DUOLGraphicsLibrary::ResourceViewLayout _resourceViewLayout;

	public:
		void ChangeResourceView();

	};
}
