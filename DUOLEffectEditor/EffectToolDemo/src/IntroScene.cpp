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
#include "SkyBox.h"
#include "DebugBox.h"

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
	/// ����Ű ����...!!
	HotKey::Get().RegisterHotKey('Z', MOD_CONTROL, []() {UNDO(); });
	HotKey::Get().RegisterHotKey('Y', MOD_CONTROL, []() {REDO(); });
	HotKey::Get().RegisterHotKey(VK_DELETE, NULL, []() {
		if (EffectEditorManager::Get().GetSelectedObject())
			ParticleObjectManager::Get().DeleteParticleObject(EffectEditorManager::Get().GetSelectedObject()->GetObjectID());
		EXCUTE(new SelectObjectCommand(nullptr));
		});
	/// ����Ű ����...!!

	ImGui::SetCurrentContext(Muscle::IGameEngine::Get()->GetGraphicsManager()->GetImguiContext());

	auto dockSpace = Muscle::CreateGameObject()->AddComponent<DockSpace>();

	auto logSystem = Muscle::CreateGameObject()->AddComponent<LogSystem>();

	auto camera = Muscle::CreateGameObject();
	auto Camera = camera->AddComponent<Muscle::Camera>();
	auto transform = Camera->GetTransform();
	Muscle::IGameEngine::Get()->SetMainCamera(Camera);
	camera->GetTransform()->SetPosition(20, 20, 20);
	camera->GetTransform()->LookAt(DUOLMath::Vector3());

	auto skyBox = Muscle::CreateGameObject();
	skyBox->SetParent(camera);
	auto skyBoxComponent = skyBox->AddComponent<Muscle::SkyBox>();
	skyBoxComponent->Initialize(TEXT("Asset/Particle/Resource/Image/snowcube1024.dds"), Camera);
	EffectEditorManager::Get().CreateMoveTool();

	auto GridObject = Muscle::CreateGameObject();
	auto GridMesh = GridObject->AddComponent<Muscle::MeshRenderer>();
	GridMesh->_renderingData->_shaderInfo._shaderName.push_back("Wire");
	GridMesh->_renderingData->_objectInfo._meshName = "Grid";

	auto UITest = Muscle::CreateGameObject();
	auto inspector = UITest->AddComponent<Inspector>();

	auto particle = ParticleObjectManager::Get().CreateParticleObject();

	auto objectView = Muscle::CreateGameObject()->AddComponent<ObjectView>();

	auto debugBox = Muscle::CreateGameObject();
	debugBox->AddComponent<DebugBox>()->SetSkyBox(skyBoxComponent);


	// PBR Todo :
	/*for (int i = 0; i <= 10; i++)
	{
		for (int j = 0; j <= 10; j++)
		{
			for (int k = 0; k <= 10; k++)
			{
				auto pbrTestObject = Muscle::CreateGameObject();
				auto mesh = pbrTestObject->AddComponent<Muscle::MeshRenderer>();
				mesh->_renderingData->_shaderInfo._shaderName.push_back("Basic");
				mesh->_renderingData->_materialInfo._metalicRoughnessAoSpecular.x = 0.1f * i;
				mesh->_renderingData->_materialInfo._metalicRoughnessAoSpecular.y = 0.1f * j;
				mesh->_renderingData->_materialInfo._metalicRoughnessAoSpecular.z = 0.1f;
				mesh->_renderingData->_materialInfo._metalicRoughnessAoSpecular.w = 0.1f * k;

				mesh->_renderingData->_materialInfo._color = DUOLMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);

				mesh->_renderingData->_objectInfo._meshName = "Sphere";
				pbrTestObject->GetTransform()->SetPosition(2 * i, 2 * j, 2 * k);
			}
		}
	}

	auto light = Muscle::CreateGameObject();
	auto& temp = light->AddComponent<Muscle::Light>()->_lightInfo;
	temp.Direction = DUOLMath::Vector3(-1.0f, -1.0f, -1.0f);
	temp.Direction.Normalize();
	temp.Color = DUOLMath::Vector3(1.0f, 1.0f, 1.0f);
	light->GetTransform()->SetPosition(20, 20, 20);*/
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