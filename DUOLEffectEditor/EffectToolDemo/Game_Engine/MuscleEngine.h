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
	class XPad;

	class MuscleEngine : public IGameEngine// 너는 여유있을 때 인터페이스로 바꿔줄게..
	{
	public:
		MuscleEngine() = default;

		~MuscleEngine();

	private:
		std::shared_ptr<GraphicsManager> m_GraphicsManager; //그래픽스 엔진.. 완전 분리를 위해 인터페이스로.

		std::shared_ptr<SceneManager> m_SceneManager;

		std::shared_ptr<CTime> m_Time;

		std::shared_ptr<KeyBoard> m_keyBoard;

		//std::shared_ptr<XPad> m_XPad;

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

		virtual std::shared_ptr<GraphicsManager> GetGraphicsManager() override;

		virtual std::shared_ptr<SceneManager> GetSceneManager() override;

		virtual std::shared_ptr<Camera> GetMainCamera() override;

		void SetObjManager(std::shared_ptr<ObjectManager> _ObjManager);

		virtual std::shared_ptr<ObjectManager> GetObjManager() override;

		HWND GetHWND();

		static std::shared_ptr<MuscleEngine> GetInstance();

		void Finalize();
	};

}
