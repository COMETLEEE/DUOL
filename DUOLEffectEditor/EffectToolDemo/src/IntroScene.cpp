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
#include "GraphicsManager.h"
#include "../Common/Imgui/imgui_internal.h"
#include "ObjectManager.h"
#include "EffectEditorManager.h"
#include "HotKey.h"
#include "Commands.h"

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

	HotKey::Get().RegisterHotKey('Z', MOD_CONTROL, []() {UNDO(); });
	HotKey::Get().RegisterHotKey('Y', MOD_CONTROL, []() {REDO(); });
	HotKey::Get().RegisterHotKey(VK_DELETE, NULL, []() {
		if (EffectEditorManager::Get().GetSelectedObject())
			ParticleObjectManager::Get().DeleteParticleObject(EffectEditorManager::Get().GetSelectedObject()->GetObjectID());
		EXCUTE(new SelectObjectCommand(nullptr));
		});

	ImGui::SetCurrentContext(Muscle::IGameEngine::Get()->GetGraphicsManager()->GetImguiContext());

	auto dockSpace = Muscle::CreateGameObject()->AddComponent<DockSpace>();

	auto logSystem = Muscle::CreateGameObject()->AddComponent<LogSystem>();

	EffectEditorManager::Get().CreateMoveTool();

	auto camera = Muscle::CreateGameObject();
	auto Camera = camera->AddComponent<Muscle::Camera>();
	auto transform = Camera->GetTransform();
	Muscle::IGameEngine::Get()->SetMainCamera(Camera);
	camera->GetTransform()->SetPosition(20, 20, 20);
	camera->GetTransform()->LookAt(DUOLMath::Vector3());

	auto GridObject = Muscle::CreateGameObject();
	auto GridMesh = GridObject->AddComponent<Muscle::MeshRenderer>();
	GridMesh->_renderingData->_shaderInfo._shaderName.push_back(TEXT("Wire"));

	auto UITest = Muscle::CreateGameObject();
	auto inspector = UITest->AddComponent<Inspector>();

	auto particle = ParticleObjectManager::Get().CreateParticleObject();

	auto objectView = Muscle::CreateGameObject()->AddComponent<ObjectView>();
}

void IntroScene::Update()
{
	float _speed = 10.0f;

	if (Muscle::KeyBoard::Get()->KeyPress(VK_SHIFT))
		_speed = 30.0f;

	// ī�޶� TEST�� Ű ��ǲ
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

	EffectEditorManager::Get().MouseEventUpdate();
}