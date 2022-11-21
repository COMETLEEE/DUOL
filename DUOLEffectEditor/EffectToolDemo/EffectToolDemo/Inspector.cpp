#include "pch.h"
#include "Inspector.h"

#include "FileDialogs.h"

constexpr int offset_x = 200;

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

void Inspector::ParticleSystemCommonInfo()
{
	if (ImGui::CollapsingHeader("ParticleSystemCommonInfo"))
	{
		// Create a window called "Hello, world!" and append into it.

		ImGui::Text("Duration"); ImGui::SameLine(offset_x); ImGui::InputFloat("Duration", &_myParticle->GetParticleData()->_commonInfo->_duration, 0.1f, 1.0f, "%.3f");

		ImGui::Text("Looping"); ImGui::SameLine(offset_x); ImGui::Checkbox("Looping", &_myParticle->GetParticleData()->_commonInfo->_looping);      // Edit bools storing our window open/close state

		ImGui::Text("StartDelay"); ImGui::SameLine(offset_x); ImGui::InputFloat("StartDelay", &_myParticle->GetParticleData()->_commonInfo->_startDelay[0], 0.1f, 1.0f, "%.3f");

		ImGui::Text("StartLifeTime"); ImGui::SameLine(offset_x); ImGui::InputFloat("StartLifeTime", &_myParticle->GetParticleData()->_commonInfo->_startLifeTime[0], 0.01f, 1.0f, "%.3f");

		ImGui::Text("StartSpeed"); ImGui::SameLine(offset_x); ImGui::InputFloat("StartSpeed", &_myParticle->GetParticleData()->_commonInfo->_startSpeed[0], 0.1f, 1.0f, "%.3f");

		ImGui::Text("StartSize"); ImGui::SameLine(offset_x); ImGui::InputFloat2("StartSize", reinterpret_cast<float*>(&_myParticle->GetParticleData()->_commonInfo->_startSize[0]));

		ImGui::Text("StartRotation"); ImGui::SameLine(offset_x); ImGui::InputFloat("StartRotation", &_myParticle->GetParticleData()->_commonInfo->_startRotation[0], 0.03f, 1.0f, "%.3f");

		ImGui::Text("StartColor"); ImGui::SameLine(offset_x); ImGui::ColorEdit4("StartColor", reinterpret_cast<float*>(&_myParticle->GetParticleData()->_commonInfo->_startColor[0])); // Edit 3 floats representing a color

		ImGui::Text("GravityModifier"); ImGui::SameLine(offset_x); ImGui::InputFloat("GravityModifier", &_myParticle->GetParticleData()->_commonInfo->_gravityModifier[0], 0.1f, 1.0f, "%.3f");

		ImGui::Text("MaxParticles"); ImGui::SameLine(offset_x); ImGui::InputInt("MaxParticles", &_myParticle->GetParticleData()->_commonInfo->_maxParticles);

		//ImGui::Text("Texture"); ImGui::SameLine(offset_x); ImGui::InputFloat("Texture", &_commonInfo._startDelay[0], 0.01f, 1.0f, "%.3f");

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);


	}
}

void Inspector::Emission()
{
	if (ImGui::CollapsingHeader("Emission"))
	{
		ImGui::Text("EmissiveCount"); ImGui::SameLine(offset_x); ImGui::InputInt("EmissiveCount", &_myParticle->GetParticleData()->_emission->_emissiveCount);

		ImGui::Text("EmissiveTime"); ImGui::SameLine(offset_x); ImGui::InputFloat("EmissiveTime", &_myParticle->GetParticleData()->_emission->_emissiveTime, 0.1f, 1.0f, "%.3f");

	}
}

void Inspector::Shape()
{
	if (ImGui::CollapsingHeader("Shape"))
	{

	}
}

void Inspector::Velocity_Over_Lifetime()
{
	if (ImGui::CollapsingHeader("Velocity Over Lifetime"))
	{
		ImGui::Text("LinearVelocity"); ImGui::SameLine(offset_x); ImGui::InputFloat3("LinearVelocity", reinterpret_cast<float*>(&_myParticle->GetParticleData()->_velocity_Over_Lifetime->_linearVelocity));
	}
}

