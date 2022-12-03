#include "ImGuiRnedererBase.h"
#include "MuscleEngine.h"
#include "GraphicsManager.h"

Muscle::ImGuiRnedererBase::ImGuiRnedererBase(std::shared_ptr<GameObject> _GameObject) : IComponents(_GameObject)
{
}

void Muscle::ImGuiRnedererBase::Render()
{
	MuscleEngine::GetInstance()->GetGraphicsManager()->PostRenderingData_Imgui(_renderingFunc);
}
