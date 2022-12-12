#include "EffectEditorManager.h"
#include "Export\RenderingData.h"
#include "ParticleObjectManager.h"
#include "GameObject.h"
#include "FileDialogs.h"
#include "ParticleRenderer.h"
#include "LogSystem.h"
EffectEditorManager EffectEditorManager::_instance;

EffectEditorManager::EffectEditorManager() :
	_selectedObject(nullptr),
	_selectedParticle(nullptr),
	_lastSavedParticleData(nullptr),
	_savedPath(TEXT(""))
{
}

void EffectEditorManager::NewParticle()
{
	ParticleObjectManager::Get().DeleteAllParticleObject();

	_selectedObject = ParticleObjectManager::Get().CreateParticleObject();

	_selectedParticle = _selectedObject->GetComponent<Muscle::ParticleRenderer>();

	_lastSavedParticleData.reset();

	_lastSavedParticleData = std::make_unique<MuscleGrapics::RenderingData_Particle>();

	_savedPath = (TEXT(""));

	
}

void EffectEditorManager::SelectObject(const std::shared_ptr<Muscle::GameObject>& object)
{
	_selectedObject = object;

	_selectedParticle = _selectedObject->GetComponent<Muscle::ParticleRenderer>();

	WriteLog("[Log] ObjectSelect : %s \n" , object->GetName().c_str());
}

void EffectEditorManager::SaveParticle()
{
	if (_selectedObject)
	{
		auto temp = _selectedObject->GetTopParent()->GetComponent<Muscle::ParticleRenderer>();

		SaveChildData(temp);

		FileDialogs::SaveParticleFile(temp->GetParticleData());
	}
	else
	{
		WriteLog("[Error] Object Not Selected!\n");
	}

}

void EffectEditorManager::SaveAsParticle()
{
	if (_selectedObject)
	{
		auto temp = _selectedObject->GetTopParent()->GetComponent<Muscle::ParticleRenderer>();

		SaveChildData(temp);

		FileDialogs::SaveAsParticleFile(temp->GetParticleData());
	}
	else
	{
		WriteLog("[Error] Object Not Selected!\n");
	}
}

void EffectEditorManager::LoadParticle()
{
	auto temp = FileDialogs::LoadParticleFile(); // 데이터를 가지고 다시 조립하여야 한다.

	ParticleObjectManager::Get().CreateParticleObjectFromParticleData(temp);

}

const std::shared_ptr<Muscle::GameObject>& EffectEditorManager::GetSelectedObject()
{
	return _selectedObject;
}

const std::shared_ptr<Muscle::ParticleRenderer>& EffectEditorManager::GetSelectedParticle()
{
	return _selectedParticle;
}

void EffectEditorManager::SaveChildData(const std::shared_ptr<Muscle::ParticleRenderer>& parent)
{
	for (auto iter : parent->GetGameObject()->GetChildrens())
	{
		SaveChildData(iter->GetComponent<Muscle::ParticleRenderer>());
		auto childParticle = iter->GetComponent<Muscle::ParticleRenderer>();
		parent->GetParticleData()->_childrens.push_back(*childParticle->GetParticleData());
	}
}
