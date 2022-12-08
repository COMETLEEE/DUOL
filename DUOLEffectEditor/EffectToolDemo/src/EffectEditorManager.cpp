#include "EffectEditorManager.h"
#include "Export\RenderingData.h"
#include "ParticleObjectManager.h"
#include "GameObject.h"
#include "FileDialogs.h"
#include "ParticleRenderer.h"

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

void EffectEditorManager::SelectObject(std::shared_ptr<Muscle::GameObject>& object)
{
	_selectedObject = ParticleObjectManager::Get().CreateParticleObject();

	_selectedParticle = _selectedObject->GetComponent<Muscle::ParticleRenderer>();
}

void EffectEditorManager::SaveParticle()
{
	auto temp = _selectedObject->GetTopParent()->GetComponent<Muscle::ParticleRenderer>();

	FileDialogs::SaveParticleFile(temp->GetParticleData());
}
