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
	auto vectorRotate = transform->GetEuler();

	vectorRotate *= 180.0f / 3.141592f;

	if (ImGui::DragFloat3("Pos", (float*)&vectorPos, 0.1f))
		transform->SetPosition(vectorPos);
	if (ImGui::DragFloat3("Scale", (float*)&vectorScale, 0.1f))
		transform->SetScale(vectorScale);
	if (ImGui::DragFloat3("Rotate", (float*)&vectorRotate, 0.1f))
	{
		vectorRotate *= 3.141592f / 180.0f;
		transform->SetEuler(vectorRotate);
	}

	ImGui::Separator();





}