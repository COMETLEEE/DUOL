#include "EffectEditorManager.h"
#include "Export\RenderingData.h"
#include "ParticleObjectManager.h"
#include "GameObject.h"
#include "FileDialogs.h"
#include "ParticleRenderer.h"
#include "LogSystem.h"
#include "MoveTool.h"
#include "Keyboard.h"
#include "IGameEngine.h"
#include "Camera.h"
#include "ObjectManager.h"
#include "MoveTool.h"
#include "Transform.h"
#include "Commands.h"

EffectEditorManager EffectEditorManager::_instance;

EffectEditorManager::EffectEditorManager() :
	_selectedObject(nullptr),
	_selectedParticle(nullptr),
	_lastChangedParticleData(std::make_unique<MuscleGrapics::RenderingData_Particle>()),
	_savedPath(TEXT(""))
{
}

void EffectEditorManager::CreateMoveTool()
{
	_moveToolParent = Muscle::CreateGameObject();

	auto XObject = Muscle::CreateGameObject();
	XObject->AddComponent<MoveTool>()->SetType(1);

	auto YObject = Muscle::CreateGameObject();
	YObject->AddComponent<MoveTool>()->SetType(2);

	auto ZObject = Muscle::CreateGameObject();
	ZObject->AddComponent<MoveTool>()->SetType(3);

	_moveToolParent->SetChild(XObject);
	_moveToolParent->SetChild(YObject);
	_moveToolParent->SetChild(ZObject);

	_moveToolParent->SetIsEnable(false);
}

void EffectEditorManager::MouseEventUpdate()
{
	auto _MouseMoveUpdate = []() {

		static float m_LastMousePosx = 0;

		static float m_LastMousePosy = 0;

		if ((Muscle::KeyBoard::Get()->KeyPress(VK_RBUTTON)))
		{
			float dx = DirectX::XMConvertToRadians(0.25f * static_cast<float>(Muscle::KeyBoard::Get()->GetPos().x - m_LastMousePosx));

			float dy = DirectX::XMConvertToRadians(0.25f * static_cast<float>(Muscle::KeyBoard::Get()->GetPos().y - m_LastMousePosy));

			Muscle::IGameEngine::Get()->GetMainCamera()->Pitch(dy);

			Muscle::IGameEngine::Get()->GetMainCamera()->RotateY(dx);
		}

		m_LastMousePosx = Muscle::KeyBoard::Get()->GetPos().x;

		m_LastMousePosy = Muscle::KeyBoard::Get()->GetPos().y;
	};
	_MouseMoveUpdate();

	if (_selectedObject)
	{
		auto targetPos = _selectedObject->GetTransform()->GetWorldPosition();

		DUOLMath::Vector4 temp(targetPos.x, targetPos.y, targetPos.z, 1.0f);

		auto viewproj = Muscle::IGameEngine::Get()->GetMainCamera()->View() * Muscle::IGameEngine::Get()->GetMainCamera()->Proj();

		temp = DUOLMath::Vector4::Transform(temp, viewproj);

		temp.x = temp.x / temp.w * (Muscle::IGameEngine::Get()->GetMainCamera()->GetWidth() / 2);

		temp.y = temp.y / temp.w * (Muscle::IGameEngine::Get()->GetMainCamera()->GetHeight() / 2);

		temp.z = 100.0f;

		temp.w = 1.0f;

		temp = DUOLMath::Vector4::Transform(temp, Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform()->GetWorldTM());

		targetPos.x = temp.x;

		targetPos.y = temp.y;

		targetPos.z = temp.z;

		_moveToolParent->GetTransform()->SetPosition(targetPos);
	}

	if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && Muscle::KeyBoard::Get()->KeyDown(VK_LBUTTON))
	{
		auto pos = Muscle::KeyBoard::Get()->GetPos();

		auto objectID = Muscle::IGameEngine::Get()->GetMainCamera()->Pick(pos.x, pos.y);

		WriteLog("Picking MousePos : %d , %d\n", (int)pos.x, (int)pos.y);

		if (objectID == 0)
		{
			EXCUTE(new SelectObjectCommand(nullptr));

			WriteLog("Picking Faild\n");
		}
		else
		{
			WriteLog("Picked Object : %d\n", objectID);

			auto selectObject = Muscle::IGameEngine::Get()->GetObjManager()->GetGameObject(objectID);

			_moveTool = selectObject->GetComponent<MoveTool>();

			if (selectObject->GetComponent<Muscle::ParticleRenderer>())
			{
				EXCUTE(new SelectObjectCommand(selectObject));
			}
			else if (_moveTool)
			{
				_moveTool->Select();
			}
			else
			{
				EXCUTE(new SelectObjectCommand(nullptr));
			}
		}
	}
	else if (Muscle::KeyBoard::Get()->KeyPress(VK_LBUTTON))
	{

	}
	else if (Muscle::KeyBoard::Get()->KeyUp(VK_LBUTTON))
	{
		if (_moveTool)
		{
			_moveTool->UnSelect();
			_moveTool = nullptr;
		}
	}
}

