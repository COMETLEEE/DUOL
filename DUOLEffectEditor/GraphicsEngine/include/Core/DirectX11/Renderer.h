#pragma once
#include <functional>
#include <memory>
#include <queue>

/// <summary>
/// ���� ť�� �޾Ƽ� �ѹ��� �������� ���� ���ִ� ��ü.
/// ������ �Ŵ������ �����ϸ� �� ��...
/// 
/// </summary>
class ID3DUserDefinedAnnotation;

namespace MuscleGrapics
{
	struct RenderingData_Particle;

	struct RenderingData_3D;

	struct RenderingData_UI;

	struct TextData;

	struct PerFrameData;

	class Renderer
	{
	public:
		Renderer();

		~Renderer();

	public:
		void MoveRenderingData_Particle(std::queue<std::shared_ptr<RenderingData_Particle>>&& renderQueueParticle);

		void MoveRenderingData_3D(std::queue<std::shared_ptr<RenderingData_3D>>&& renderQueue3D);

		void MoveRenderingData_UI(std::queue<std::shared_ptr<RenderingData_UI>>&& renderQueueUI);

		void MoveRenderingData_ImGui(std::queue<std::function<void()>>&& renderQueueImGui);

		void MoveTextData(std::queue<std::shared_ptr<TextData>>&& renderQueueText);

		void MovePerFrameData(std::shared_ptr<PerFrameData>&& perframeData);

		void Render();

		static const std::shared_ptr<PerFrameData>& GetPerfreamData();
	private:
		std::queue<std::shared_ptr<RenderingData_Particle>> _renderQueueParticle;

		std::queue<std::shared_ptr<RenderingData_Particle>> _renderQueueParticleOIT;

		std::queue<std::shared_ptr<RenderingData_3D>> _renderQueue3D;

		std::queue<std::shared_ptr<RenderingData_UI>> _renderQueueUI;

		std::queue<std::function<void()>> _renderQueueImgui;

		std::queue<std::shared_ptr<TextData>> _renderQueueText;

		static std::shared_ptr<PerFrameData> _perframeData;

		static ID3DUserDefinedAnnotation* _debugEvent;

		void ExecuteRender();

		void ExecuteForwardRender();

		void ExecuteOITRender();

		void ExecuteImGuiRender();
	public:
		static void BeginEvent(const wchar_t* message);
		static void EndEvent();

	};
}
