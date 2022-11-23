#pragma once

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

		std::queue<std::shared_ptr<RenderingData_Particle>> _renderQueueParticle;

		std::queue<std::shared_ptr<RenderingData_3D>> _renderQueue;

		std::queue<std::shared_ptr<RenderingData_UI>> _renderQueueUI;

		std::queue<std::function<void()>> _renderQueueImGui;

		std::queue<std::shared_ptr<TextData>> _textDataQueue;

		std::queue<std::shared_ptr<DirectionalLightInfo>> _dirLightInfoQueue;

		std::queue<std::shared_ptr<PointLightInfo>> _pointLightInfoQueue;

		std::queue<std::shared_ptr<SpotLightInfo>> _spotLightInfoQueue;

		std::shared_ptr<PerFrameData> _perframeData;

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

		void PostRenderingData_UI(std::shared_ptr<RenderingData_UI>& renderingData);

		void PostRenderingData_3D(std::shared_ptr<RenderingData_3D>& renderingData);

		void PostRenderingData_Particle(std::shared_ptr<RenderingData_Particle>& renderingData);

		void PostRenderingData_Imgui(std::function<void()>& renderingData);

		void PostDirectionalLightInfo(std::shared_ptr<DirectionalLightInfo>& dirLightInfo);

		void PostPointLightInfo(std::shared_ptr<PointLightInfo>& pointLightInfo);

		void PostSpotLightInfo(std::shared_ptr<SpotLightInfo>& spotLightInfo);

		void PostTextData(std::shared_ptr<TextData>& textData);

		void* InsertTexture(tstring path);

		void* GetTexture(tstring textureMap);

	};
}