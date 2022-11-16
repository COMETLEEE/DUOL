#include "pch.h"
#include "Inspector.h"

#include "FileDialogs.h"

Inspector::Inspector(std::shared_ptr<Muscle::GameObject> _gameObject) : ImGuiRnedererBase(_gameObject)
{
}

void Inspector::Start()
{
	SetRenderingFunc();
}

void Inspector::SetMyParticle(std::shared_ptr<Muscle::ParticleRenderer>& myParticle)
{

	_myParticle = myParticle;
}

void Inspector::SetRenderingFunc()
{
	
	auto temp = [&]()
	{

		// Inspector
		{
			constexpr int offset_x = 200;
			static int counter = 0;

			ImGui::Begin("ParticleSystem");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("Duration"); ImGui::SameLine(offset_x); ImGui::InputFloat("Duration", &_myParticle->GetParticleData()->_commonInfo->_duration, 0.1f, 1.0f, "%.3f");

			ImGui::Text("Looping"); ImGui::SameLine(offset_x); ImGui::Checkbox("Looping", &_myParticle->GetParticleData()->_commonInfo->_looping);      // Edit bools storing our window open/close state

			ImGui::Text("StartDelay"); ImGui::SameLine(offset_x); ImGui::InputFloat("StartDelay", &_myParticle->GetParticleData()->_commonInfo->_startDelay[0], 0.1f, 1.0f, "%.3f");

			ImGui::Text("StartLifeTime"); ImGui::SameLine(offset_x); ImGui::InputFloat("StartLifeTime", &_myParticle->GetParticleData()->_commonInfo->_startLifeTime[0], 0.01f, 1.0f, "%.3f");

			ImGui::Text("StartSpeed"); ImGui::SameLine(offset_x); ImGui::InputFloat("StartSpeed", &_myParticle->GetParticleData()->_commonInfo->_startSpeed[0], 0.1f, 1.0f, "%.3f");

			ImGui::Text("StartSize"); ImGui::SameLine(offset_x); ImGui::InputFloat2("StartSize", reinterpret_cast<float*>(&_myParticle->GetParticleData()->_commonInfo->_startSize[0]));

			ImGui::Text("StartRotation"); ImGui::SameLine(offset_x); ImGui::InputFloat("StartRotation", &_myParticle->GetParticleData()->_commonInfo->_startRotation[0], 0.1f, 1.0f, "%.3f");

			ImGui::Text("StartColor"); ImGui::SameLine(offset_x); ImGui::ColorEdit4("StartColor", reinterpret_cast<float*>(&_myParticle->GetParticleData()->_commonInfo->_startColor[0])); // Edit 3 floats representing a color

			ImGui::Text("GravityModifier"); ImGui::SameLine(offset_x); ImGui::InputFloat("GravityModifier", &_myParticle->GetParticleData()->_commonInfo->_gravityModifier[0], 0.1f, 1.0f, "%.3f");

			ImGui::Text("MaxParticles"); ImGui::SameLine(offset_x); ImGui::InputInt("MaxParticles", &_myParticle->GetParticleData()->_commonInfo->_maxParticles);

			//ImGui::Text("Texture"); ImGui::SameLine(offset_x); ImGui::InputFloat("Texture", &_commonInfo._startDelay[0], 0.01f, 1.0f, "%.3f");

			ImGui::Text("EmissiveCount");

			ImGui::Text("EmissiveTime");

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		{
			ImGui::Begin("Play_Stop");
			if (ImGui::Button("Play"))
				_myParticle->Play();
			ImGui::SameLine();
			if (ImGui::Button("Stop"))
				_myParticle->Stop();
			ImGui::End();
		}
		//// 3. Show another simple window.
		//if (show_another_window)
		//{
		//	ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		//	ImGui::Text("Hello from another window!");
		//	if (ImGui::Button("Close Me"))
		//		show_another_window = false;
		//	ImGui::End();
		//}



		//ImGuiWindowFlags::NoResize;

		ImGui::Begin("TextureBox", 0/*, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize
			^ ImGuiWindowFlags_::ImGuiWindowFlags_NoMove
			^ ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground*/);
		ImGui::SetWindowSize(ImVec2(300, 300));

		ImGui::BeginChild(ImGui::GetID("TextureBox"), ImVec2(200, 200));

		if (ImGui::Button("LoadTexture", ImVec2(50, 30)))
		{
			auto str = FileDialogs::OpenFile("이미지 파일 (*.png *.dds*)\0*.png;*.dds*\0");

			if (!str.empty())
			{
				TextureLoader::InsertTexture(str);
			}
		}
		for (auto& iter : TextureLoader::GetTextureFilePaths())
		{
			if (ImGui::ImageButton(TextureLoader::GetTexture(iter), ImVec2(100, 100)))
				_myParticle->GetParticleData()->_commonInfo->_refTextureID = TextureLoader::GetTexture(iter);

		}



		ImGui::EndChild();


		ImGui::End();

	};
	_renderingFunc = temp;
}
