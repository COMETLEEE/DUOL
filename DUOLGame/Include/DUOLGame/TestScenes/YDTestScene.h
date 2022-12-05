#pragma once
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

namespace DUOLGame
{
	class YDTestScene : public DUOLGameEngine::Scene
	{
	public:
		YDTestScene();

		~YDTestScene();

	public:
		virtual void Awake() override;
	};
}