void Inspector::Limit_Velocity_over_Lifetime()
{
	if (ImGui::CollapsingHeader("Limit Velocity over Lifetime"))
	{
	}
}

void Inspector::Force_over_Lifetime()
{
	if (ImGui::CollapsingHeader("Force over Lifetime"))
	{
	}
}

void Inspector::Color_over_Lifetime()
{
	if (ImGui::CollapsingHeader("Color over Lifetime"))
	{
		ImGui::Text("Lifetime_Start_Color"); ImGui::SameLine(offset_x); ImGui::ColorEdit4("Lifetime_Start_Color", reinterpret_cast<float*>(&_myParticle->GetParticleData()->_color_Over_Lifetime->_startColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); // Edit 3 floats representing a color
		ImGui::SameLine(); ImGui::Text("Lifetime_End_Color"); ImGui::SameLine(200 + offset_x); ImGui::ColorEdit4("Lifetime_End_Color", reinterpret_cast<float*>(&_myParticle->GetParticleData()->_color_Over_Lifetime->_endColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); // Edit 3 floats representing a color
	}
}

void Inspector::Color_by_Speed()
{
	if (ImGui::CollapsingHeader("Color by Speed"))
	{
	}
}

void Inspector::Size_over_Lifetime()
{
	if (ImGui::CollapsingHeader("Size over Lifetime"))
	{
	}
}

void Inspector::Size_by_Speed()
{
	if (ImGui::CollapsingHeader("Size by Speed"))
	{
	}
}

void Inspector::Rotation_over_Lifetime()
{
	if (ImGui::CollapsingHeader("Rotation over Lifetime"))
	{
	}
}

void Inspector::Rotation_by_Speed()
{
	if (ImGui::CollapsingHeader("Rotation by Speed"))
	{
	}
}

void Inspector::External_Forces()
{
	if (ImGui::CollapsingHeader("External Forces"))
	{
	}
}

void Inspector::Noise()
{
	if (ImGui::CollapsingHeader("Noise"))
	{
	}
}

void Inspector::Collision()
{
	if (ImGui::CollapsingHeader("Collision"))
	{
	}
}

void Inspector::Triggers()
{
	if (ImGui::CollapsingHeader("Triggers"))
	{
	}
}

void Inspector::SubEmitters()
{
	if (ImGui::CollapsingHeader("SubEmitters"))
	{
	}
}

void Inspector::Texture_Sheet_Animation()
{
	if (ImGui::CollapsingHeader("Texture Sheet Animation"))
	{
	}
}

void Inspector::Lights()
{
	if (ImGui::CollapsingHeader("Lights"))
	{
	}
}

void Inspector::Trails()
{
	if (ImGui::CollapsingHeader("Trails"))
	{
	}
}

void Inspector::Custom_Data()
{
	if (ImGui::CollapsingHeader("Custom Data"))
	{
	}
}

void Inspector::Renderer()
{
	if (ImGui::CollapsingHeader("Renderer"))
	{
	}
}

void Inspector::SetRenderingFunc()
{
	auto temp = [&]()
	{

		// Inspector
		ImGui::Begin("ParticleSystem");
		// �� �κ��� ���÷������� �ϸ� �� ���ܰ� ����...
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
		// �� �κ��� ���÷������� �ϸ� �� ���ܰ� ����...
		// ���߿� ������ ����� �ν�Ʈ ���̺귯���� ���÷��� ����غ���.
		ImGui::End();




		{
			ImGui::Begin("Play_Stop");
			if (ImGui::Button("Play"))
				_myParticle->Play();
			ImGui::SameLine();
			if (ImGui::Button("Stop"))
				_myParticle->Stop();
			ImGui::End();
		}

		ImGui::Begin("TextureBox", 0/*, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize
			^ ImGuiWindowFlags_::ImGuiWindowFlags_NoMove
			^ ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground*/);
		ImGui::SetWindowSize(ImVec2(300, 300));

		ImGui::BeginChild(ImGui::GetID("TextureBox"), ImVec2(200, 200));

		if (ImGui::Button("LoadTexture", ImVec2(50, 30)))
		{
			auto str = FileDialogs::OpenFile("�̹��� ���� (*.png *.dds*)\0*.png;*.dds*\0");

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
