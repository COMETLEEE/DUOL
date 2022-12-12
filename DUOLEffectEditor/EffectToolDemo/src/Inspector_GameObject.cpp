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
	static float vectorRotate[3] = { 0,0,0 }; //DUOLMath::Quaternion::ConvertQuaternionToEuler(transform->GetQuaternion());


	ImGui::DragFloat3("Pos", (float*)&vectorPos, 0.01f);
	ImGui::DragFloat3("Scale", (float*)&vectorScale, 0.01f);
	ImGui::DragFloat3("Rotate", (float*)&vectorRotate, 0.01f);

	ImGui::Separator();

	transform->SetPosition(vectorPos);
	transform->SetScale(vectorScale);

	float x = DUOLMath::XMConvertToRadians(vectorRotate[0]);
	float y = DUOLMath::XMConvertToRadians(vectorRotate[1]);
	float z = DUOLMath::XMConvertToRadians(vectorRotate[2]);

	double c1 = cos(x / 2);
	double s1 = sin(x / 2);
	double c2 = cos(y / 2);
	double s2 = sin(y / 2);
	double c3 = cos(z / 2);
	double s3 = sin(z / 2);

	double c1c2 = c1 * c2;
	double s1s2 = s1 * s2;

	DUOLMath::Quaternion temp;

	temp.w = c1c2 * c3 - s1s2 * s3;
	temp.x = c1c2 * s3 + s1s2 * c3;
	temp.z = s1 * c2 * c3 + c1 * s2 * s3;
	temp.y = c1 * s2 * c3 - s1 * c2 * s3;



	transform->SetQuaternion(temp);

	// todo : 나중에 혜성이형한테 물어보자. 셰이더 코드에도 회전 행렬 관련 구현이 없으니 나중으로 미루자..
	// https://www.euclideanspace.com/maths/geometry/rotations/conversions/eulerToQuaternion/index.htm 레퍼런스

}