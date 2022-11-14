#include "pch.h"
#include "GraphicsManager.h"


namespace Muscle
{
	void GraphicsManager::Initialize(HWND hWnd, uint32 screenWidth, uint32 screenHeight)
	{
		// ���� ���ο� ĳ���ϰ� �������.
		_graphicsEngine = CreateDXEngine();

		_graphicsEngine->Initialize(hWnd, screenWidth, screenHeight);

		// ��ü�� �������� �̰����� ��Ȱ���ϸ鼭 ������ ..
		_perframeData = std::make_shared<PerFrameData>();
	}

	void GraphicsManager::Render()
	{
		// ------------ Multi Thread ���� ����
		// 1. Per-Frame Data (ȭ�� ���ſ� �־ ��� �۾����� �����ؾ��� ���׵�)
		DispatchPerFrameData();

		// 2. Render Queue (3D)�� �ִ� �����͵��� ���� (Frustum Culling �� �� �ִ�.)
		DispatchRenderingData_3D();

		// 3. 2D UI Data���� ������.
		DispatchRenderingData_UI();

		// 4. Particle Data���� ������.
		DispatchRenderingData_Particle();

		// 5. Text Data�� �ִ� �����͵��� ����
		DispatchTextData();

		// 6. ImGui
		DispatchRenderingData_ImGui();
		// ------------ Multi Thread ���� ����
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
		ClearQueue<std::queue<std::shared_ptr<RenderingData_Particle>>>(_renderQueueParticle);
	}

	void GraphicsManager::DispatchRenderingData_UI()
	{
		if (!_renderQueueUI.empty())
			_graphicsEngine->PostRenderingData_UI(std::move(_renderQueueUI));
		ClearQueue<std::queue<std::shared_ptr<RenderingData_UI>>>(_renderQueueUI);
	}

	void GraphicsManager::DispatchRenderingData_3D()
	{
		if (!_renderQueue.empty())
			_graphicsEngine->PostRenderingData_3D(std::move(_renderQueue));
		ClearQueue<std::queue<std::shared_ptr<RenderingData_3D>>>(_renderQueue);
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
			memcpy(_perframeData->_cameraInfo.get(), MuscleEngine::GetInstance()->GetMainCamera()->_cameraInfo.get(), sizeof(CameraInfo));
		}

		while (!_dirLightInfoQueue.empty())
		{
			std::shared_ptr<DirectionalLightInfo> dirInfo = _dirLightInfoQueue.front(); _dirLightInfoQueue.pop();

			_perframeData->_directionalLightInfos.emplace_back(dirInfo);
		}

		while (!_pointLightInfoQueue.empty())
		{
			std::shared_ptr<PointLightInfo> pointLightInfo = _pointLightInfoQueue.front(); _pointLightInfoQueue.pop();

			_perframeData->_pointLightInfos.emplace_back(pointLightInfo);
		}

		while (!_spotLightInfoQueue.empty())
		{
			std::shared_ptr<SpotLightInfo> spotLightInfo = _spotLightInfoQueue.front(); _spotLightInfoQueue.pop();

			_perframeData->_spotLightInfos.emplace_back(spotLightInfo);
		}

		// ��Ÿ Ÿ�� �����ִ� ����� �߰��Ѵ�. (�׷��Ƚ� ���� ���ο����� ���� �ùķ��̼ǿ� �ʿ��ϴ�.)
		// ���� ��Ÿ Ÿ���� �����ִ±��� !!
		_perframeData->_deltaTime = CTime::GetGameDeltaTime();			// ��� �͵��� �̰ɷ� ������Ʈ�ȴ�.

		_graphicsEngine->PostPerFrameData(std::move(_perframeData));

		//MuscleEngine::GetInstance()->GetDebugManager()->PostPerFrameData(_perframeData);
	}

	void GraphicsManager::DispatchTextData()
	{
		if (!_textDataQueue.empty())
			_graphicsEngine->PostTextData(std::move(_textDataQueue));
		ClearQueue<std::queue<std::shared_ptr<TextData>>>(_textDataQueue);
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

	void GraphicsManager::PostRenderingData_UI(std::shared_ptr<RenderingData_UI>& renderingData)
	{
		_renderQueueUI.emplace(renderingData);
	}

	void GraphicsManager::PostRenderingData_3D(std::shared_ptr<RenderingData_3D>& renderingData)
	{
		_renderQueue.emplace(renderingData);
	}

	void GraphicsManager::PostRenderingData_Particle(std::shared_ptr<RenderingData_Particle>& renderingData)
	{
		_renderQueueParticle.emplace(renderingData);
	}

	void GraphicsManager::PostRenderingData_Imgui(std::function<void()>& renderingData)
	{
		_renderQueueImGui.emplace(renderingData);
	}

	void GraphicsManager::PostDirectionalLightInfo(std::shared_ptr<DirectionalLightInfo>& dirLightInfo)
	{
		_dirLightInfoQueue.emplace(dirLightInfo);
	}

	void GraphicsManager::PostPointLightInfo(std::shared_ptr<PointLightInfo>& pointLightInfo)
	{
		_pointLightInfoQueue.emplace(pointLightInfo);
	}

	void GraphicsManager::PostSpotLightInfo(std::shared_ptr<SpotLightInfo>& spotLightInfo)
	{
		_spotLightInfoQueue.emplace(spotLightInfo);
	}

	void GraphicsManager::PostTextData(std::shared_ptr<TextData>& textData)
	{
		_textDataQueue.emplace(textData);
	}

	void* GraphicsManager::InsertTexture(tstring name, tstring path)
	{
		return _graphicsEngine->InsertTexture(name, path);

	}

	void* GraphicsManager::GetTexture(tstring textureMap)
	{
		return _graphicsEngine->GetTexture(textureMap);
	}
}
