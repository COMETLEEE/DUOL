#pragma once
//#include "MuscleEngine.h"

/// <summary>
/// 게임엔진의 인터페이스 빼자.
/// 2022.07.30 신성현
/// </summary>
///
#include <memory>
#include <Windows.h>

namespace Muscle
{
	class GameObject;
	class Camera;
	class GraphicsManager;
	class SceneManager;
	class IScene;
	class ObjectManager;

	class IGameEngine
	{
	public:
		IGameEngine(const IGameEngine& other) = delete;

		IGameEngine& operator =(const IGameEngine& other) = delete;

		IGameEngine(IGameEngine&& other) = delete;

	protected:
		IGameEngine() = default;

		virtual ~IGameEngine() = default;

	private:
		static std::shared_ptr<IGameEngine> _gameEngine;

		bool _isDebug = false;
	public:
		virtual void Initialize(HWND hWnd, int Width, int height) abstract;

		virtual void Update() abstract;

		virtual void Render() abstract;

		virtual void OnResize() abstract;

		virtual void SetMainCamera(std::shared_ptr<Camera> _MainCamera) abstract;

		virtual std::shared_ptr<Camera> GetMainCamera() abstract;

		virtual void InsertObject(std::shared_ptr<GameObject> _GameObject) abstract;

		virtual std::shared_ptr<GraphicsManager> GetGraphicsManager() abstract;

		virtual std::shared_ptr<SceneManager> GetSceneManager() abstract;

		virtual std::shared_ptr<ObjectManager> GetObjManager() abstract;

		static std::shared_ptr<IGameEngine> Get();

		bool GetisDebug() { return _isDebug; };

		void TurnOnDebug() { _isDebug = true; };

		void TurnOffDebug() { _isDebug = false; };

		void Finalize();
	};
	extern "C" LRESULT CALLBACK WndProc(HWND hwnd, UINT uint, WPARAM wparam, LPARAM lparam);


}

