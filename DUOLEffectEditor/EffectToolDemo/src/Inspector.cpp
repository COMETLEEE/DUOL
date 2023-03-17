#include "Inspector.h"

#include "Commands.h"
#include "FileDialogs.h"
#include "ParticleRenderer.h"
#include "TextureLoader.h"
#include "../Common/Imgui/imgui.h"
#include "Transform.h"

#include "EffectEditorManager.h"
#include "KeyBoard.h"
#include "LogSystem.h"
#include "ParticleObjectManager.h"

#include "MeshRenderer.h"

Inspector::Inspector(std::shared_ptr<Muscle::GameObject> _gameObject) : ImGuiRnedererBase(_gameObject),
_isTextureBoxOpen(false), _isTrailTextureBoxOpen(false)
{
}

void Inspector::Start()
{
	SetRenderingFunc();

	for (int i = 0; i < 8; i++)
	{
		_collsionPlane[i] = Muscle::CreateGameObject();

		_collsionPlane[i]->SetIsEnable(false);

		_collsionPlane[i]->GetTransform()->SetScale(10.0f, 10.0f, 0.1f);

		auto planMesh = _collsionPlane[i]->AddComponent<Muscle::MeshRenderer>();
		planMesh->_renderingData->_shaderInfo._shaderName.push_back("Basic");
		planMesh->_renderingData->_shaderInfo._blendState = MuscleGrapics::ShaderInfo::BLENDDATA_TYPE::OIT;
		planMesh->_renderingData->_objectInfo._meshName = "Box";
		planMesh->_renderingData->_materialInfo._color = DUOLMath::Vector4(0.8f, 0.8f, 0, 0.5f);

		auto normalVec = Muscle::CreateGameObject();

		normalVec->SetParent(_collsionPlane[i]);

		normalVec->GetTransform()->SetScale(0.01f, 0.01f, 20.0f);

		normalVec->GetTransform()->SetPosition(0, 0, 10.0f);

		auto normalMesh = normalVec->AddComponent<Muscle::MeshRenderer>();
		normalMesh->_renderingData->_shaderInfo._shaderName.push_back("Basic");
		normalMesh->_renderingData->_shaderInfo._blendState = MuscleGrapics::ShaderInfo::BLENDDATA_TYPE::OIT;
		normalMesh->_renderingData->_objectInfo._meshName = "Box";
		normalMesh->_renderingData->_materialInfo._color = DUOLMath::Vector4(0.0f, 0.0f, 1.0f, 0.5f);

	}
}

void Inspector::Update()
{
	if (_selectedParticle && _selectedParticle->GetParticleData()->_collision._useModule)
	{
		auto& collision = _selectedParticle->GetParticleData()->_collision;

		for (int i = 0; i < collision._planeCount; i++)
		{
			_collsionPlane[i]->SetIsEnable(true);

			_collsionPlane[i]->GetTransform()->SetPosition(collision._planePos[i]);

			_collsionPlane[i]->GetTransform()->SetLook(collision._planeNormalVec[i]);

		}
		for (int i = collision._planeCount; i < 8; i++)
		{
			_collsionPlane[i]->SetIsEnable(false);
		}
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			_collsionPlane[i]->SetIsEnable(false);
		}
	}

}

void Inspector::SetRenderingFunc()
{
	auto temp = [&]()
	{
		if (_selectedParticle != EffectEditorManager::Get().GetSelectedParticle())
		{
			_selectedParticle = EffectEditorManager::Get().GetSelectedParticle();

			_selectedGameObject = EffectEditorManager::Get().GetSelectedObject();

			_isTextureBoxOpen = false;
		}

		// Inspector
		ImGui::Begin("ParticleSystem");

		if (_selectedGameObject)
		{
			GameObjectInfo();
			TranformInfo();
		}

		if (_selectedParticle)
		{
			// 이 부분을 리플렉션으로 하면 참 예쁠것 같다...
			ParticleSystemCommonInfo();
			Emission();
			Shape();
			Velocity_Over_Lifetime();
			Limit_Velocity_Over_Lifetime();
			Force_over_Lifetime();
			Color_over_Lifetime();
			Size_over_Lifetime();
			Rotation_over_Lifetime();
			Noise();
			Collision();
			Texture_Sheet_Animation();
			Trails();
			Renderer();
		}
		else
		{
			ImGui::Text("Not Selected Particle Object");
		}
		// 이 부분을 리플렉션으로 하면 참 예쁠것 같다...
		// 나중에 여유가 생기면 부스트 라이브러리의 리플렉션 사용해보자.
		ImGui::End();

		if (_selectedParticle)
		{
			if (Muscle::KeyBoard::Get()->KeyUp(VK_LBUTTON) || Muscle::KeyBoard::Get()->KeyUp(VK_RETURN))
			{
				EffectEditorManager::Get().CheckChangedData_Update(*_selectedParticle->GetParticleData());
			}
		}

	};
	_renderingFunc = temp;
}
