#include "IntroScene.h"
#include "ComponentsHead.h"
#include "IGameEngine.h"
#include "Inspector.h"
#include "Keyboard.h"
#include "CTime.h"
#include "../../DUOLMath/include/DUOLMath/DUOLMath.h"
#include "ObjectView.h"
#include "ParticleObjectManager.h"
#include "DockSpace.h"
#include "LogSystem.h"

IntroScene::IntroScene() : IScene("IntroScene")
{

}

IntroScene::~IntroScene()
{

}


void IntroScene::RapidUpdate()
{

}

void IntroScene::Start()
{
	auto dockSpace = Muscle::CreateGameObject()->AddComponent<DockSpace>();

	auto logSystem = Muscle::CreateGameObject()->AddComponent<LogSystem>();

	auto camera = Muscle::CreateGameObject();
	auto Camera = camera->AddComponent<Muscle::Camera>();
	auto transform = Camera->GetTransform();
	Muscle::IGameEngine::Get()->SetMainCamera(Camera);
	camera->GetTransform()->SetPosition(20, 20, 20);
	camera->GetTransform()->LookAt(DUOLMath::Vector3());

	auto BoxObject = Muscle::CreateGameObject();
	auto BoxMesh = BoxObject->AddComponent<Muscle::MeshRenderer>();
	BoxMesh->_renderingData->_objectInfo->_meshID = 1;
	BoxMesh->_renderingData->_shaderInfo->_shaderName = TEXT("Basic");
	BoxObject->GetTransform()->SetPosition(0, 10, 0);
	//Muscle::IGameEngine::Get()->GetGraphicsManager()->

	auto GridObject = Muscle::CreateGameObject();
	auto GridMesh = GridObject->AddComponent<Muscle::MeshRenderer>();
	GridMesh->_renderingData->_shaderInfo->_shaderName = TEXT("Wire");

	auto UITest = Muscle::CreateGameObject();
	auto inspector = UITest->AddComponent<Inspector>();

	auto parent = ParticleObjectManager::Get().CreateParticleObject();
	parent->SetChild(ParticleObjectManager::Get().CreateParticleObject());
	parent->SetChild(ParticleObjectManager::Get().CreateParticleObject());
	auto child = ParticleObjectManager::Get().CreateParticleObject();
	parent->SetChild(child);
	child->SetChild(ParticleObjectManager::Get().CreateParticleObject());
	ParticleObjectManager::Get().CreateParticleObject()->GetComponent<Muscle::ParticleRenderer>();

	auto objectView = Muscle::CreateGameObject()->AddComponent<ObjectView>();

}

void IntroScene::Update()
{
	float _speed = 10.0f;

	if (Muscle::KeyBoard::Get()->KeyPress(VK_SHIFT))
		_speed = 30.0f;

	// 카메라 TEST용 키 인풋
	if (Muscle::KeyBoard::Get()->KeyPress('W'))
		Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform()->Walk(1.0f * _speed * Muscle::CTime::GetGameDeltaTime());

	if (Muscle::KeyBoard::Get()->KeyPress('S'))
		Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform()->Walk(-1.0f * _speed * Muscle::CTime::GetGameDeltaTime());

	if (Muscle::KeyBoard::Get()->KeyPress('A'))
		Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform()->Strafe(-1.0 * _speed * Muscle::CTime::GetGameDeltaTime());

	if (Muscle::KeyBoard::Get()->KeyPress('D'))
		Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform()->Strafe(1.0f * _speed * Muscle::CTime::GetGameDeltaTime());

	if (Muscle::KeyBoard::Get()->KeyPress('Q'))
		Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform()->WorldUpDown(-1.0f * _speed * Muscle::CTime::GetGameDeltaTime());

	if (Muscle::KeyBoard::Get()->KeyPress('E'))
		Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform()->WorldUpDown(1.0f * _speed * Muscle::CTime::GetGameDeltaTime());

	auto _MouseMoveUpdate = []() {

		static float m_LastMousePosx = 0;
		static float m_LastMousePosy = 0;

		if ((Muscle::KeyBoard::Get()->KeyPress(VK_RBUTTON)))
		{
			// Make each pixel correspond to a quarter of a degree.
			float dx = DirectX::XMConvertToRadians(0.25f * static_cast<float>(Muscle::KeyBoard::Get()->GetPos().x - m_LastMousePosx));
			float dy = DirectX::XMConvertToRadians(0.25f * static_cast<float>(Muscle::KeyBoard::Get()->GetPos().y - m_LastMousePosy));

			Muscle::IGameEngine::Get()->GetMainCamera()->Pitch(dy);
			Muscle::IGameEngine::Get()->GetMainCamera()->RotateY(dx);
		}

		m_LastMousePosx = Muscle::KeyBoard::Get()->GetPos().x;
		m_LastMousePosy = Muscle::KeyBoard::Get()->GetPos().y;
	};
	_MouseMoveUpdate();
}