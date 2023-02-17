#include "DebugBox.h"

#include "Commands.h"
#include "EffectEditorManager.h"
#include "../Common/Imgui/imgui_internal.h"
#include "ParticleObjectManager.h"
#include "ParticleRenderer.h"
#include "SkyBox.h"

DebugBox::DebugBox(std::shared_ptr<Muscle::GameObject> _gameObject) : ImGuiRnedererBase(_gameObject), _selectedParticle()
{
}

void DebugBox::SetRenderingFunc()
{
	auto temp = [&]()
	{
		_selectedParticle = EffectEditorManager::Get().GetSelectedParticle();

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

		ImGui::NewLine();

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		if (_selectedParticle)
			ImGui::Text("Now Particle Count : %d", _selectedParticle->GetParticleData()->_particleCount);


		if (_skyBox)
			ImGui::ColorEdit4("##SkyBoxColor", reinterpret_cast<float*>(&_skyBox->_renderingData->_materialInfo._color));

		ImGui::End();
	};

	_renderingFunc = temp;
}

void DebugBox::Start()
{
	SetRenderingFunc();

}

void DebugBox::Update()
{

}

void DebugBox::SetSkyBox(std::shared_ptr<Muscle::SkyBox>& skyBox)
{
	_skyBox = skyBox;
}
