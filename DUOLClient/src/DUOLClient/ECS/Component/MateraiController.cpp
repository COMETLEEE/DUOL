#include "DUOLClient/ECS/Component/MateraiController.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Light.h"
#include "DUOLGameEngine/ECS/Object/Material.h"
#include "DUOLGameEngine/ECS/Object/Mesh.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"

#include <rttr/registration>

#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"
#include "DUOLGameEngine/ECS/Component/SkinnedMeshRenderer.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::MaterialController>("MaterialController")
			.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

DUOLClient::MaterialController::MaterialController() :
	MonoBehaviourBase(nullptr, L"")
{
}

DUOLClient::MaterialController::MaterialController(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) : MonoBehaviourBase(owner, name)
{
}

DUOLClient::MaterialController::~MaterialController()
{
}

void DUOLClient::MaterialController::OnStart()
{
	checkOnce = true;
}

void DUOLClient::MaterialController::OnUpdate(float deltaTime)
{
	if (checkOnce)
	{
		checkOnce = false;

		int objCount = 100;

		for (int i = 0; i < objCount; i++)
		{
			for (int j = 0; j < objCount; j++)
			{
				auto obj = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateFromFBXModel(_T("player_sword"));
				obj->GetTransform()->SetPosition({ static_cast<float>(i), static_cast<float>(j),0 });
				obj->SetIsStatic(true);
				for (auto a : obj->GetTransform()->GetChildGameObjects())
				{
					meshobj = a;
					meshobj->SetIsStatic(true);
				}
			}
		}


	}

	if(DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::I))
	{
		auto meshRenderer = meshobj->GetComponent<DUOLGameEngine::MeshRenderer>();
		bool inverse = !meshRenderer->GetGpuInstancing();
		meshRenderer->SetGPUInstancing(inverse);
	}
}
