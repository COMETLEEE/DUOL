#include "GraphicsManager.h"
#include "MuscleEngine.h"
#include "CTime.h"
#include "Camera.h"


namespace Muscle
{
	void GraphicsManager::Initialize(HWND hWnd, uint32 screenWidth, uint32 screenHeight)
	{
		// 앤진 내부에 캐싱하고 사용하자.
		_graphicsEngine = MuscleGrapics::CreateDXEngine();

		_graphicsEngine->Initialize(hWnd, screenWidth, screenHeight);

		// 개체는 만들어놓고 이것으로 재활용하면서 던지자 ..
		_perframeData = std::make_shared<MuscleGrapics::PerFrameData>();
	}

	void GraphicsManager::Render()
	{
		// ------------ Multi Thread 가능 영역
		// 1. Per-Frame Data (화면 갱신에 있어서 모든 작업에서 참조해야할 사항들)
		DispatchPerFrameData();

		// 2. Render Queue (3D)에 있는 데이터들을 날려 (Frustum Culling 할 수 있다.)
		DispatchRenderingData_3D();

		// 3. 2D UI Data들을 보낸다.
		DispatchRenderingData_UI();

		// 4. Particle Data들을 보낸다.
		DispatchRenderingData_Particle();

		// 5. Text Data에 있는 데이터들을 날려
		DispatchTextData();

		// 6. ImGui
		DispatchRenderingData_ImGui();
		// ------------ Multi Thread 가능 영역
	}

	void GraphicsManager::ExecuteRender()
	{
		_graphicsEngine->ExecuteRender();
	}

	void GraphicsManager::OnResize()
	{
		RECT _rect;
		GetClientRect(MuscleEngine::GetInstance()->GetHWND(), &_rect);
		_graphicsEngine->OnResize();
	}

	GraphicsManager::~GraphicsManager()
	{
		Release();

	}
	template<class T>
	void ClearQueue(T& targetQueue)
	{
		T empty;
		std::swap(targetQueue, empty);
	}
	void GraphicsManager::DispatchRenderingData_Particle()
	{
		if (!_renderQueueParticle.empty())
			_graphicsEngine->PostRenderingData_Particle(std::move(_renderQueueParticle));
		ClearQueue<std::queue<std::shared_ptr<MuscleGrapics::RenderingData_Particle>>>(_renderQueueParticle);
	}

	void GraphicsManager::DispatchRenderingData_UI()
	{
		if (!_renderQueueUI.empty())
			_graphicsEngine->PostRenderingData_UI(std::move(_renderQueueUI));
		ClearQueue<std::queue<std::shared_ptr<MuscleGrapics::RenderingData_UI>>>(_renderQueueUI);
	}

	void GraphicsManager::DispatchRenderingData_3D()
	{
		if (!_renderQueue.empty())
			_graphicsEngine->PostRenderingData_3D(std::move(_renderQueue));
		ClearQueue<std::queue<std::shared_ptr<MuscleGrapics::RenderingData_3D>>>(_renderQueue);
	}

	void GraphicsManager::DispatchRenderingData_ImGui()
	{
		if (!_renderQueueImGui.empty())
			_graphicsEngine->PostRenderingData_ImGui(std::move(_renderQueueImGui));
		ClearQueue <std::queue<std::function<void()>>>(_renderQueueImGui);
	}

