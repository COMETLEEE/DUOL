#include "Inspector.h"
#include "../Common/Imgui/imgui.h"
#include "ParticleRenderer.h"
#include "StringHelper.h"
#include "EffectEditorManager.h"
#include "Transform.h"
#include "../../DUOLMath/include/DUOLMath/DUOLMath.h"


void Inspector::GameObjectInfo()
{
	char str[50];

	strcpy_s(str, _selectedGameObject->GetName().c_str());

	ImGui::InputText("Object Name", str, 50);

	_selectedGameObject->SetName(str);

	ImGui::Separator();
}

void Inspector::TranformInfo()
{

	float postion[3]; float scale[3]; float rotate[3];

	auto transform = _selectedGameObject->GetTransform();

	auto vectorPos = transform->GetPosition();
	auto vectorScale = transform->GetScale();
	auto vectorRotate = transform->GetEuler(); //DUOLMath::Quaternion::ConvertQuaternionToEuler(transform->GetQuaternion());

	ImGui::DragFloat3("Pos", (float*)&vectorPos, 0.01f);
	ImGui::DragFloat3("Scale", (float*)&vectorScale, 0.01f);
	ImGui::DragFloat3("Rotate", (float*)&vectorRotate, 0.01f);

	ImGui::Separator();

	transform->SetPosition(vectorPos);
	transform->SetScale(vectorScale);
	transform->SetEuler(vectorRotate);

}