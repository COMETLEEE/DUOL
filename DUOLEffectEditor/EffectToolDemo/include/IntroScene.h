#pragma once

#include "IScene.h"

class MoveTool;
namespace Muscle
{
	class GameObject;
}

class IntroScene : public Muscle::IScene
{
public:
	IntroScene();

	~IntroScene() override;

public:
	virtual void RapidUpdate() override;

	virtual void Start() override;

	virtual void Update() override;

};

