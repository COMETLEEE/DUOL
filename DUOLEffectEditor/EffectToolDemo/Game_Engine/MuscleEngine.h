#pragma once
#include "IGameEngine.h"


namespace Muscle
{
	class SceneManager;
	class GraphicsManager;
	class CTime;
	class KeyBoard;
	class ObjectManager;
	class IMesh;
	class Camera;
	class ResourceManager;
	class DebugManager;
	class XPad;

	class MuscleEngine : public IGameEngine// �ʴ� �������� �� �������̽��� �ٲ��ٰ�..
	{
	public:
		MuscleEngine() = default;

		~MuscleEngine();

	private:
		std::shared_ptr<GraphicsManager> m_GraphicsManager; //�׷��Ƚ� ����.. ���� �и��� ���� �������̽���.

		std::shared_ptr<ResourceManager> m_ResourceManager;

		std::shared_ptr<SceneManager> m_SceneManager;

		std::shared_ptr<DebugManager> m_DebugManager; // ����� ���� ���� �Ŵ���!!

		std::shared_ptr<CTime> m_Time;

		std::shared_ptr<KeyBoard> m_keyBoard;

		std::shared_ptr<XPad> m_XPad;

		std::shared_ptr<ObjectManager> m_ObjManager;

		static std::shared_ptr<MuscleEngine> m_Instance;

		HWND m_hWnd;

		std::shared_ptr<Camera> m_MainCamera;

	public:
		virtual void Initialize(HWND hWnd, int Width, int height) override;

		virtual void Update() override;

		virtual void Render() override;

		virtual void OnResize() override;

		virtual void SetMainCamera(std::shared_ptr<Camera> _MainCamera) override;

		virtual void InsertObject(std::shared_ptr<GameObject> _GameObject) override;

		virtual std::shared_ptr<ResourceManager> GetResourceManager() override;

		virtual std::shared_ptr<GraphicsManager> GetGraphicsManager() override;

		virtual std::shared_ptr<SceneManager> GetSceneManager() override;

		std::shared_ptr<DebugManager> GetDebugManager();

		virtual std::shared_ptr<Camera> GetMainCamera() override;

		void SetObjManager(std::shared_ptr<ObjectManager> _ObjManager);

		virtual std::shared_ptr<ObjectManager> GetObjManager() override;

		HWND GetHWND();

		static std::shared_ptr<MuscleEngine> GetInstance();
		
	};

}
