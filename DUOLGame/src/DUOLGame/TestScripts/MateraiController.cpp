#include "MateraiController.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Light.h"
#include "DUOLGameEngine/ECS/Object/Material.h"
#include "DUOLGameEngine/ECS/Object/Mesh.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"

DUOLGame::MaterialController::MaterialController(DUOLGameEngine::GameObject* owner) : MonoBehaviourBase(owner, TEXT("MaterialController"))
{
}

DUOLGame::MaterialController::~MaterialController()
{
}

void DUOLGame::MaterialController::OnStart()
{
}

void DUOLGame::MaterialController::OnUpdate(float deltaTime)
{

	//auto mat = DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(L"SphereMat");

	//if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::O))
	//{
	//	_metallic += 0.0001f;
	//	_metallic = std::ranges::clamp(_metallic, 0.0f, 1.0f);

	//	mat->SetMetaillic(_metallic);
	//}

	//if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::P))
	//{
	//	_metallic -= 0.01f;
	//	_metallic = std::ranges::clamp(_metallic, 0.0f, 1.0f);

	//	mat->SetMetaillic(_metallic);
	//}

	//if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::U))
	//{
	//	_roughness += 0.001f;
	//	_roughness = std::ranges::clamp(_roughness, 0.0f, 1.0f);

	//	mat->SetRoughness(_roughness);
	//}

	//if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::I))
	//{
	//	_roughness -= 0.01f;
	//	_roughness = std::ranges::clamp(_roughness, 0.0f, 1.0f);

	//	mat->SetRoughness(_roughness);
	//}

	auto mat = GetGameObject()->GetComponent<DUOLGameEngine::Light>();

	if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::O))
	{
		_metallic += 1.0f;

		mat->SetIntensity(_metallic);
	}

	if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::P))
	{
		_metallic -= 1.0f;

		mat->SetIntensity(_metallic);
	}

	if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::U))
	{
		_roughness += 1.0f;

		mat->SetAttenuationRadius(_roughness);
	}

	if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::I))
	{
		_roughness -= 1.0f;

		mat->SetAttenuationRadius(_roughness);
	}
}
