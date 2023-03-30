#include "Commands.h"
#include "EffectEditorManager.h"
#include "LogSystem.h"
#include "ParticleRenderer.h"
#include "Transform.h"
#include "ParticleObjectManager.h"
#include "MeshRenderer.h"
#include "IGameEngine.h"
#include "ObjectManager.h"

Commander::~Commander()
{
	Clear();
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

void Commander::Clear()
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

ObjectTranslateCommand::ObjectTranslateCommand(const std::shared_ptr<Muscle::GameObject>& target,
	DUOLMath::Vector3 endPos)
{
	_target = target;
	_startPos = EffectEditorManager::Get()._prePos;
	_endPos = endPos;
	EffectEditorManager::Get()._prePos = endPos;
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

ObjectRotateCommand::ObjectRotateCommand(const std::shared_ptr<Muscle::GameObject>& target,
	DUOLMath::Vector3 endRot) :
	_target(target),
	_startRot(EffectEditorManager::Get()._preRotate),
	_endRot(endRot)
{
	EffectEditorManager::Get()._preRotate = endRot;
}

void ObjectRotateCommand::Excute()
{
	_target->GetTransform()->SetEuler(_endRot);
}

void ObjectRotateCommand::Undo()
{
	_target->GetTransform()->SetEuler(_startRot);
}

void ObjectRotateCommand::Redo()
{
	Excute();
}

ObjectScaleCommand::ObjectScaleCommand(const std::shared_ptr<Muscle::GameObject>& target,
	DUOLMath::Vector3 endScale) :
	_target(target),
	_startScale(EffectEditorManager::Get()._preScale),
	_endScale(endScale)
{
	EffectEditorManager::Get()._preScale = endScale;
}

void ObjectScaleCommand::Excute()
{
	_target->GetTransform()->SetScale(_endScale);
}

void ObjectScaleCommand::Undo()
{
	_target->GetTransform()->SetScale(_startScale);
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
	if (_target)
		*_target->GetParticleData() = _cur;
}

void RenderingDataUpdateCommand::Undo()
{
	if (_target)
		*_target->GetParticleData() = _prev;
}

void RenderingDataUpdateCommand::Redo()
{
	Excute();
}

ObjectDeleteCommand::ObjectDeleteCommand(const std::shared_ptr<Muscle::GameObject>& target) :
	_target(target),
	_isDelete(false)
{
}
ObjectDeleteCommand::~ObjectDeleteCommand()
{
	if (_isDelete)
		ParticleObjectManager::Get().ExcuteDeleteParticleObject(_target->GetObjectID());
}
void ObjectDeleteCommand::Excute()
{
	_target->SetIsEnable(false);
	_isDelete = true;
}

void ObjectDeleteCommand::Undo()
{
	_target->SetIsEnable(true);
	_isDelete = false;
}

void ObjectDeleteCommand::Redo()
{
	_target->SetIsEnable(false);
	_isDelete = true;
}

ObjectCreateCommand::ObjectCreateCommand(const std::shared_ptr<Muscle::GameObject>& target) :
	_target(target)
{
}
ObjectCreateCommand::~ObjectCreateCommand()
{
	if (ParticleObjectManager::Get()._particleObjects.find(_target->GetObjectID()) == ParticleObjectManager::Get()._particleObjects.end())
		Muscle::IGameEngine::Get()->GetObjManager()->DeleteObject(_target);
}
void ObjectCreateCommand::Excute()
{
	_target->AddComponent<Muscle::ParticleRenderer>();

	auto debugBox = _target->AddComponent<Muscle::MeshRenderer>()->_renderingData;

	debugBox->_shaderInfo._shaderName.push_back("Basic");

	debugBox->_shaderInfo._rasterizerState = MuscleGrapics::RASTERIZER_STATE::WIREFRAME;

	debugBox->_objectInfo._meshName = "Box";

	ParticleObjectManager::Get()._particleObjects.insert({ _target->GetObjectID(),_target });

}

void ObjectCreateCommand::Undo()
{
	_target->SetIsEnable(false);
	ParticleObjectManager::Get()._particleObjects.erase(_target->GetObjectID());
}

void ObjectCreateCommand::Redo()
{
	_target->SetIsEnable(true);
	ParticleObjectManager::Get()._particleObjects.insert({ _target->GetObjectID(),_target });
}

SetParentsCommand::SetParentsCommand(const std::shared_ptr<Muscle::GameObject>& child, const std::shared_ptr<Muscle::GameObject>& parents) :
	_child(child),
	_parents(parents),
	_preParents(child->GetParent())
{
}

void SetParentsCommand::Excute()
{
	_prevMatrix = _child->GetTransform()->GetWorldTM();
	_child->SetParent(_parents);
	_child->GetTransform()->SetXMWorldTM(_prevMatrix);
}
void SetParentsCommand::Undo()
{
	_child->SetParent(_preParents);
	_child->GetTransform()->SetXMWorldTM(_prevMatrix);
}
void SetParentsCommand::Redo()
{
	Excute();
}