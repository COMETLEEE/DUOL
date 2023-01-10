#include "include/Core/Pass/OnlyObjectIDPass.h"

MuscleGrapics::OnlyObjectIDPass::OnlyObjectIDPass() : PassBase<RenderingData_3D>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST), _drawIndex(0)
{
}

void MuscleGrapics::OnlyObjectIDPass::SetConstants(RenderingData_3D& renderingData)
{
}

void MuscleGrapics::OnlyObjectIDPass::Draw(RenderingData_3D& renderingData)
{
}