	void GraphicsManager::DispatchPerFrameData()
	{

		if (MuscleEngine::GetInstance()->GetMainCamera() != nullptr)
		{
			memcpy(&_perframeData->_cameraInfo, MuscleEngine::GetInstance()->GetMainCamera()->_cameraInfo.get(), sizeof(MuscleGrapics::CameraInfo));
		}

		while (!_dirLightInfoQueue.empty())
		{
			MuscleGrapics::DirectionalLightInfo& dirInfo = _dirLightInfoQueue.front(); _dirLightInfoQueue.pop();

			_perframeData->_directionalLightInfos.emplace_back(dirInfo);
		}

		while (!_pointLightInfoQueue.empty())
		{
			MuscleGrapics::PointLightInfo& pointLightInfo = _pointLightInfoQueue.front(); _pointLightInfoQueue.pop();

			_perframeData->_pointLightInfos.emplace_back(pointLightInfo);
		}
		while (!_spotLightInfoQueue.empty())
		{
			MuscleGrapics::SpotLightInfo& spotLightInfo = _spotLightInfoQueue.front(); _spotLightInfoQueue.pop();

			_perframeData->_spotLightInfos.emplace_back(spotLightInfo);
		}

		// 델타 타임 보내주는 기능을 추가한다. (그래픽스 엔진 내부에서의 물리 시뮬레이션에 필요하다.)
		// 게임 델타 타임을 던져주는구먼 !!
		_perframeData->_deltaTime = CTime::GetGameDeltaTime();			// 모든 것들이 이걸로 업데이트된다.

		_perframeData->_gamePlayTime = CTime::GetGamePlayTime();

		_graphicsEngine->PostPerFrameData(std::move(_perframeData));

		//MuscleEngine::GetInstance()->GetDebugManager()->PostPerFrameData(_perframeData);
	}

	void GraphicsManager::DispatchTextData()
	{
		if (!_textDataQueue.empty())
			_graphicsEngine->PostTextData(std::move(_textDataQueue));
		ClearQueue<std::queue<std::shared_ptr<MuscleGrapics::TextData>>>(_textDataQueue);
	}

	void GraphicsManager::Release()
	{
		DeleteDXEngine(_graphicsEngine);
		_graphicsEngine = nullptr;
	}

	void GraphicsManager::TextureRelease()
	{
		_graphicsEngine->ReleaseTexture();
	}

	void GraphicsManager::PostRenderingData_UI(std::shared_ptr<MuscleGrapics::RenderingData_UI>& renderingData)
	{
		_renderQueueUI.emplace(renderingData);
	}

	void GraphicsManager::PostRenderingData_3D(std::shared_ptr<MuscleGrapics::RenderingData_3D>& renderingData)
	{
		_renderQueue.emplace(renderingData);
	}

	void GraphicsManager::PostRenderingData_Particle(std::shared_ptr<MuscleGrapics::RenderingData_Particle>& renderingData)
	{
		_renderQueueParticle.emplace(renderingData);
	}

	void GraphicsManager::PostRenderingData_Imgui(std::function<void()>& renderingData)
	{
		_renderQueueImGui.emplace(renderingData);
	}

	void GraphicsManager::PostDirectionalLightInfo(MuscleGrapics::DirectionalLightInfo& dirLightInfo)
	{
		_dirLightInfoQueue.emplace(dirLightInfo);
	}

	void GraphicsManager::PostPointLightInfo(MuscleGrapics::PointLightInfo& pointLightInfo)
	{
		_pointLightInfoQueue.emplace(pointLightInfo);
	}

	void GraphicsManager::PostSpotLightInfo(MuscleGrapics::SpotLightInfo& spotLightInfo)
	{
		_spotLightInfoQueue.emplace(spotLightInfo);
	}

	void GraphicsManager::PostTextData(std::shared_ptr<MuscleGrapics::TextData>& textData)
	{
		_textDataQueue.emplace(textData);
	}

	void* GraphicsManager::InsertTexture(tstring path)
	{
		return _graphicsEngine->InsertTexture(path);

	}

	void* GraphicsManager::GetTexture(tstring textureMap)
	{
		return _graphicsEngine->GetTexture(textureMap);
	}

	void* GraphicsManager::GetNoiseMap(std::tuple<float, int, float> key)
	{
		return _graphicsEngine->GetNoiseMap(key);
	}

	unsigned int GraphicsManager::PickObjectID(int x, int y)
	{
		return _graphicsEngine->PickObjectID(x, y);
	}
	ImGuiContext* GraphicsManager::GetImguiContext()
	{
		return MuscleGrapics::GetImguiContext();
	}
}
