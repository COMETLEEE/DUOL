#pragma once


#include "RenderingData.h"


struct ImGuiContext;

namespace MuscleGrapics
{
	class DXEngine;

	class IGraphicsEngine
	{
	public:
		IGraphicsEngine() = default;

		virtual ~IGraphicsEngine() = default;

	public:
		GRPHICHS_API virtual void Initialize(HWND hWnd, int Width, int height) abstract;

		GRPHICHS_API virtual void OnResize() abstract;

		GRPHICHS_API virtual void ExecuteRender() abstract;

		GRPHICHS_API virtual void PostRenderingData_Particle(std::queue<std::shared_ptr<RenderingData_Particle>>&& renderQueueParticle) abstract;

		GRPHICHS_API virtual void PostRenderingData_3D(std::queue<std::shared_ptr<RenderingData_3D>>&& renderQueueParticle) abstract;

		GRPHICHS_API virtual void PostRenderingData_UI(std::queue<std::shared_ptr<RenderingData_UI>>&& renderQueueParticle) abstract;

		GRPHICHS_API virtual void PostRenderingData_ImGui(std::queue<std::function<void()>>&& renderQueueImGui) abstract;

		GRPHICHS_API virtual void PostTextData(std::queue<std::shared_ptr<TextData>>&& renderQueueParticle) abstract;

		GRPHICHS_API virtual void PostPerFrameData(std::shared_ptr<PerFrameData>&& perframeData) abstract;

		GRPHICHS_API virtual void ReleaseTexture() abstract;

		GRPHICHS_API virtual void* InsertTexture(tstring path) abstract;

		GRPHICHS_API virtual void* GetTexture(tstring textureMap) abstract;

		GRPHICHS_API virtual void* GetNoiseMap(std::tuple<float, int, float> key) abstract;

		GRPHICHS_API virtual unsigned int PickObjectID(int x, int y) abstract;
	};
	// 그래픽스 엔진 생성
	extern "C" GRPHICHS_API IGraphicsEngine * CreateDXEngine();

	extern "C" GRPHICHS_API void DeleteDXEngine(IGraphicsEngine * _pEngine);

	extern "C" GRPHICHS_API ImGuiContext * GetImguiContext();
}
