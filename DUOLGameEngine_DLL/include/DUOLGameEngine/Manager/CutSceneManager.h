#pragma once
#include <vector>

#include "DUOLGameEngine/Util/Defines.h"
#include "DUOLGameEngine/Util/SingletonBase.h"

namespace DUOLGameEngine
{
	class FadeInOut;
	class Camera;
	class Image;
	class GameObject;
	enum class FadeInOutMode;

}

namespace DUOLGameEngine
{
	class DUOL_GAMEENGINE_API CutSceneManager : public SingletonBase<CutSceneManager>
	{
		DECLARE_SINGLETON(CutSceneManager)

			DELETE_COPY_MOVE(CutSceneManager)


	private:
		virtual ~CutSceneManager() override;

		void LoadScene();

		void LoadUnityScene();

	public:
		void Initialize();

		void UnInitialize();

		void Update(float deltaTime);

	public:
		void StartCutScene();

		void PlayCutScene(float deltaTime);

		bool ChangeCutImage();

		void SetStart(bool value);

		void SkipImage();

	private:
		float _skipImageTime;

		float _currentTime;

		bool _isStart;

		bool _isEnd;

		std::string _cutImageName;

		int _nowCutCount;

		int _nowChildCutCount;

		GameObject* _nowCutObject;

		FadeInOutMode _skilFadeMode;

		DUOLGameEngine::FadeInOut* _skipFade;

		std::vector<GameObject*> _nowChildCutScene;

		DUOLGameEngine::FadeInOut* _fadeInOut;

	};

}