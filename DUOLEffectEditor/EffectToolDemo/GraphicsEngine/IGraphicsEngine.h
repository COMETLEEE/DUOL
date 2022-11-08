#pragma once
#include <windows.h>
#include <string>
#include "RenderingData.h"

#ifdef GRAPHICSENGINE_EXPORTS
#define GRPHICHS_API __declspec(dllexport)
#else
#define GRPHICHS_API __declspec(dllimport)
#endif


struct ImGuiContext;
class DXEngine;

class IGraphicsEngine
{
public:
	IGraphicsEngine() = default;
	virtual ~IGraphicsEngine() = default;
	//
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




};
// �׷��Ƚ� ���� ����
extern "C" GRPHICHS_API IGraphicsEngine * CreateDXEngine();
extern "C" GRPHICHS_API void DeleteDXEngine(IGraphicsEngine * _pEngine);
extern "C" GRPHICHS_API ImGuiContext * GetImguiContext();
