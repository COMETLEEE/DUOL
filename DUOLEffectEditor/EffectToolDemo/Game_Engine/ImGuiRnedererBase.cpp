#include "pch.h"
#include "ImGuiRnedererBase.h"

Muscle::ImGuiRnedererBase::ImGuiRnedererBase(std::shared_ptr<GameObject> _GameObject) : IComponents(_GameObject)
{
}

void Muscle::ImGuiRnedererBase::Render()
{
	MuscleEngine::GetInstance()->GetGraphicsManager()->PostRenderingData_Imgui(_renderingFunc);
}
