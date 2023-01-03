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


Inspector::Inspector(std::shared_ptr<Muscle::GameObject> _gameObject) : ImGuiRnedererBase(_gameObject),
_isTextureBoxOpen(false), _isTrailTextureBoxOpen(false)
{
}

void Inspector::Start()
{
	SetRenderingFunc();
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
			Force_over_Lifetime();
			Color_over_Lifetime();
			Size_over_Lifetime();
			Rotation_over_Lifetime();
			Noise();
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

		{
			ImGui::Begin("Tool_Box");
			if (ImGui::Button("Play"))
			{
				if (_selectedParticle)
					_selectedParticle->Play();
			}
			ImGui::SameLine();
			if (ImGui::Button("Stop"))
			{
				if (_selectedParticle)
					_selectedParticle->Stop();
			}

			if (ImGui::Button("CreateObject"))
				ParticleObjectManager::Get().CreateParticleObject();
			ImGui::SameLine();
			if (ImGui::Button("DeleteObject"))
			{
				if (EffectEditorManager::Get().GetSelectedObject())
					ParticleObjectManager::Get().DeleteParticleObject(EffectEditorManager::Get().GetSelectedObject()->GetObjectID());

				EXCUTE(new SelectObjectCommand(nullptr));
			}
			ImGui::End();

		}

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
