#include "Core/Pass/3DObject/SkyBoxPass.h"

MuscleGrapics::SkyBoxPass::SkyBoxPass() : PassBase<RenderingData_3D>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
_drawIndex(0)
{
}

void MuscleGrapics::SkyBoxPass::SetConstants(RenderingData_3D& renderingData)
{
	PassBase<RenderingData_3D>::SetConstants(renderingData);
}

void MuscleGrapics::SkyBoxPass::Draw(RenderingData_3D& renderingData)
{
	PassBase<RenderingData_3D>::Draw(renderingData);
}
