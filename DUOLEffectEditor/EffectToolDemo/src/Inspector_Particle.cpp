#include "EffectEditorManager.h"
#include "FileDialogs.h"
#include "Inspector.h"
#include "KeyBoard.h"
#include "../Common/Imgui/imgui.h"
#include "ParticleRenderer.h"
#include "TextureLoader.h"
#include "ColorGradientBar.h"
#include "CTime.h"

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

		ImGui::Text("SimulationSpeed"); ImGui::SameLine(offset_x); ImGui::DragFloat("SimulationSpeed", &_selectedParticle->GetParticleData()->_commonInfo._simulationSpeed, 0.1f, 0.01f);

		ImGui::Text("MaxParticles"); ImGui::SameLine(offset_x);
		if (ImGui::InputInt("MaxParticles", &_selectedParticle->GetParticleData()->_commonInfo._maxParticles, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue))
			_selectedParticle->Play();

		const char* items[] = { "Local", "World" };
		ImGui::Text("Space"); ImGui::SameLine(offset_x); ImGui::Combo(" ##CommonInfoSpace", reinterpret_cast<int*>(&_selectedParticle->GetParticleData()->_commonInfo._space), items, IM_ARRAYSIZE(items));

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

		ImGui::SameLine(offset_x); ImGui::DragFloat2("StartSize", reinterpret_cast<float*>(&_selectedParticle->GetParticleData()->_commonInfo._startSize[0]), 0.1f);

		_selectedParticle->GetParticleData()->_commonInfo._startSize[1] = _selectedParticle->GetParticleData()->_commonInfo._startSize[0];

		break;

	case MuscleGrapics::Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant:

		ImGui::SameLine(offset_x); ImGui::DragFloat2("StartSize##1", reinterpret_cast<float*>(&_selectedParticle->GetParticleData()->_commonInfo._startSize[0]), 0.1f);

		ImGui::NewLine();

		ImGui::SameLine(offset_x); ImGui::DragFloat2("StartSize##2", reinterpret_cast<float*>(&_selectedParticle->GetParticleData()->_commonInfo._startSize[1]), 0.1f);

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

		ImGui::SameLine(offset_x); ImGui::DragFloat("StartSpeed", &_selectedParticle->GetParticleData()->_commonInfo._startSpeed[0], 0.1f);

		_selectedParticle->GetParticleData()->_commonInfo._startSpeed[1] = _selectedParticle->GetParticleData()->_commonInfo._startSpeed[0];

		break;
	case MuscleGrapics::Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant:

		ImGui::SameLine(offset_x); ImGui::DragFloat("StartSpeed##1", &_selectedParticle->GetParticleData()->_commonInfo._startSpeed[0], 0.1f);

		ImGui::NewLine();

		ImGui::SameLine(offset_x); ImGui::DragFloat("StartSpeed##2", &_selectedParticle->GetParticleData()->_commonInfo._startSpeed[1], 0.1f);

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
	auto& shape = _selectedParticle->GetParticleData()->_shape;

	ImGui::Checkbox("##Shape", &shape._useModule);

	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Shape"))
	{

		ImGui::Text("Shape"); ImGui::SameLine(offset_x);

		const char* items[] = { "Sphere", "Hemisphere", "Cone", "Donut", "Box", "Circle","Rectangle" };
		ImGui::Combo("combo", reinterpret_cast<int*>(&shape._shape), items, IM_ARRAYSIZE(items));

		float degreeAngle = DUOLMath::XMConvertToDegrees(shape._angle);
		float degreeArc = DUOLMath::XMConvertToDegrees(shape._arc);

		switch (_selectedParticle->GetParticleData()->_shape._shape)
		{
		case MuscleGrapics::Particle_Shape::Shape::Box:

			break;
		case MuscleGrapics::Particle_Shape::Shape::Rectangle:

			break;
		case MuscleGrapics::Particle_Shape::Shape::Sphere:

			ImGui::Text("Radius"); ImGui::SameLine(offset_x); ImGui::DragFloat("Radius##Shape", &shape._radius, 0.1f, 0.001f);

			ImGui::Text("Arc"); ImGui::SameLine(offset_x); ImGui::DragFloat("Arc##Shape", &degreeArc, 0.1f, 0, 360.0f);

			break;
		case MuscleGrapics::Particle_Shape::Shape::Hemisphere:

			ImGui::Text("Radius"); ImGui::SameLine(offset_x); ImGui::DragFloat("Radius##Shape", &shape._radius, 0.1f, 0.001f);

			ImGui::Text("Arc"); ImGui::SameLine(offset_x); ImGui::DragFloat("Arc##Shape", &degreeArc, 0.1f, 0, 360.0f);

			break;
		case MuscleGrapics::Particle_Shape::Shape::Circle:

			ImGui::Text("Radius"); ImGui::SameLine(offset_x); ImGui::DragFloat("Radius##Shape", &shape._radius, 0.1f, 0.001f);

			ImGui::Text("Arc"); ImGui::SameLine(offset_x); ImGui::DragFloat("Arc##Shape", &degreeArc, 0.1f, 0, 360.0f);

			break;
		case MuscleGrapics::Particle_Shape::Shape::Cone:

			ImGui::Text("Angle"); ImGui::SameLine(offset_x); ImGui::DragFloat("Angle##Shape", &degreeAngle, 0.1f, 0.001f);

			ImGui::Text("Radius"); ImGui::SameLine(offset_x); ImGui::DragFloat("Radius##Shape", &shape._radius, 0.1f, 0.001f);

			ImGui::Text("Arc"); ImGui::SameLine(offset_x); ImGui::DragFloat("Arc##Shape", &degreeArc, 0.1f, 0, 360.0f);

			break;
		case MuscleGrapics::Particle_Shape::Shape::Donut:

			ImGui::Text("Radius"); ImGui::SameLine(offset_x); ImGui::DragFloat("Radius##Shape", &shape._radius, 0.1f, 0.001f);

			ImGui::Text("DonutRadius"); ImGui::SameLine(offset_x); ImGui::DragFloat("DonutRadius##Shape", &shape._donutRadius, 0.1f, 0.001f);

			ImGui::Text("Arc"); ImGui::SameLine(offset_x); ImGui::DragFloat("Arc##Shape", &degreeArc, 0.1f, 0, 360.0f);

			break;

		case MuscleGrapics::Particle_Shape::Shape::Edge:

			ImGui::Text("Radius"); ImGui::SameLine(offset_x); ImGui::DragFloat("DonutRadius##Shape", &shape._donutRadius, 0.1f, 0.001f);

			break;
		default:
			break;
		}

		shape._angle = DUOLMath::XMConvertToRadians(degreeAngle);
		shape._arc = DUOLMath::XMConvertToRadians(degreeArc);

		ImGui::NewLine();

		ImGui::Text("Pos"); ImGui::SameLine(offset_x); ImGui::DragFloat3("Pos##Shape", reinterpret_cast<float*>(&shape._position), 0.01f);

		ImGui::Text("Scale"); ImGui::SameLine(offset_x);  ImGui::DragFloat3("Scale##Shape", reinterpret_cast<float*>(&shape._scale), 0.01f);

		ImGui::Text("Rot"); ImGui::SameLine(offset_x);  ImGui::DragFloat3("Rotate##Shape", reinterpret_cast<float*>(&shape._rotation), 0.01f);

	}
}
void Inspector::Velocity_Over_Lifetime()
{
	ImGui::Checkbox("##Velocity_Over_Lifetime", &_selectedParticle->GetParticleData()->_velocity_Over_Lifetime._useModule);

	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Velocity Over Lifetime"))
	{
		const char* items[] = { "Local", "World" };
		ImGui::Text("Space"); ImGui::SameLine(offset_x); ImGui::Combo(" ##VelocitySpace", reinterpret_cast<int*>(&_selectedParticle->GetParticleData()->_velocity_Over_Lifetime._space), items, IM_ARRAYSIZE(items));

		ImGui::Text("LinearVelocity"); ImGui::SameLine(offset_x); ImGui::DragFloat3("LinearVelocity", reinterpret_cast<float*>(&_selectedParticle->GetParticleData()->_velocity_Over_Lifetime._linearVelocity), 0.1f);

		ImGui::Text("Orbital"); ImGui::SameLine(offset_x); ImGui::DragFloat3("Orbital", reinterpret_cast<float*>(&_selectedParticle->GetParticleData()->_velocity_Over_Lifetime._orbital), 0.01f);

		ImGui::Text("Offset"); ImGui::SameLine(offset_x); ImGui::DragFloat3("Offset", reinterpret_cast<float*>(&_selectedParticle->GetParticleData()->_velocity_Over_Lifetime._offset), 0.01f);

		ImGui::Text("ConvertTime"); ImGui::SameLine(offset_x); ImGui::DragFloat("ConvertTime", &_selectedParticle->GetParticleData()->_velocity_Over_Lifetime._convertTime, 0.01f);
	}
}
void Inspector::Force_over_Lifetime()
{
	ImGui::Checkbox("##Force_over_Lifetime", &_selectedParticle->GetParticleData()->_force_Over_Lifetime._useModule);

	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Force over Lifetime"))
	{
		const char* items[] = { "Local", "World" };
		ImGui::Text("Space"); ImGui::SameLine(offset_x); ImGui::Combo(" ##ForceSpace", reinterpret_cast<int*>(&_selectedParticle->GetParticleData()->_force_Over_Lifetime._space), items, IM_ARRAYSIZE(items));

		ImGui::Text("Force"); ImGui::SameLine(offset_x); ImGui::DragFloat3(" ##Force", reinterpret_cast<float*>(&_selectedParticle->GetParticleData()->_force_Over_Lifetime._force), 0.1f);
	}
}
void Inspector::Color_over_Lifetime()
{
	ImGui::Checkbox("##Color_over_Lifetime", &_selectedParticle->GetParticleData()->_color_Over_Lifetime._useModule);

	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Color over Lifetime"))
	{
		static ColorGradientBar colorGradientBar;

		static ColorGradientBar alphaGradientBar;

		ImGui::Text("Color##GradientBar");

		ImVec2 pos = ImGui::GetCursorScreenPos();

		colorGradientBar.Draw(pos, 50, 300, _selectedParticle->GetParticleData()->_color_Over_Lifetime._color_Ratio, 8);

		ImGui::Text("Alpha##GradientBar");

		pos = ImGui::GetCursorScreenPos();

		alphaGradientBar.DrawAlpha(pos, 50, 300, _selectedParticle->GetParticleData()->_color_Over_Lifetime._alpha_Ratio, 8);

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
	auto& noise = _selectedParticle->GetParticleData()->_noise;
	ImGui::Checkbox("##Noise", &noise._useModule);

	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Noise"))
	{

		auto prevPos = ImGui::GetCursorPos();

		ImGui::Text("Strength"); ImGui::SameLine(offset_x); ImGui::SetNextItemWidth(offset_x); ImGui::DragFloat(" ##Strength", &noise._strength, 0.1f, 0.001f);

		ImGui::Text("Frequency"); ImGui::SameLine(offset_x); ImGui::SetNextItemWidth(offset_x); ImGui::DragFloat(" ##Frequency", &noise._frequency, 0.1f, 0.001f);

		ImGui::Text("ScrollSpeed"); ImGui::SameLine(offset_x); ImGui::SetNextItemWidth(offset_x); ImGui::DragFloat(" ##ScrollSpeed", &noise._scrollSpeed, 0.1f, 0.001f);

		ImGui::Text("Damping"); ImGui::SameLine(offset_x); ImGui::SetNextItemWidth(offset_x); ImGui::Checkbox(" ##Damping", &noise._damping);

		ImGui::Text("Octaves"); ImGui::SameLine(offset_x); ImGui::SetNextItemWidth(offset_x); ImGui::DragInt(" ##Octaves", &noise._octaves, 1, 1, 4);

		ImGui::Text("OctaaveMutiplier"); ImGui::SameLine(offset_x); ImGui::SetNextItemWidth(offset_x); ImGui::DragFloat(" ##OctaaveMutiplier", &noise._octaveMultiplier, 0.1f, 0.001f);

		ImGui::Text("OctaveScale"); ImGui::SameLine(offset_x); ImGui::SetNextItemWidth(offset_x); ImGui::DragFloat(" ##OctaveScale", &noise._octaveScale, 0.1f, 0.001f);

		ImGui::Text("PositionAmount"); ImGui::SameLine(offset_x); ImGui::SetNextItemWidth(offset_x); ImGui::DragFloat(" ##PositionAmount", &noise._positionAmount, 1.0f, 0.0f);

		ImGui::Text("RotationAmount"); ImGui::SameLine(offset_x); ImGui::SetNextItemWidth(offset_x); ImGui::DragFloat(" ##RotationAmount", &noise._rotationAmount, 1.0f, 0.0f);

		ImGui::Text("SizeAmount"); ImGui::SameLine(offset_x); ImGui::SetNextItemWidth(offset_x); ImGui::DragFloat(" ##SizeAmount", &noise._sizeAmount, 1.0f, 0.0f);

		auto pos = ImGui::GetCursorPos();

		ImGui::SetCursorPos(ImVec2(420, prevPos.y));

		float scrollOffset = Muscle::CTime::GetGamePlayTime() * noise._scrollSpeed;

		ImGui::Image(TextureLoader::GetNoiseMap({ noise._frequency,noise._octaves,noise._octaveMultiplier }), ImVec2(150, 150), ImVec2(0, 0 + scrollOffset), ImVec2(1, 1 + scrollOffset));

		ImGui::SetCursorPos(ImVec2(420, prevPos.y + 150));

		ImGui::Text("PreView");

		ImGui::SetCursorPos(pos);
	}

}
void Inspector::Collision()
{
	auto& collision = _selectedParticle->GetParticleData()->_collision;
	ImGui::Checkbox("##Collision", &collision._useModule);

	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Collision"))
	{
		ImGui::Text("PlaneCount"); ImGui::SameLine(offset_x); ImGui::SliderInt(" ##Height", &collision._planeCount, 0, 8);

		for (int i = 0; i < collision._planeCount; i++)
		{
			std::string str = "##Position" + std::to_string(i);

			ImGui::Text("Position"); ImGui::SameLine(offset_x); ImGui::DragFloat3(str.c_str(), reinterpret_cast<float*>(&collision._planePos[i]), 0.1f);

			str = "##NormalVector" + std::to_string(i);

			ImGui::Text("NormalVector"); ImGui::SameLine(offset_x); ImGui::DragFloat3(str.c_str(), reinterpret_cast<float*>(&collision._planeNormalVec[i]), 0.1f);

			ImGui::NewLine();
		}

		ImGui::Text("Boundce"); ImGui::SameLine(offset_x); ImGui::DragFloat(" ##Boundce", &collision._boundce, 0.1f);

		ImGui::Text("LifeTimeLoss"); ImGui::SameLine(offset_x); ImGui::DragFloat(" ##LifeTimeLoss", &collision._lifeTimeLoss, 0.1f);
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
	auto& Trail = _selectedParticle->GetParticleData()->_trails;

	ImGui::Checkbox("##Trails", &Trail._useModule);

	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Trails"))
	{
		ImGui::Text("Ratio"); ImGui::SameLine(offset_x); ImGui::DragFloat(" ##Ratio", &Trail._ratio, 0.1, 0.0f, 1.0f);

		ImGui::Text("VertexCount"); ImGui::SameLine(offset_x); ImGui::DragInt(" ##VertexCount", &Trail._trailVertexCount, 1, 1, 15);

		ImGui::Text("MinimumVertexDistance"); ImGui::SameLine(offset_x); ImGui::DragFloat(" ##MinimumVertexDistance", &Trail._minimumVertexDistance, 0.01f, 1.0f);

		ImGui::Text("WorldSpace"); ImGui::SameLine(offset_x); ImGui::Checkbox(" ##WorldSpace", &Trail._worldSpace);

		ImGui::Text("DieWithParticle"); ImGui::SameLine(offset_x); ImGui::Checkbox(" ##DieWithParticle", &Trail._dieWithParticle);

		const char* items[] = { "Stretch", "Tile", "DistributePerSegment", "RepeatPerSegment" };
		ImGui::Text("TextureMode"); ImGui::SameLine(offset_x); ImGui::Combo(" ##TextureMode", reinterpret_cast<int*>(&Trail._textureMode), items, IM_ARRAYSIZE(items));

		ImGui::Text("SizeAffectsWidth"); ImGui::SameLine(offset_x); ImGui::Checkbox(" ##SizeAffectsWidth", &Trail._sizeAffectsWidth);

		ImGui::Text("SizeAffectsLifetime"); ImGui::SameLine(offset_x); ImGui::Checkbox(" ##SizeAffectsLifetime", &Trail._sizeAffectsLifeTime);

		ImGui::Text("InheritParticleColor"); ImGui::SameLine(offset_x); ImGui::Checkbox(" ##InheritParticleColor", &Trail._inheritParticleColor);

		ImGui::Text("WidthOverTrail"); ImGui::SameLine(offset_x); ImGui::DragFloat(" ##WidthOverTrail", &Trail._widthOverTrail, 0.01f);

		ImGui::Text("GenerateLightingData"); ImGui::SameLine(offset_x); ImGui::Checkbox(" ##GenerateLightingData", &Trail._generateLightingData);

		ImGui::Text("ShadowBias"); ImGui::SameLine(offset_x); ImGui::DragFloat(" ##ShadowBias", &Trail._shadowBias, 0.1f);
		{
			static ColorGradientBar colorGradientBar;

			static ColorGradientBar alphaGradientBar;

			ImGui::Text("ColorOverLifeTime"); ImGui::SameLine(offset_x);

			ImVec2 pos = ImGui::GetCursorScreenPos();

			colorGradientBar.Draw(pos, 30, 300, _selectedParticle->GetParticleData()->_trails._color_Ratio_Lifetime, 8);

			pos.y = ImGui::GetCursorScreenPos().y;

			alphaGradientBar.DrawAlpha(pos, 30, 300, _selectedParticle->GetParticleData()->_trails._alpha_Ratio_Lifetime, 8);
			ImGui::NewLine();
		}
		{

			static ColorGradientBar colorGradientBar;

			static ColorGradientBar alphaGradientBar;

			ImGui::Text("ColorOverTrail"); ImGui::SameLine(offset_x);

			ImVec2 pos = ImGui::GetCursorScreenPos();

			colorGradientBar.Draw(pos, 30, 300, _selectedParticle->GetParticleData()->_trails._color_Ratio_Trail, 8);

			pos.y = ImGui::GetCursorScreenPos().y;

			alphaGradientBar.DrawAlpha(pos, 30, 300, _selectedParticle->GetParticleData()->_trails._alpha_Ratio_Trail, 8);
			ImGui::NewLine();
		}



	}
}
void Inspector::Renderer()
{
	auto textureBoxFunc = [&](bool& isOpen, tstring& path, const char* str)
	{
		ImGui::Begin(str, &isOpen);
		ImGui::SetWindowSize(ImVec2(300, 300));
		if (ImGui::Button(str, ImVec2(100, 30)))
		{
			auto str = FileDialogs::OpenTextureFile();

			if (!str.empty())
				TextureLoader::InsertTexture(str);
		}

		ImVec2 rect = ImGui::GetWindowSize();

		int count = 1;

		float offset = 120;

		for (auto& iter : TextureLoader::GetTextureFilePaths())
		{
			if (_selectedParticle)
			{
				rect.x -= offset;

				if (ImGui::ImageButton(TextureLoader::GetTexture(iter), ImVec2(100, 100))) // 텍스쳐 버튼을 만들고.
					path = iter; // 버튼을 클릭하면 파티클의 텍스쳐를 변경한다.

				if (rect.x > offset)
					ImGui::SameLine(offset * count++);
				else
				{
					count = 1;
					rect = ImGui::GetWindowSize();
				}
			}
		}
		ImGui::End();
	};


	auto& renderer = _selectedParticle->GetParticleData()->_renderer;

	ImGui::Checkbox("##Renderer", &_selectedParticle->GetParticleData()->_renderer._useModule);

	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Renderer"))
	{
		const char* renderMode[] = { "Billboard", "StretchedBillboard", "HorizontalBillboard", "VerticalBillboard","Mesh","None" };
		ImGui::Text("Render Mode"); ImGui::SameLine(offset_x); ImGui::Combo(" ##Render Mode", reinterpret_cast<int*>(&renderer._renderMode), renderMode, IM_ARRAYSIZE(renderMode));

		switch (renderer._renderMode)
		{
		case MuscleGrapics::Particle_Renderer::RenderMode::StretchedBillboard:
			ImGui::Text("SpeedScale"); ImGui::SameLine(offset_x); ImGui::DragFloat(" ##SpeedScale", &renderer._speedScale, 0.1f);

			ImGui::Text("LengthScale"); ImGui::SameLine(offset_x); ImGui::DragFloat(" ##LengthScale", &renderer._lengthScale, 0.1f);
			break;
		default:
			break;
		}

		const char* blendState[] = { "OIT_Default", "OIT_Additive","ForwardAlphaSorting" };
		auto prev = renderer._blendState;
		ImGui::Text("Blend State"); ImGui::SameLine(offset_x); ImGui::Combo(" ##Blend State", reinterpret_cast<int*>(&renderer._blendState), blendState, IM_ARRAYSIZE(blendState));

		if (prev != renderer._blendState)
		{
			auto& shader = _selectedParticle->GetParticleData()->shaderName;
			switch (renderer._blendState)
			{
			case MuscleGrapics::Particle_Renderer::BlendState::OIT_Default:
				shader[0] = "OITParticlePass";
				break;
			case MuscleGrapics::Particle_Renderer::BlendState::OIT_Additive:
				shader[0] = "OITParticlePass";
				break;
			case MuscleGrapics::Particle_Renderer::BlendState::Foward:
				shader[0] = "BasicParticle";
				break;
			default:
				break;
			}
		}

		ImGui::Text("ParticleTexture"); ImGui::SameLine(offset_x);

		if (ImGui::ImageButton(TextureLoader::GetTexture(_selectedParticle->GetParticleData()->_renderer._texturePath), ImVec2(100, 100)))
			_isTextureBoxOpen = true;

		ImGui::Text("TrailTexture"); ImGui::SameLine(offset_x);

		if (ImGui::Button("TrailsTexture", ImVec2(100, 100)))
			_isTrailTextureBoxOpen = true;

		const char* MaskingMode[] = { "NoMasking", "VisibleInsideMask", "OutsideMask" };
		ImGui::Text("Masking"); ImGui::SameLine(offset_x); ImGui::Combo(" ##Masking", reinterpret_cast<int*>(&renderer._masking), MaskingMode, IM_ARRAYSIZE(MaskingMode));

		ImGui::NewLine();

		if (_isTextureBoxOpen)
			textureBoxFunc(_isTextureBoxOpen, _selectedParticle->GetParticleData()->_renderer._texturePath, "ParticleTextureBox");

		if (_isTrailTextureBoxOpen)
			textureBoxFunc(_isTrailTextureBoxOpen, _selectedParticle->GetParticleData()->_renderer._traillTexturePath, "TrailTextureBox");
	}


}
