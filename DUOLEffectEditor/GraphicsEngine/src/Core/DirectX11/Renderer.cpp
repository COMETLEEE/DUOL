#include "Core/DirectX11/Renderer.h"

#include <d3d11_1.h>
#include <memory>

#include "../Common/Imgui/imgui.h"
#include "../Common/Imgui/imgui_impl_win32.h"
#include "../Common/Imgui/imgui_impl_dx11.h"
#include "../Common/Imgui/imgui_internal.h"
#include "Core/DirectX11/OrderIndependentTransparency.h"

#include "Core/Pass/Particle/BasicParticlePass.h"

#include "Core/Resource/ResourceManager.h"
#include "Core/DirectX11/RenderTarget.h"

namespace MuscleGrapics
{
	std::shared_ptr<PerFrameData> Renderer::_perframeData = nullptr;

	ID3DUserDefinedAnnotation* Renderer::_debugEvent = nullptr;

	Renderer::Renderer()
	{
		DXEngine::GetInstance()->Getd3dImmediateContext()->QueryInterface(__uuidof(_debugEvent), reinterpret_cast<void**>(&_debugEvent));
	}

	Renderer::~Renderer()
	{
		while (!_renderQueueParticle.empty())
			_renderQueueParticle.pop();

		while (!_renderQueue3D.empty())
			_renderQueue3D.pop();

		while (!_renderQueue3D.empty())
			_renderQueue3D.pop();

		while (!_renderQueue3D_AlphaSort.empty())
			_renderQueue3D_AlphaSort.pop();

		while (!_renderQueue3D_OIT.empty())
			_renderQueue3D_OIT.pop();

		while (!_renderQueueText.empty())
			_renderQueueText.pop();

		while (!_renderQueueImgui.empty())
			_renderQueueImgui.pop();
		_perframeData.reset();
		_debugEvent->Release();
	}

	void Renderer::MoveRenderingData_Particle(std::queue<std::shared_ptr<RenderingData_Particle>>&& renderQueueParticle)
	{
		while (!renderQueueParticle.empty())
		{
			switch (renderQueueParticle.front()->_renderer._blendState)
			{
			case Particle_Renderer::BlendState::OIT_Default:
				_renderQueueParticleOIT.push(renderQueueParticle.front());
				break;
			case Particle_Renderer::BlendState::OIT_Additive:
				_renderQueueParticleOIT.push(renderQueueParticle.front());
				break;
			case Particle_Renderer::BlendState::Foward:
				_renderQueueParticle.push(renderQueueParticle.front());
				break;
			default:
				break;
			}
			renderQueueParticle.pop();

		}
	}

	void Renderer::MoveRenderingData_3D(std::queue<std::shared_ptr<RenderingData_3D>>&& renderQueue3D)
	{
		while (!renderQueue3D.empty())
		{
			switch (renderQueue3D.front()->_shaderInfo._blendState)
			{
			case ShaderInfo::BLENDDATA_TYPE::None:
				_renderQueue3D.push(renderQueue3D.front());
				break;
			case ShaderInfo::BLENDDATA_TYPE::OIT:
				_renderQueue3D_OIT.push(renderQueue3D.front());
				break;
			case ShaderInfo::BLENDDATA_TYPE::AlphaSort:
				_renderQueue3D_AlphaSort.push(renderQueue3D.front());
				break;
			default:
				break;
			}
			renderQueue3D.pop();
		}
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

	void Renderer::Render()
	{

		Renderer::BeginEvent(TEXT("3DObjectRender"));
		ExecuteRender();// 렌더러가 패스를 돌며 렌더링을 실시한다.
		Renderer::EndEvent();

		Renderer::BeginEvent(TEXT("DeferredRendering"));
		DXEngine::GetInstance()->GetRenderTarget()->ExecuteRender(); // 렌더 타겟에 디퍼드 조립을 한다. 
		Renderer::EndEvent();

		Renderer::BeginEvent(TEXT("ForwardRender"));
		ExecuteForwardRender();
		Renderer::EndEvent();

		Renderer::BeginEvent(TEXT("OITRender"));
		ExecuteOITRender();// Ui, Particle 등 반투명 오브젝트를 포워드 렌더링으로 실행한다.
		Renderer::EndEvent();

		Renderer::BeginEvent(TEXT("ImGuiRender"));
		ExecuteImGuiRender();
		Renderer::EndEvent();
	}

	void Renderer::ExecuteRender()
	{
		while (!_renderQueue3D.empty())
		{
			auto& object = _renderQueue3D.front();

			for (auto& iter : object->_shaderInfo._shaderName)
			{
				const auto shader = DXEngine::GetInstance()->GetResourceManager()->GetResource<Pass_3D>(iter);
				shader->Draw(*object);
			}

			_renderQueue3D.pop();
		}
	}

	void Renderer::ExecuteForwardRender()
	{
		while (!_renderQueue3D_AlphaSort.empty())
		{
			auto& object = _renderQueue3D_AlphaSort.front();

			for (auto& iter : object->_shaderInfo._shaderName)
			{
				const auto shader = DXEngine::GetInstance()->GetResourceManager()->GetResource<Pass_3D>(iter);
				shader->Draw(*object);
			}

			_renderQueue3D_AlphaSort.pop();
		}
		while (!_renderQueueParticle.empty())
		{
			auto& object = _renderQueueParticle.front();

			for (auto& iter : object->shaderName)
			{
				const auto shader = DXEngine::GetInstance()->GetResourceManager()->GetResource<Pass_Particle>(iter);
				shader->Draw(*object);
			}

			_renderQueueParticle.pop();
		}
	}

	void Renderer::ExecuteOITRender()
	{
		OrderIndependentTransparency::Get().RegistRenderingData(_renderQueueParticleOIT);

		OrderIndependentTransparency::Get().RegistRenderingData(_renderQueue3D_OIT);

		OrderIndependentTransparency::Get().CreateLayer();

		OrderIndependentTransparency::Get().MergeLayer();

		OrderIndependentTransparency::Get().PostProcessing();

	}

	void Renderer::ExecuteImGuiRender()
	{
		auto renderTarget = DXEngine::GetInstance()->GetRenderTarget();

		renderTarget->SetRenderTargetView(nullptr, 1, renderTarget->GetRenderTargetView());

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

	void Renderer::BeginEvent(const wchar_t* message)
	{
#ifdef DEBUG
		_debugEvent->BeginEvent(message);
#endif
	}

	void Renderer::EndEvent()
	{
#ifdef DEBUG
		_debugEvent->EndEvent();
#endif
	}
}
