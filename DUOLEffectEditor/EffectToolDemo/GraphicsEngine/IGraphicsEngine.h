#pragma once
#include <windows.h>
#include <string>
#include "RenderingData.h"

#ifdef GRAPHICSENGINE_EXPORTS
#define GRPHICHS_API __declspec(dllexport)
#else
#define GRPHICHS_API __declspec(dllimport)
#endif


class IMesh;
class DXEngine;

class IGraphicsEngine
{
public:
	IGraphicsEngine() = default;
	virtual ~IGraphicsEngine() = default;
	//
public:
	GRPHICHS_API virtual void Initialize(HWND hWnd, int Width, int height) abstract;
	GRPHICHS_API virtual void BeginRender() abstract;
	GRPHICHS_API virtual void EndRender() abstract;
	GRPHICHS_API virtual void OnResize() abstract;
	GRPHICHS_API virtual void CameraUpdate(const XMMATRIX&& _View, const XMMATRIX&& _Proj, const XMFLOAT3&& _Pos) abstract;
	GRPHICHS_API virtual void ShadowUpdate(const XMMATRIX&& _View, const XMMATRIX&& _Proj) abstract;
	GRPHICHS_API virtual IMesh* GetMeshResource(std::string _Name) abstract;
	GRPHICHS_API virtual float GetWidth() abstract;
	GRPHICHS_API virtual float GetHeight() abstract;

	GRPHICHS_API virtual void ExecuteRender() abstract;
	GRPHICHS_API virtual void PostRenderingData_Particle(std::queue<std::shared_ptr<RenderingData_Particle>>&& renderQueueParticle) abstract;
	GRPHICHS_API virtual void PostRenderingData_3D(std::queue<std::shared_ptr<RenderingData_3D>>&& renderQueueParticle) abstract;
	GRPHICHS_API virtual void PostRenderingData_UI(std::queue<std::shared_ptr<RenderingData_UI>>&& renderQueueParticle) abstract;
	GRPHICHS_API virtual void PostTextData(std::queue<std::shared_ptr<TextData>>&& renderQueueParticle) abstract;
	GRPHICHS_API virtual void PostPerFrameData(std::shared_ptr<PerFrameData>&& perframeData) abstract;

	GRPHICHS_API virtual void ReleaseTexture() abstract;

	


};
// 그래픽스 엔진 생성
extern "C" GRPHICHS_API IGraphicsEngine * CreateDXEngine();
extern "C" GRPHICHS_API void DeleteDXEngine(IGraphicsEngine * _pEngine);
extern "C" GRPHICHS_API LRESULT ImGui_WndProcHandler(HWND hwnd, UINT unit, WPARAM wparam, LPARAM lparam); // imgui를 위한
