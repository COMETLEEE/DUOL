#include "Inspector.h"
#include "FileDialogs.h"
#include "ParticleRenderer.h"
#include "TextureLoader.h"
#include "../Common/Imgui/imgui.h"

#include "EffectEditorManager.h"
#include "LogSystem.h"
#include "ParticleObjectManager.h"

Inspector::Inspector(std::shared_ptr<Muscle::GameObject> _gameObject) : ImGuiRnedererBase(_gameObject)
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
		_selectedParticle = EffectEditorManager::Get().GetSelectedParticle();

		_selectedGameObject = EffectEditorManager::Get().GetSelectedObject();
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
			Color_by_Speed();
			Size_over_Lifetime();
			Size_by_Speed();
			Rotation_over_Lifetime();
			Rotation_by_Speed();
			External_Forces();
			Noise();
			Collision();
			Triggers();
			SubEmitters();
			Texture_Sheet_Animation();
			Lights();
			Trails();
			Custom_Data();
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
				_selectedParticle->Play();
			ImGui::SameLine();
			if (ImGui::Button("Stop"))
				_selectedParticle->Stop();

			if (ImGui::Button("CreateObject"))
				ParticleObjectManager::Get().CreateParticleObject();
			ImGui::SameLine();
			if (ImGui::Button("DeleteObject"))
			{
				if (EffectEditorManager::Get().GetSelectedObject())
					ParticleObjectManager::Get().DeleteParticleObject(EffectEditorManager::Get().GetSelectedObject()->GetObjectID());
				EffectEditorManager::Get().SelectObject(nullptr);
			}
			ImGui::End();

		}

		ImGui::Begin("TextureBox", 0/*, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize
			^ ImGuiWindowFlags_::ImGuiWindowFlags_NoMove
			^ ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground*/);
		ImGui::SetWindowSize(ImVec2(300, 300));

		ImGui::BeginChild(ImGui::GetID("TextureBox"), ImVec2(200, 200));

		if (ImGui::Button("LoadTexture", ImVec2(50, 30)))
		{
			auto str = FileDialogs::OpenTextureFile();

			if (!str.empty())
			{
				TextureLoader::InsertTexture(str);
			}
		}
		for (auto& iter : TextureLoader::GetTextureFilePaths())
		{
			if (ImGui::ImageButton(TextureLoader::GetTexture(iter), ImVec2(100, 100))) // 텍스쳐 버튼을 만들고.
				_selectedParticle->GetParticleData()->_commonInfo._refTexturePath = iter; // 버튼을 클릭하면 파티클의 텍스쳐를 변경한다.

		}

		ImGui::EndChild();
		ImGui::End();

	};
	_renderingFunc = temp;
}
