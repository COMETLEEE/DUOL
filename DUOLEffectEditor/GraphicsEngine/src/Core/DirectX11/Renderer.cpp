#include "Core/DirectX11/Renderer.h"

#include <memory>

#include "../Common/Imgui/imgui.h"
#include "../Common/Imgui/imgui_impl_win32.h"
#include "../Common/Imgui/imgui_impl_dx11.h"
#include "../Common/Imgui/imgui_internal.h"

#include "Core/Pass/BasicParticlePass.h"

#include "Core/Resource/ResourceManager.h"
#include "Core/DirectX11/RenderTarget.h"

namespace MuscleGrapics
{
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

		while (!_renderQueueImgui.empty())
			_renderQueueImgui.pop();
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

	void Renderer::MoveRenderingData_ImGui(std::queue<std::function<void()>>&& renderQueueImGui)
	{
		_renderQueueImgui = renderQueueImGui;
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


			for (auto& iter : object->_shaderInfo->_shaderName)
			{
				const auto shader = DXEngine::GetInstance()->GetResourceManager()->Get3DShader(iter);
				shader->Draw(*object);
			}

			_renderQueue3D.pop();
		}
	}

	void Renderer::ExecuteForwardRender()
	{
		while (!_renderQueueParticle.empty())
		{
			/// <summary>
			/// 파티클은 반투명하기 때문에 렌더링의 순서를 마지막으로 변경 하여야한다.
			/// </summary>
			auto& object = _renderQueueParticle.front();


			for (auto& iter : object->shaderName)
			{
				const auto shader = DXEngine::GetInstance()->GetResourceManager()->GetParticleShader(iter);

				shader->Draw(*object);
			}

			if (object->_isDelete)
				DXEngine::GetInstance()->GetResourceManager()->DeleteParticleMesh(object->_objectID);

			_renderQueueParticle.pop();
		}

		auto renderTarget = DXEngine::GetInstance()->GetRenderTarget();

		renderTarget->SetRenderTargetView(
			nullptr,
			1,
			renderTarget->GetRenderTargetView()
		);


		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		while (!_renderQueueImgui.empty())
		{
			_renderQueueImgui.front()();
			_renderQueueImgui.pop();
		}

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	const std::shared_ptr<PerFrameData>& Renderer::GetPerfreamData()
	{
		return _perframeData;
	}
}
