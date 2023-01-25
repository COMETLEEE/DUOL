#pragma once
#include <memory>
#include <queue>
#include <functional>
#include <Windows.h>
#include "util\TypeDefine.h"
#include "Export\RenderingData.h"
#include "Export\IGraphicsEngine.h"

class ImGuiContext;

namespace Muscle
{
	/// <summary>
	/// 그래픽스 엔진을 1대1 마크하는 녀석 
	/// 실질적인 게임 플레이를 그리는 녀석
	/// 안만들고 싶었는데 PerFrameData를 관리해 줄 친구가 없다..!
	/// </summary>
	class GraphicsManager final
	{
	public:
		GraphicsManager() = default;

		~GraphicsManager();

		GraphicsManager(const GraphicsManager& other) = delete;

		GraphicsManager& operator= (const GraphicsManager& other) = delete;

		GraphicsManager(GraphicsManager&& other) = delete;

	private:
		MuscleGrapics::IGraphicsEngine* _graphicsEngine;

		std::queue<std::shared_ptr<MuscleGrapics::RenderingData_Particle>> _renderQueueParticle;

		std::queue<std::shared_ptr<MuscleGrapics::RenderingData_3D>> _renderQueue;

		std::queue<std::shared_ptr<MuscleGrapics::RenderingData_UI>> _renderQueueUI;

		std::queue<std::function<void()>> _renderQueueImGui;

		std::queue<std::shared_ptr<MuscleGrapics::TextData>> _textDataQueue;

		std::queue<MuscleGrapics::DirectionalLightInfo> _dirLightInfoQueue;

		std::queue<MuscleGrapics::PointLightInfo> _pointLightInfoQueue;

		std::queue<MuscleGrapics::SpotLightInfo> _spotLightInfoQueue;

		std::shared_ptr<MuscleGrapics::PerFrameData> _perframeData;

	private:
		void DispatchRenderingData_Particle();

		void DispatchRenderingData_UI();

		void DispatchRenderingData_3D();

		void DispatchRenderingData_ImGui();

		void DispatchPerFrameData();

		void DispatchTextData();

	public:
		void Initialize(HWND hWnd, uint32 screenWidth, uint32 screenHeight);

		void Render();

		void ExecuteRender();

		void OnResize();

		void Release();

		void TextureRelease();

		void PostRenderingData_UI(std::shared_ptr<MuscleGrapics::RenderingData_UI>& renderingData);

		void PostRenderingData_3D(std::shared_ptr<MuscleGrapics::RenderingData_3D>& renderingData);

		void PostRenderingData_Particle(std::shared_ptr<MuscleGrapics::RenderingData_Particle>& renderingData);

		void PostRenderingData_Imgui(std::function<void()>& renderingData);

		void PostDirectionalLightInfo(MuscleGrapics::DirectionalLightInfo& dirLightInfo);

		void PostPointLightInfo(MuscleGrapics::PointLightInfo& pointLightInfo);

		void PostSpotLightInfo(MuscleGrapics::SpotLightInfo& spotLightInfo);

		void PostTextData(std::shared_ptr<MuscleGrapics::TextData>& textData);

		void* InsertTexture(tstring path);

		void* GetTexture(tstring textureMap);

		void* GetNoiseMap(std::tuple<float, int, float> key);

		unsigned int PickObjectID(int x, int y);

		ImGuiContext* GetImguiContext();
	};
}