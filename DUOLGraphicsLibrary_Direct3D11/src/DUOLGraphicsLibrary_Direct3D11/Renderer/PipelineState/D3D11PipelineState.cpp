#include "D3D11PipelineState.h"

#include "DUOLGraphicsLibrary/Core/TypeCast.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXFlagMap.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Shader/D3D11Shader.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Renderer/D3D11RenderContext.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/StateManager/D3D11StateManager.h"

DUOLGraphicsLibrary::D3D11PipelineState::D3D11PipelineState(const UINT64& guid) :
	PipelineState(guid)
{
}
