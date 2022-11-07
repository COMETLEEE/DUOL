#include "pch.h"
#include "Renderer.h"

std::shared_ptr<PerFrameData> Renderer::_perframeData = nullptr;

Renderer::~Renderer()
{
	while (!_renderQueueParticle.empty())
		_renderQueueParticle.pop();

	while (!_renderQueue3D.empty())
		_renderQueue3D.pop();

	while (!_renderQueueUI.empty())
		_renderQueueUI.pop();

	while (!_renderQueueText.empty())
		_renderQueueText.pop();

	_perframeData.reset();
}

void Renderer::MoveRenderingData_Particle(std::queue<std::shared_ptr<RenderingData_Particle>>&& renderQueueParticle)
{
	_renderQueueParticle = renderQueueParticle;
}

void Renderer::MoveRenderingData_3D(std::queue<std::shared_ptr<RenderingData_3D>>&& renderQueue3D)
{
	_renderQueue3D = renderQueue3D;
}

void Renderer::MoveRenderingData_UI(std::queue<std::shared_ptr<RenderingData_UI>>&& renderQueueUI)
{
	_renderQueueUI = renderQueueUI;
}

void Renderer::MoveTextData(std::queue<std::shared_ptr<TextData>>&& renderQueueText)
{
	_renderQueueText = renderQueueText;
}

void Renderer::MovePerFrameData(std::shared_ptr<PerFrameData>&& perframeData)
{
	_perframeData = perframeData;
}

void Renderer::ExecuteRender()
{
	while (!_renderQueue3D.empty())
	{
		auto& object = _renderQueue3D.front();

		const auto shader = DXEngine::GetInstance()->GetResourceManager()->Get3DShader(object->_shaderInfo->_shaderName);

		shader->Draw(object);
		_renderQueue3D.pop();
	}


	while (!_renderQueueParticle.empty())
	{
		/// <summary>
		/// 파티클은 반투명하기 때문에 렌더링의 순서를 마지막으로 변경 하여야한다.
		///
		/// </summary>
		auto& object = _renderQueueParticle.front();

		const auto shader = DXEngine::GetInstance()->GetResourceManager()->GetParticleShader(object->shaderName);

		shader->Draw(object);
		_renderQueueParticle.pop();
	}




}

const std::shared_ptr<PerFrameData>& Renderer::GetPerfreamData()
{
	return _perframeData;
}
