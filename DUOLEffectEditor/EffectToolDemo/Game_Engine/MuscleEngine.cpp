#include "pch.h"
#include "MuscleEngine.h"
//#include "..\GraphicsEngine\IGraphicsEngine.h"




namespace Muscle
{

	std::shared_ptr<MuscleEngine> MuscleEngine::m_Instance = nullptr;



	MuscleEngine::~MuscleEngine()
	{
		m_GraphicsManager.reset();

		//m_DebugManager.reset();

		m_SceneManager.reset();

		m_Time.reset();

		//m_XPad.reset();

		m_keyBoard->Finalize();

		m_keyBoard.reset();

		m_ObjManager.reset();

		m_MainCamera.reset();
	}

	void MuscleEngine::Initialize(HWND hWnd, int Width, int height)
	{
		m_hWnd = hWnd;

		m_GraphicsManager = std::make_shared<GraphicsManager>();
		m_GraphicsManager->Initialize(hWnd, Width, height);

		//m_ResourceManager = ResourceManager::Get();
		//m_ResourceManager->Initialize();

		m_SceneManager = std::make_shared<SceneManager>();

		//m_DebugManager = std::make_shared<DebugManager>();
		//m_DebugManager->Initialize();

		m_Time = std::make_shared<CTime>();

		//m_XPad = XPad::Get();

		m_keyBoard = KeyBoard::Get();


		ImGui::SetCurrentContext(GetImguiContext());
	}

	void MuscleEngine::Update()
	{
		m_Time->Update();

		m_keyBoard->Update();

		//m_XPad->UpdateXPadStates(CTime::GetDeltaTime());

		// �� ��ȯ�� ����Ǿ� ������ �� �ø���
		if (m_SceneManager->_isLoadSceneReserved)
		{
			m_SceneManager->FlipScene();
		}
		// �� ��ȯ�� ����Ǿ� ������ �� �ø���

		m_SceneManager->GetScene()->RapidUpdate();

		m_SceneManager->GetScene()->Update();

		m_ObjManager->Update();

		if (m_keyBoard->Get()->KeyDown(VK_F1))
			TurnOnDebug(); // ����� ������ �� ����

		if (m_keyBoard->Get()->KeyDown(VK_F2))
			TurnOffDebug(); // ����� ������ �� ����



	}

	void MuscleEngine::Render()
	{
		m_ObjManager->Render();

		m_GraphicsManager->Render();

		//m_DebugManager->Render();

		m_GraphicsManager->ExecuteRender(); // ���������� ��� ť�� �Ѱ��ش�.
	}

	void MuscleEngine::OnResize()
	{
		if (m_GraphicsManager != nullptr)
			m_GraphicsManager->OnResize();

		if (m_MainCamera != nullptr)
			m_MainCamera->SetLens();
	}

	std::shared_ptr<MuscleEngine> MuscleEngine::GetInstance()
	{
		if (m_Instance == nullptr)
			m_Instance = std::make_shared<MuscleEngine>();

		return m_Instance;
	}

	void MuscleEngine::Finalize()
	{
		m_Instance.reset();
	}

	void MuscleEngine::SetObjManager(std::shared_ptr<ObjectManager> _ObjManager)
	{
		m_ObjManager = _ObjManager;
	}

	std::shared_ptr<ObjectManager> MuscleEngine::GetObjManager()
	{
		return m_ObjManager;
	}

	HWND MuscleEngine::GetHWND()
	{
		return m_hWnd;
	}

	void MuscleEngine::SetMainCamera(std::shared_ptr<Camera> _MainCamera)
	{
		if (m_MainCamera)
			m_MainCamera->SetMainCamera(false);

		m_MainCamera = _MainCamera;
		m_MainCamera->SetMainCamera(true);
		m_MainCamera->SetLens();
	}

	void MuscleEngine::InsertObject(std::shared_ptr<GameObject> _GameObject)
	{
		GetObjManager()->InsertObject(_GameObject);
	}

	std::shared_ptr<GraphicsManager> MuscleEngine::GetGraphicsManager()
	{
		return m_GraphicsManager;
	}


	std::shared_ptr<SceneManager> MuscleEngine::GetSceneManager()
	{
		return m_SceneManager;
	}

	std::shared_ptr<Camera> MuscleEngine::GetMainCamera()
	{
		assert(m_MainCamera, "NoMainCamera");
		return m_MainCamera;
	}
}