void EffectEditorManager::NewParticle()
{
	COMMANDCLEAR();

	ParticleObjectManager::Get().DeleteAllParticleObject();

	_selectedObject = ParticleObjectManager::Get().CreateParticleObject();

	_selectedParticle = _selectedObject->GetComponent<Muscle::ParticleRenderer>();

	_lastChangedParticleData.reset();

	_lastChangedParticleData = std::make_unique<MuscleGrapics::RenderingData_Particle>();

	_savedPath = (TEXT(""));


}

void EffectEditorManager::SelectObject(const std::shared_ptr<Muscle::GameObject>& object)
{
	if (_selectedParticle)
	{
		auto& particleData = _selectedParticle->GetParticleData();

		auto iter = std::find(particleData->shaderName.begin(), particleData->shaderName.end(), "ParticleOutLinePass");

		if (iter != particleData->shaderName.end())
			particleData->shaderName.erase(iter);
	}

	_selectedObject = object;

	if (_selectedObject)
	{
		_selectedParticle = _selectedObject->GetComponent<Muscle::ParticleRenderer>();

		*_lastChangedParticleData = *_selectedParticle->GetParticleData();

		_moveToolParent->SetIsEnable(true);

		_prePos = _selectedObject->GetTransform()->GetWorldPosition();

		_preScale = _selectedObject->GetTransform()->GetScale();

		_preRotate = _selectedObject->GetTransform()->GetEuler();

		_preName = _selectedObject->GetName();


		auto& particleData = _selectedParticle->GetParticleData();

		auto iter = std::find(particleData->shaderName.begin(), particleData->shaderName.end(), "ParticleOutLinePass");

		if (iter == particleData->shaderName.end())
			_selectedParticle->GetParticleData()->shaderName.push_back("ParticleOutLinePass");

	}
	else
	{
		WriteLog("[Log] ObjectSelect : %s \n", "nullptr");

		_selectedParticle = nullptr;

		_moveToolParent->SetIsEnable(false);

		return;
	}
	WriteLog("[Log] ObjectSelect : %s \n", object->GetName().c_str());
}

void EffectEditorManager::SaveParticle()
{
	if (_selectedObject)
	{
		auto temp = _selectedObject->GetTopParent()->GetComponent<Muscle::ParticleRenderer>();

		SaveChildData(temp);

		FileDialogs::SaveParticleFile(*temp->GetParticleData());

		ClearChildData(*temp->GetParticleData());
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

		FileDialogs::SaveAsParticleFile(*temp->GetParticleData());

		ClearChildData(*temp->GetParticleData());
	}
	else
	{
		WriteLog("[Error] Object Not Selected!\n");
	}
}

void EffectEditorManager::LoadParticle()
{
	COMMANDCLEAR();

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

void EffectEditorManager::CheckChangedData_Update(MuscleGrapics::RenderingData_Particle& paritcleData)
{
	if (*_lastChangedParticleData != paritcleData)
	{
		EXCUTE(new RenderingDataUpdateCommand(GetSelectedParticle(), *_lastChangedParticleData, paritcleData));
		*_lastChangedParticleData = paritcleData;
	}

	if (_selectedObject)
	{
		if (_prePos != _selectedObject->GetTransform()->GetWorldPosition())
		{
			EXCUTE(new ObjectTranslateCommand(_selectedObject, _selectedObject->GetTransform()->GetWorldPosition()));
		}
		if (_preScale != _selectedObject->GetTransform()->GetScale())
		{
			EXCUTE(new ObjectScaleCommand(_selectedObject, _selectedObject->GetTransform()->GetScale()));
		}
		if (_preRotate != _selectedObject->GetTransform()->GetEuler())
		{
			EXCUTE(new ObjectRotateCommand(_selectedObject, _selectedObject->GetTransform()->GetEuler()));
		}
	}
}

void EffectEditorManager::SaveChildData(const std::shared_ptr<Muscle::ParticleRenderer>& parent)
{
	std::vector<MuscleGrapics::RenderingData_Particle>().swap(parent->GetParticleData()->_childrens);

	for (auto iter : parent->GetGameObject()->GetChildrens())
	{
		if (!iter->GetIsEnable()) continue;

		SaveChildData(iter->GetComponent<Muscle::ParticleRenderer>());

		auto childParticle = iter->GetComponent<Muscle::ParticleRenderer>();

		parent->GetParticleData()->_childrens.push_back(*childParticle->GetParticleData());
	}
}

void EffectEditorManager::ClearChildData(MuscleGrapics::RenderingData_Particle& parentData)
{
	for (auto iter : parentData._childrens)
	{
		ClearChildData(iter);
	}

	std::vector<MuscleGrapics::RenderingData_Particle>().swap(parentData._childrens);
}
