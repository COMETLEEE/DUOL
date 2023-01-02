#include "Inspector.h"
#include "../Common/Imgui/imgui.h"
#include "ParticleRenderer.h"

constexpr int offset_x = 200;

void Inspector::ParticleSystemCommonInfo()
{
	ImGui::Checkbox("##ParticleSystemCommonInfo", &_selectedParticle->GetParticleData()->_commonInfo._useModule);

	ImGui::SameLine();

	if (ImGui::CollapsingHeader("ParticleSystemCommonInfo"))
	{
		ImGui::Text("Duration"); ImGui::SameLine(offset_x); ImGui::InputFloat("Duration", &_selectedParticle->GetParticleData()->_commonInfo._duration, 0.1f, 1.0f, "%.3f");

		ImGui::Text("Looping"); ImGui::SameLine(offset_x); ImGui::Checkbox("Looping", &_selectedParticle->GetParticleData()->_commonInfo._looping);      // Edit bools storing our window open/close state

		ImGui::Text("StartDelay"); ImGui::SameLine(offset_x); ImGui::InputFloat("StartDelay", &_selectedParticle->GetParticleData()->_commonInfo._startDelay[0], 0.1f, 1.0f, "%.3f");

		ParticleSystemCommonInfo_StartLifetime();

		ParticleSystemCommonInfo_StartSpeed();

		ParticleSystemCommonInfo_StartSize();

		ParticleSystemCommonInfo_StartRotation();

		ParticleSystemCommonInfo_StartColor();

		ParticleSystemCommonInfo_GravityModifier();

		ImGui::Text("MaxParticles"); ImGui::SameLine(offset_x);
		if (ImGui::InputInt("MaxParticles", &_selectedParticle->GetParticleData()->_commonInfo._maxParticles, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue))
			_selectedParticle->Play();

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
void Inspector::ParticleSystemCommonInfo_StartSize()
{
	ImGui::Text("StartSize"); ImGui::SameLine(120);

	if (ImGui::Button("Option##StartSize"))
		ImGui::OpenPopup("StartSize_popup");

	switch (_selectedParticle->GetParticleData()->_commonInfo._startSizeOption)
	{
	case MuscleGrapics::Particle_CommonInfo::Option_Particle::Constant:

		ImGui::SameLine(offset_x); ImGui::InputFloat2("StartSize", reinterpret_cast<float*>(&_selectedParticle->GetParticleData()->_commonInfo._startSize[0]));

		_selectedParticle->GetParticleData()->_commonInfo._startSize[1] = _selectedParticle->GetParticleData()->_commonInfo._startSize[0];

		break;

	case MuscleGrapics::Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant:

		ImGui::SameLine(offset_x); ImGui::InputFloat2("StartSize##1", reinterpret_cast<float*>(&_selectedParticle->GetParticleData()->_commonInfo._startSize[0]));

		ImGui::NewLine();

		ImGui::SameLine(offset_x); ImGui::InputFloat2("StartSize##2", reinterpret_cast<float*>(&_selectedParticle->GetParticleData()->_commonInfo._startSize[1]));

		break;

	default:

		break;

	}



	if (ImGui::BeginPopup("StartSize_popup"))
	{
		ImGui::Text("Option");

		ImGui::Separator();

		if (ImGui::Selectable("Constant"))
			_selectedParticle->GetParticleData()->_commonInfo._startSizeOption = MuscleGrapics::Particle_CommonInfo::Option_Particle::Constant;

		if (ImGui::Selectable("Random Between Two Constants"))
			_selectedParticle->GetParticleData()->_commonInfo._startSizeOption = MuscleGrapics::Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant;

		ImGui::EndPopup();
	}
}
void Inspector::ParticleSystemCommonInfo_StartSpeed()
{

	ImGui::Text("StartSpeed"); ImGui::SameLine(120);

	if (ImGui::Button("Option##StartSpeed"))
		ImGui::OpenPopup("StartSpeed_popup");


	switch (_selectedParticle->GetParticleData()->_commonInfo._startSpeedOption)
	{
	case MuscleGrapics::Particle_CommonInfo::Option_Particle::Constant:

		ImGui::SameLine(offset_x); ImGui::InputFloat("StartSpeed", &_selectedParticle->GetParticleData()->_commonInfo._startSpeed[0], 0.1f, 1.0f, "%.3f");

		_selectedParticle->GetParticleData()->_commonInfo._startSpeed[1] = _selectedParticle->GetParticleData()->_commonInfo._startSpeed[0];

		break;
	case MuscleGrapics::Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant:

		ImGui::SameLine(offset_x); ImGui::InputFloat("StartSpeed##1", &_selectedParticle->GetParticleData()->_commonInfo._startSpeed[0], 0.1f, 1.0f, "%.3f");

		ImGui::NewLine();

		ImGui::SameLine(offset_x); ImGui::InputFloat("StartSpeed##2", &_selectedParticle->GetParticleData()->_commonInfo._startSpeed[1], 0.1f, 1.0f, "%.3f");

		break;
	default:

		break;
	}



	if (ImGui::BeginPopup("StartSpeed_popup"))
	{
		ImGui::Text("Option");

		ImGui::Separator();

		if (ImGui::Selectable("Constant"))
			_selectedParticle->GetParticleData()->_commonInfo._startSpeedOption = MuscleGrapics::Particle_CommonInfo::Option_Particle::Constant;

		if (ImGui::Selectable("Random Between Two Constants"))
			_selectedParticle->GetParticleData()->_commonInfo._startSpeedOption = MuscleGrapics::Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant;

		ImGui::EndPopup();
	}

}
void Inspector::ParticleSystemCommonInfo_StartLifetime()
{
	ImGui::Text("StartLifeTime"); ImGui::SameLine(120);

	if (ImGui::Button("Option##StartLifeTime"))
		ImGui::OpenPopup("StartLifeTime_popup");


	switch (_selectedParticle->GetParticleData()->_commonInfo._startLifeTimeOption)
	{
	case MuscleGrapics::Particle_CommonInfo::Option_Particle::Constant:

		ImGui::SameLine(offset_x); ImGui::InputFloat("StartLifeTime", &_selectedParticle->GetParticleData()->_commonInfo._startLifeTime[0], 0.01f, 1.0f, "%.3f");

		_selectedParticle->GetParticleData()->_commonInfo._startLifeTime[1] = _selectedParticle->GetParticleData()->_commonInfo._startLifeTime[0];

		break;
	case MuscleGrapics::Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant:

		ImGui::SameLine(offset_x); ImGui::InputFloat("StartLifeTime##1", &_selectedParticle->GetParticleData()->_commonInfo._startLifeTime[0], 0.1f, 1.0f, "%.3f");

		ImGui::NewLine();

		ImGui::SameLine(offset_x); ImGui::InputFloat("StartLifeTime##2", &_selectedParticle->GetParticleData()->_commonInfo._startLifeTime[1], 0.1f, 1.0f, "%.3f");

		break;
	default:

		break;
	}



	if (ImGui::BeginPopup("StartLifeTime_popup"))
	{
		ImGui::Text("Option");

		ImGui::Separator();

		if (ImGui::Selectable("Constant"))
			_selectedParticle->GetParticleData()->_commonInfo._startLifeTimeOption = MuscleGrapics::Particle_CommonInfo::Option_Particle::Constant;

		if (ImGui::Selectable("Random Between Two Constants"))
			_selectedParticle->GetParticleData()->_commonInfo._startLifeTimeOption = MuscleGrapics::Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant;

		ImGui::EndPopup();
	}
}
void Inspector::ParticleSystemCommonInfo_StartRotation()
{

	ImGui::Text("StartRotation"); ImGui::SameLine(120);

	if (ImGui::Button("Option##StartRotation"))
		ImGui::OpenPopup("StartRotation_popup");


	switch (_selectedParticle->GetParticleData()->_commonInfo._startRotationOption)
	{
	case MuscleGrapics::Particle_CommonInfo::Option_Particle::Constant:

		ImGui::SameLine(offset_x); ImGui::InputFloat("StartRotation", &_selectedParticle->GetParticleData()->_commonInfo._startRotation[0], 0.03f, 1.0f, "%.3f");

		_selectedParticle->GetParticleData()->_commonInfo._startRotation[1] = _selectedParticle->GetParticleData()->_commonInfo._startRotation[0];

		break;
	case MuscleGrapics::Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant:

		ImGui::SameLine(offset_x); ImGui::InputFloat("StartRotation##1", &_selectedParticle->GetParticleData()->_commonInfo._startRotation[0], 0.03f, 1.0f, "%.3f");

		ImGui::NewLine();

		ImGui::SameLine(offset_x); ImGui::InputFloat("StartRotation##2", &_selectedParticle->GetParticleData()->_commonInfo._startRotation[1], 0.03f, 1.0f, "%.3f");

		break;
	default:

		break;
	}



	if (ImGui::BeginPopup("StartRotation_popup"))
	{
		ImGui::Text("Option");

		ImGui::Separator();

		if (ImGui::Selectable("Constant"))
			_selectedParticle->GetParticleData()->_commonInfo._startRotationOption = MuscleGrapics::Particle_CommonInfo::Option_Particle::Constant;

		if (ImGui::Selectable("Random Between Two Constants"))
			_selectedParticle->GetParticleData()->_commonInfo._startRotationOption = MuscleGrapics::Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant;

		ImGui::EndPopup();
	}
}
void Inspector::ParticleSystemCommonInfo_StartColor()
{

	ImGui::Text("StartColor"); ImGui::SameLine(120);

	if (ImGui::Button("Option##StartStartColor"))
		ImGui::OpenPopup("StartColor_popup");


	switch (_selectedParticle->GetParticleData()->_commonInfo._startColorOption)
	{
	case MuscleGrapics::Particle_CommonInfo::Option_Particle::Constant:

		ImGui::SameLine(offset_x); ImGui::ColorEdit4("StartColor", reinterpret_cast<float*>(&_selectedParticle->GetParticleData()->_commonInfo._startColor[0])); // Edit 3 floats representing a color

		_selectedParticle->GetParticleData()->_commonInfo._startColor[1] = _selectedParticle->GetParticleData()->_commonInfo._startColor[0];

		break;
	case MuscleGrapics::Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant:

		ImGui::SameLine(offset_x); ImGui::ColorEdit4("StartColor##1", reinterpret_cast<float*>(&_selectedParticle->GetParticleData()->_commonInfo._startColor[0])); // Edit 3 floats representing a color

		ImGui::NewLine();

		ImGui::SameLine(offset_x); ImGui::ColorEdit4("StartColor##2", reinterpret_cast<float*>(&_selectedParticle->GetParticleData()->_commonInfo._startColor[1])); // Edit 3 floats representing a color

		break;
	default:

		break;
	}



	if (ImGui::BeginPopup("StartColor_popup"))
	{
		ImGui::Text("Option");

		ImGui::Separator();

		if (ImGui::Selectable("Constant"))
			_selectedParticle->GetParticleData()->_commonInfo._startColorOption = MuscleGrapics::Particle_CommonInfo::Option_Particle::Constant;

		if (ImGui::Selectable("Random Between Two Constants"))
			_selectedParticle->GetParticleData()->_commonInfo._startColorOption = MuscleGrapics::Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant;

		ImGui::EndPopup();
	}
}
void Inspector::ParticleSystemCommonInfo_GravityModifier()
{

	ImGui::Text("GravityModifier"); ImGui::SameLine(120);

	if (ImGui::Button("Option##GravityModifier"))
		ImGui::OpenPopup("GravityModifier_popup");


	switch (_selectedParticle->GetParticleData()->_commonInfo._gravirtModifierOption)
	{
	case MuscleGrapics::Particle_CommonInfo::Option_Particle::Constant:

		ImGui::SameLine(offset_x); ImGui::InputFloat("GravityModifier", &_selectedParticle->GetParticleData()->_commonInfo._gravityModifier[0], 0.1f, 1.0f, "%.3f");

		_selectedParticle->GetParticleData()->_commonInfo._gravityModifier[1] = _selectedParticle->GetParticleData()->_commonInfo._gravityModifier[0];

		break;
	case MuscleGrapics::Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant:

		ImGui::SameLine(offset_x); ImGui::InputFloat("GravityModifier##1", &_selectedParticle->GetParticleData()->_commonInfo._gravityModifier[0], 0.1f, 1.0f, "%.3f");

		ImGui::NewLine();

		ImGui::SameLine(offset_x); ImGui::InputFloat("GravityModifier##2", &_selectedParticle->GetParticleData()->_commonInfo._gravityModifier[1], 0.1f, 1.0f, "%.3f");

		break;
	default:

		break;
	}


	if (ImGui::BeginPopup("GravityModifier_popup"))
	{
		ImGui::Text("Option");

		ImGui::Separator();

		if (ImGui::Selectable("Constant"))
			_selectedParticle->GetParticleData()->_commonInfo._gravirtModifierOption = MuscleGrapics::Particle_CommonInfo::Option_Particle::Constant;

		if (ImGui::Selectable("Random Between Two Constants"))
			_selectedParticle->GetParticleData()->_commonInfo._gravirtModifierOption = MuscleGrapics::Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant;

		ImGui::EndPopup();
	}
}
void Inspector::Emission()
{
	ImGui::Checkbox("##Emission", &_selectedParticle->GetParticleData()->_emission._useModule);

	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Emission"))
	{
		ImGui::Text("EmissiveCount"); ImGui::SameLine(offset_x);

		if (ImGui::InputInt("EmissiveCount", &_selectedParticle->GetParticleData()->_emission._emissiveCount, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue))
			_selectedParticle->Play();

		ImGui::Text("EmissiveTime"); ImGui::SameLine(offset_x); ImGui::InputFloat("EmissiveTime", &_selectedParticle->GetParticleData()->_emission._emissiveTime, 0.1f, 1.0f, "%.3f");

	}
}
void Inspector::Shape()
{
	ImGui::Checkbox("##Shape", &_selectedParticle->GetParticleData()->_shape._useModule);

	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Shape"))
	{

	}
}
void Inspector::Velocity_Over_Lifetime()
{
	ImGui::Checkbox("##Velocity_Over_Lifetime", &_selectedParticle->GetParticleData()->_velocity_Over_Lifetime._useModule);

	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Velocity Over Lifetime"))
	{
		ImGui::Text("LinearVelocity"); ImGui::SameLine(offset_x); ImGui::InputFloat3("LinearVelocity", reinterpret_cast<float*>(&_selectedParticle->GetParticleData()->_velocity_Over_Lifetime._linearVelocity));
	}
}
void Inspector::Force_over_Lifetime()
{
	ImGui::Checkbox("##Force_over_Lifetime", &_selectedParticle->GetParticleData()->_force_Over_Lifetime._useModule);

	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Force over Lifetime"))
	{
	}
}
void Inspector::Color_over_Lifetime()
{
	ImGui::Checkbox("##Color_over_Lifetime", &_selectedParticle->GetParticleData()->_color_Over_Lifetime._useModule);

	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Color over Lifetime"))
	{
		//ImGui::Text("Lifetime_Start_Color"); ImGui::SameLine(offset_x); ImGui::ColorEdit4("Lifetime_Start_Color", reinterpret_cast<float*>(&_selectedParticle->GetParticleData()->_color_Over_Lifetime._startColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); // Edit 3 floats representing a color
		//ImGui::SameLine(); ImGui::Text("Lifetime_End_Color"); ImGui::SameLine(200 + offset_x); ImGui::ColorEdit4("Lifetime_End_Color", reinterpret_cast<float*>(&_selectedParticle->GetParticleData()->_color_Over_Lifetime._endColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel); // Edit 3 floats representing a color
	}
}
void Inspector::Size_over_Lifetime()
{
	ImGui::Checkbox("##Size_over_Lifetime", &_selectedParticle->GetParticleData()->_size_Over_Lifetime._useModule);

	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Size over Lifetime"))
	{

		static float start = 0;

		static float end = 1;

		static float startOffset = 0;

		static float endOffset = 0;


		ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);

		ImGui::SliderFloat("StartSize", &_selectedParticle->GetParticleData()->_size_Over_Lifetime._startSize, 0, 1);

		ImGui::SameLine(ImGui::GetFontSize() * 16);

		ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);

		ImGui::SliderFloat("EndSize", &_selectedParticle->GetParticleData()->_size_Over_Lifetime._endSize, 0, 1);

		ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);

		ImGui::SliderFloat("StartOffset", &_selectedParticle->GetParticleData()->_size_Over_Lifetime._startOffset, 0, 1);

		ImGui::SameLine(ImGui::GetFontSize() * 16);

		ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);

		ImGui::SliderFloat("EndOffset", &_selectedParticle->GetParticleData()->_size_Over_Lifetime._endOffset, 0, 1);

		start = _selectedParticle->GetParticleData()->_size_Over_Lifetime._startSize;

		end = _selectedParticle->GetParticleData()->_size_Over_Lifetime._endSize;

		startOffset = _selectedParticle->GetParticleData()->_size_Over_Lifetime._startOffset;

		endOffset = _selectedParticle->GetParticleData()->_size_Over_Lifetime._endOffset;

		auto UpFunc = [](void*, int t)->float
		{
			const float result = (100 - t) * (start - startOffset) + t * (end + endOffset);

			if (result < 0)
				return 	0;
			if (100.0f < result)
				return 	100.0f;

			return result;
		};


		ImGui::PlotLines("Lines", UpFunc, NULL, 100, 0, NULL, 0.0f, 100.0f, ImVec2(0, 80));
	}
}
void Inspector::Rotation_over_Lifetime()
{
	ImGui::Checkbox("##Rotation_over_Lifetime", &_selectedParticle->GetParticleData()->_rotation_Over_Lifetime._useModule);

	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Rotation over Lifetime"))
	{
		ImGui::Text("AngularVelocity"); ImGui::SameLine(offset_x); ImGui::InputFloat("AngularVelocity", &_selectedParticle->GetParticleData()->_rotation_Over_Lifetime._AngularVelocity, 0.03f, 0.1f, "%.3f");
	}
}
void Inspector::Noise()
{
	ImGui::Checkbox("##Noise", &_selectedParticle->GetParticleData()->_noise._useModule);

	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Noise"))
	{
	}
}
void Inspector::Texture_Sheet_Animation()
{
	ImGui::Checkbox("##Texture_Sheet_Animation", &_selectedParticle->GetParticleData()->_texture_Sheet_Animaition._useModule);

	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Texture Sheet Animation"))
	{

		auto& grid_xy = _selectedParticle->GetParticleData()->_texture_Sheet_Animaition._grid_XY;

		static int grid_size = 0; // = grid_xy[0] * grid_xy[1];

		grid_size = grid_xy[0] * grid_xy[1];

		ImGui::Text("Grid_XY"); ImGui::SameLine(offset_x); ImGui::InputInt2("Grid_XY", grid_xy);

		if (grid_xy[0] < 1 || grid_xy[1] < 1)
		{
			grid_xy[0] = 1;
			grid_xy[1] = 1;
		}

		ImGui::Text("Mode"); ImGui::SameLine(offset_x); ImGui::Combo("test", &_selectedParticle->GetParticleData()->_texture_Sheet_Animaition._timeMode, { "None\0LifeTime" });
	}

}
void Inspector::Trails()
{
	ImGui::Checkbox("##Trails", &_selectedParticle->GetParticleData()->_trails._useModule);

	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Trails"))
	{
	}
}
void Inspector::Renderer()
{
	ImGui::Checkbox("##Renderer", &_selectedParticle->GetParticleData()->_renderer._useModule);

	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Renderer"))
	{
	}
}