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
		ImGui::Text("Duration"); ImGui::SameLine(offset_x); ImGui::InputFloat("Duration", &_myParticle->GetParticleData()->_commonInfo->_duration, 0.1f, 1.0f, "%.3f");

		ImGui::Text("Looping"); ImGui::SameLine(offset_x); ImGui::Checkbox("Looping", &_myParticle->GetParticleData()->_commonInfo->_looping);      // Edit bools storing our window open/close state

		ImGui::Text("StartDelay"); ImGui::SameLine(offset_x); ImGui::InputFloat("StartDelay", &_myParticle->GetParticleData()->_commonInfo->_startDelay[0], 0.1f, 1.0f, "%.3f");

		ParticleSystemCommonInfo_StartLifetime();

		ParticleSystemCommonInfo_StartSpeed();

		ParticleSystemCommonInfo_StartSize();

		ParticleSystemCommonInfo_StartRotation();

		ParticleSystemCommonInfo_StartColor();

		ParticleSystemCommonInfo_GravityModifier();

		ImGui::Text("MaxParticles"); ImGui::SameLine(offset_x);
		if (ImGui::InputInt("MaxParticles", &_myParticle->GetParticleData()->_commonInfo->_maxParticles, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue))
			_myParticle->Play();

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
void Inspector::ParticleSystemCommonInfo_StartSize()
{
	ImGui::Text("StartSize"); ImGui::SameLine(120);

	if (ImGui::Button("Option##StartSize"))
		ImGui::OpenPopup("StartSize_popup");

	switch (_myParticle->GetParticleData()->_commonInfo->_startSizeOption)
	{
	case Particle_CommonInfo::Option_Particle::Constant:

		ImGui::SameLine(offset_x); ImGui::InputFloat2("StartSize", reinterpret_cast<float*>(&_myParticle->GetParticleData()->_commonInfo->_startSize[0]));

		_myParticle->GetParticleData()->_commonInfo->_startSize[1] = _myParticle->GetParticleData()->_commonInfo->_startSize[0];

		break;

	case Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant:

		ImGui::SameLine(offset_x); ImGui::InputFloat2("StartSize##1", reinterpret_cast<float*>(&_myParticle->GetParticleData()->_commonInfo->_startSize[0]));

		ImGui::NewLine();

		ImGui::SameLine(offset_x); ImGui::InputFloat2("StartSize##2", reinterpret_cast<float*>(&_myParticle->GetParticleData()->_commonInfo->_startSize[1]));

		break;

	default:

		break;

	}



	if (ImGui::BeginPopup("StartSize_popup"))
	{
		ImGui::Text("Option");

		ImGui::Separator();

		if (ImGui::Selectable("Constant"))
			_myParticle->GetParticleData()->_commonInfo->_startSizeOption = Particle_CommonInfo::Option_Particle::Constant;

		if (ImGui::Selectable("Random Between Two Constants"))
			_myParticle->GetParticleData()->_commonInfo->_startSizeOption = Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant;

		ImGui::EndPopup();
	}
}
void Inspector::ParticleSystemCommonInfo_StartSpeed()
{

	ImGui::Text("StartSpeed"); ImGui::SameLine(120);

	if (ImGui::Button("Option##StartSpeed"))
		ImGui::OpenPopup("StartSpeed_popup");


	switch (_myParticle->GetParticleData()->_commonInfo->_startSpeedOption)
	{
	case Particle_CommonInfo::Option_Particle::Constant:

		ImGui::SameLine(offset_x); ImGui::InputFloat("StartSpeed", &_myParticle->GetParticleData()->_commonInfo->_startSpeed[0], 0.1f, 1.0f, "%.3f");

		_myParticle->GetParticleData()->_commonInfo->_startSpeed[1] = _myParticle->GetParticleData()->_commonInfo->_startSpeed[0];

		break;
	case Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant:

		ImGui::SameLine(offset_x); ImGui::InputFloat("StartSpeed##1", &_myParticle->GetParticleData()->_commonInfo->_startSpeed[0], 0.1f, 1.0f, "%.3f");

		ImGui::NewLine();

		ImGui::SameLine(offset_x); ImGui::InputFloat("StartSpeed##2", &_myParticle->GetParticleData()->_commonInfo->_startSpeed[1], 0.1f, 1.0f, "%.3f");

		break;
	default:

		break;
	}



	if (ImGui::BeginPopup("StartSpeed_popup"))
	{
		ImGui::Text("Option");

		ImGui::Separator();

		if (ImGui::Selectable("Constant"))
			_myParticle->GetParticleData()->_commonInfo->_startSpeedOption = Particle_CommonInfo::Option_Particle::Constant;

		if (ImGui::Selectable("Random Between Two Constants"))
			_myParticle->GetParticleData()->_commonInfo->_startSpeedOption = Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant;

		ImGui::EndPopup();
	}

}
void Inspector::ParticleSystemCommonInfo_StartLifetime()
{
	ImGui::Text("StartLifeTime"); ImGui::SameLine(120);

	if (ImGui::Button("Option##StartLifeTime"))
		ImGui::OpenPopup("StartLifeTime_popup");


	switch (_myParticle->GetParticleData()->_commonInfo->_startLifeTimeOption)
	{
	case Particle_CommonInfo::Option_Particle::Constant:

		ImGui::SameLine(offset_x); ImGui::InputFloat("StartLifeTime", &_myParticle->GetParticleData()->_commonInfo->_startLifeTime[0], 0.01f, 1.0f, "%.3f");

		_myParticle->GetParticleData()->_commonInfo->_startLifeTime[1] = _myParticle->GetParticleData()->_commonInfo->_startLifeTime[0];

		break;
	case Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant:

		ImGui::SameLine(offset_x); ImGui::InputFloat("StartLifeTime##1", &_myParticle->GetParticleData()->_commonInfo->_startLifeTime[0], 0.1f, 1.0f, "%.3f");

		ImGui::NewLine();

		ImGui::SameLine(offset_x); ImGui::InputFloat("StartLifeTime##2", &_myParticle->GetParticleData()->_commonInfo->_startLifeTime[1], 0.1f, 1.0f, "%.3f");

		break;
	default:

		break;
	}



	if (ImGui::BeginPopup("StartLifeTime_popup"))
	{
		ImGui::Text("Option");

		ImGui::Separator();

		if (ImGui::Selectable("Constant"))
			_myParticle->GetParticleData()->_commonInfo->_startLifeTimeOption = Particle_CommonInfo::Option_Particle::Constant;

		if (ImGui::Selectable("Random Between Two Constants"))
			_myParticle->GetParticleData()->_commonInfo->_startLifeTimeOption = Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant;

		ImGui::EndPopup();
	}
}
void Inspector::ParticleSystemCommonInfo_StartRotation()
{

	ImGui::Text("StartRotation"); ImGui::SameLine(120);

	if (ImGui::Button("Option##StartRotation"))
		ImGui::OpenPopup("StartRotation_popup");


	switch (_myParticle->GetParticleData()->_commonInfo->_startRotationOption)
	{
	case Particle_CommonInfo::Option_Particle::Constant:

		ImGui::SameLine(offset_x); ImGui::InputFloat("StartRotation", &_myParticle->GetParticleData()->_commonInfo->_startRotation[0], 0.03f, 1.0f, "%.3f");

		_myParticle->GetParticleData()->_commonInfo->_startRotation[1] = _myParticle->GetParticleData()->_commonInfo->_startRotation[0];

		break;
	case Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant:

		ImGui::SameLine(offset_x); ImGui::InputFloat("StartRotation##1", &_myParticle->GetParticleData()->_commonInfo->_startRotation[0], 0.03f, 1.0f, "%.3f");

		ImGui::NewLine();

		ImGui::SameLine(offset_x); ImGui::InputFloat("StartRotation##2", &_myParticle->GetParticleData()->_commonInfo->_startRotation[1], 0.03f, 1.0f, "%.3f");

		break;
	default:

		break;
	}



	if (ImGui::BeginPopup("StartRotation_popup"))
	{
		ImGui::Text("Option");

		ImGui::Separator();

		if (ImGui::Selectable("Constant"))
			_myParticle->GetParticleData()->_commonInfo->_startRotationOption = Particle_CommonInfo::Option_Particle::Constant;

		if (ImGui::Selectable("Random Between Two Constants"))
			_myParticle->GetParticleData()->_commonInfo->_startRotationOption = Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant;

		ImGui::EndPopup();
	}
}
void Inspector::ParticleSystemCommonInfo_StartColor()
{

	ImGui::Text("StartColor"); ImGui::SameLine(120);

	if (ImGui::Button("Option##StartStartColor"))
		ImGui::OpenPopup("StartColor_popup");


	switch (_myParticle->GetParticleData()->_commonInfo->_startColorOption)
	{
	case Particle_CommonInfo::Option_Particle::Constant:

		ImGui::SameLine(offset_x); ImGui::ColorEdit4("StartColor", reinterpret_cast<float*>(&_myParticle->GetParticleData()->_commonInfo->_startColor[0])); // Edit 3 floats representing a color

		_myParticle->GetParticleData()->_commonInfo->_startColor[1] = _myParticle->GetParticleData()->_commonInfo->_startColor[0];

		break;
	case Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant:

		ImGui::SameLine(offset_x); ImGui::ColorEdit4("StartColor##1", reinterpret_cast<float*>(&_myParticle->GetParticleData()->_commonInfo->_startColor[0])); // Edit 3 floats representing a color

		ImGui::NewLine();

		ImGui::SameLine(offset_x); ImGui::ColorEdit4("StartColor##2", reinterpret_cast<float*>(&_myParticle->GetParticleData()->_commonInfo->_startColor[1])); // Edit 3 floats representing a color

		break;
	default:

		break;
	}



	if (ImGui::BeginPopup("StartColor_popup"))
	{
		ImGui::Text("Option");

		ImGui::Separator();

		if (ImGui::Selectable("Constant"))
			_myParticle->GetParticleData()->_commonInfo->_startColorOption = Particle_CommonInfo::Option_Particle::Constant;

		if (ImGui::Selectable("Random Between Two Constants"))
			_myParticle->GetParticleData()->_commonInfo->_startColorOption = Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant;

		ImGui::EndPopup();
	}
}
void Inspector::ParticleSystemCommonInfo_GravityModifier()
{

	ImGui::Text("GravityModifier"); ImGui::SameLine(120);

	if (ImGui::Button("Option##GravityModifier"))
		ImGui::OpenPopup("GravityModifier_popup");


	switch (_myParticle->GetParticleData()->_commonInfo->_gravirtModifierOption)
	{
	case Particle_CommonInfo::Option_Particle::Constant:

		ImGui::SameLine(offset_x); ImGui::InputFloat("GravityModifier", &_myParticle->GetParticleData()->_commonInfo->_gravityModifier[0], 0.1f, 1.0f, "%.3f");

		_myParticle->GetParticleData()->_commonInfo->_gravityModifier[1] = _myParticle->GetParticleData()->_commonInfo->_gravityModifier[0];

		break;
	case Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant:

		ImGui::SameLine(offset_x); ImGui::InputFloat("GravityModifier##1", &_myParticle->GetParticleData()->_commonInfo->_gravityModifier[0], 0.1f, 1.0f, "%.3f");

		ImGui::NewLine();

		ImGui::SameLine(offset_x); ImGui::InputFloat("GravityModifier##2", &_myParticle->GetParticleData()->_commonInfo->_gravityModifier[1], 0.1f, 1.0f, "%.3f");

		break;
	default:

		break;
	}



	if (ImGui::BeginPopup("GravityModifier_popup"))
	{
		ImGui::Text("Option");

		ImGui::Separator();

		if (ImGui::Selectable("Constant"))
			_myParticle->GetParticleData()->_commonInfo->_gravirtModifierOption = Particle_CommonInfo::Option_Particle::Constant;

		if (ImGui::Selectable("Random Between Two Constants"))
			_myParticle->GetParticleData()->_commonInfo->_gravirtModifierOption = Particle_CommonInfo::Option_Particle::RandomBetweenTwoConstant;

		ImGui::EndPopup();
	}
}

