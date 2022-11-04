#include "DUOLEditor/Editor.h"

namespace DUOLEditor
{
	Editor::Editor(const std::shared_ptr<DUOLGameEngine::Engine>& engine) :
		_gameEngine(engine)
	{

	}

	Editor::~Editor()
	{
		_gameEngine.reset();
	}

	void Editor::PreUpdate(float deltaTime)
	{

	}

	void Editor::LateUpdate(float deltaTime)
	{

	}

	void Editor::Update(float deltaTime)
	{
		PreUpdate(deltaTime);

		LateUpdate(deltaTime);
	}
}