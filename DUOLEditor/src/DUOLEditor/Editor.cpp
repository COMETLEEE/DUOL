#include "DUOLEditor/Editor.h"

namespace DUOLEditor
{
	Editor::Editor(const std::shared_ptr<DUOLGameEngine::Engine>& engine) :
		_gameEngine(engine)
	{
	}
}
