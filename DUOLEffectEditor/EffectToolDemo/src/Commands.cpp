#include "Commands.h"
#include "EffectEditorManager.h"
#include "LogSystem.h"
#include "ParticleRenderer.h"
#include "Transform.h"

Commander::~Commander()
{
	while (!_undoList.empty())
	{
		delete _undoList.top();
		_undoList.pop();
	}
	while (!_redoList.empty())
	{
		delete _redoList.top();
		_redoList.pop();
	}
}

Commander& Commander::Get()
{
	static Commander _instance;

	return _instance;
}

void Commander::Excute(Command* command)
{
	command->Excute();

	_undoList.push(command);

	while (!_redoList.empty())
	{
		delete _redoList.top();
		_redoList.pop();
	}

	WriteLog("Excute \n");
}

void Commander::Undo()
{
	if (!_undoList.empty())
	{
		_undoList.top()->Undo();
		_redoList.push(_undoList.top());
		_undoList.pop();
		WriteLog("Undo \n");
	}
}

void Commander::Redo()
{
	if (!_redoList.empty())
	{
		_redoList.top()->Redo();
		_undoList.push(_redoList.top());
		_redoList.pop();
		WriteLog("Redo \n");
	}
}

SelectObjectCommand::SelectObjectCommand(const std::shared_ptr<Muscle::GameObject>& selectedObject) : Command()
{
	_curSelectedObject = selectedObject;
	_prevSelectedObject = EffectEditorManager::Get().GetSelectedObject();
}

void SelectObjectCommand::Excute()
{
	EffectEditorManager::Get().SelectObject(_curSelectedObject);
}

void SelectObjectCommand::Undo()
{
	EffectEditorManager::Get().SelectObject(_prevSelectedObject);
}

void SelectObjectCommand::Redo()
{
	Excute();
}

ObjectTranslateCommand::ObjectTranslateCommand(const std::shared_ptr<Muscle::GameObject>& target, DUOLMath::Vector3 startPos,
	DUOLMath::Vector3 endPos)
{
	_target = target;
	_startPos = startPos;
	_endPos = endPos;
}

void ObjectTranslateCommand::Excute()
{
	_target->GetTransform()->SetWorldPosition(_endPos);
}

void ObjectTranslateCommand::Undo()
{
	_target->GetTransform()->SetWorldPosition(_startPos);
}

void ObjectTranslateCommand::Redo()
{
	Excute();
}

ObjectRotateCommand::ObjectRotateCommand(const std::shared_ptr<Muscle::GameObject>& target, DUOLMath::Vector3 startRot,
	DUOLMath::Vector3 endRot) :
	_target(target),
	_startRot(startRot),
	_endRot(endRot)
{
}

void ObjectRotateCommand::Excute()
{
	Command::Excute();
}

void ObjectRotateCommand::Undo()
{
	Command::Undo();
}

void ObjectRotateCommand::Redo()
{
	Excute();
}

ObjectScaleCommand::ObjectScaleCommand(const std::shared_ptr<Muscle::GameObject>& target, DUOLMath::Vector3 startScale,
	DUOLMath::Vector3 endScale) :
	_target(target),
	_startScale(startScale),
	_endScale(endScale)
{
}

void ObjectScaleCommand::Excute()
{
	Command::Excute();
}

void ObjectScaleCommand::Undo()
{
	Command::Undo();
}

void ObjectScaleCommand::Redo()
{
	Excute();
}

RenderingDataUpdateCommand::RenderingDataUpdateCommand(const std::shared_ptr<Muscle::ParticleRenderer>& target,
	MuscleGrapics::RenderingData_Particle& prev, MuscleGrapics::RenderingData_Particle& cur) :
	_target(target),
	_prev(prev),
	_cur(cur)
{
}

void RenderingDataUpdateCommand::Excute()
{
	*_target->GetParticleData() = _cur;
}

void RenderingDataUpdateCommand::Undo()
{
	*_target->GetParticleData() = _prev;
}

void RenderingDataUpdateCommand::Redo()
{
	Excute();
}