void Inspector::Emission()
{
	if (ImGui::CollapsingHeader("Emission"))
	{
		ImGui::Text("EmissiveCount"); ImGui::SameLine(offset_x);

		if (ImGui::InputInt("EmissiveCount", &_myParticle->GetParticleData()->_emission->_emissiveCount, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue))
			_myParticle->Play();

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

		static float start = 0;

		static float end = 1;

		static float startOffset = 0;

		static float endOffset = 0;


		ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);

		ImGui::SliderFloat("StartSize", &_myParticle->GetParticleData()->_size_Over_Lifetime->_startSize, 0, 1);

		ImGui::SameLine(ImGui::GetFontSize() * 16);

		ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);

		ImGui::SliderFloat("EndSize", &_myParticle->GetParticleData()->_size_Over_Lifetime->_endSize, 0, 1);

		ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);

		ImGui::SliderFloat("StartOffset", &_myParticle->GetParticleData()->_size_Over_Lifetime->_startOffset, 0, 1);

		ImGui::SameLine(ImGui::GetFontSize() * 16);

		ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);

		ImGui::SliderFloat("EndOffset", &_myParticle->GetParticleData()->_size_Over_Lifetime->_endOffset, 0, 1);

		start = _myParticle->GetParticleData()->_size_Over_Lifetime->_startSize;

		end = _myParticle->GetParticleData()->_size_Over_Lifetime->_endSize;

		startOffset = _myParticle->GetParticleData()->_size_Over_Lifetime->_startOffset;

		endOffset = _myParticle->GetParticleData()->_size_Over_Lifetime->_endOffset;

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
		ImGui::Text("AngularVelocity"); ImGui::SameLine(offset_x); ImGui::InputFloat("AngularVelocity", &_myParticle->GetParticleData()->_rotation_Over_Lifetime->_AngularVelocity, 0.03f, 0.1f, "%.3f");
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

		auto& grid_xy = _myParticle->GetParticleData()->_texture_Sheet_Animaition->_grid_XY;

		static int grid_size = 0; // = grid_xy[0] * grid_xy[1];

		grid_size = grid_xy[0] * grid_xy[1];

		ImGui::Text("Grid_XY"); ImGui::SameLine(offset_x); ImGui::InputInt2("Grid_XY", grid_xy);

		if (grid_xy[0] < 1 || grid_xy[1] < 1)
		{
			grid_xy[0] = 1;
			grid_xy[1] = 1;
		}

		ImGui::Text("Mode"); ImGui::SameLine(offset_x); ImGui::Combo("test", &_myParticle->GetParticleData()->_texture_Sheet_Animaition->_timeMode, { "None\0LifeTime" });
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
		// 이 부분을 리플렉션으로 하면 참 예쁠것 같다...
		// 나중에 여유가 생기면 부스트 라이브러리의 리플렉션 사용해보자.
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
