#include "pch.h"
#include "Inspector.h"

#include "GraphicsManager.h"

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

		static bool show_demo_window = false;
		static bool show_another_window = false;
		static float size[2] = { 1.0f,1.0f };
		static float f = 4.0f;
		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		{

			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("Speed", &f, 0.0f, 100.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::SliderFloat("X", &size[0], 0.0f, 100.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::SliderFloat("Y", &size[1], 0.0f, 100.0f);            // Edit 1 float using a slider from 0.0f to 1.0f


			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}



		//ImGuiWindowFlags::NoResize;
		ImGui::Begin("test");
		ImGui::Begin("test2", 0/*, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize
			^ ImGuiWindowFlags_::ImGuiWindowFlags_NoMove
			^ ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground*/);
		ImGui::SetWindowSize(ImVec2(300, 300));

		ImGui::BeginChild(ImGui::GetID("test2"), ImVec2(100, 100));
		ImGui::Image(Muscle::IGameEngine::Get()->GetGraphicsManager()->GetTexture(TEXT("ParticleNone")), ImVec2(200, 200));
		ImGui::SameLine();
		ImGui::Image(Muscle::IGameEngine::Get()->GetGraphicsManager()->GetTexture(TEXT("ParticleNone")), ImVec2(200, 200));
		ImGui::Image(Muscle::IGameEngine::Get()->GetGraphicsManager()->GetTexture(TEXT("ParticleNone")), ImVec2(200, 200));
		ImGui::EndChild();
		ImGui::End();
		ImGui::End();


		_myParticle->GetParticleData()->_commonInfo->_startSpeed = f;
		_myParticle->GetParticleData()->_commonInfo->_startSize.x = size[0];
		_myParticle->GetParticleData()->_commonInfo->_startSize.y = size[1];

	};
	_renderingFunc = temp;
}
