#include "DUOLGraphicsLibrary_Direct3D11/Renderer/PipelineState/D3D11ComputePipelineState.h"

#include "DUOLGraphicsLibrary/Core/TypeCast.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Shader/D3D11Shader.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/StateManager/D3D11StateManager.h"

namespace DUOLGraphicsLibrary
{
	DUOLGraphicsLibrary::D3D11ComputePipelineState::D3D11ComputePipelineState(
		const UINT64& guid,
		ID3D11Device* device,
		const ComputePipelineStateDesc& pipelineStateDesc) :
		D3D11PipelineState(guid)
	{
		LoadShader(pipelineStateDesc);
	}

	bool D3D11ComputePipelineState::HasGeometryShader()
	{
		return false;
	}

	void D3D11ComputePipelineState::BindPipeline(D3D11StateManager* stateManager, ID3D11DeviceContext* context)
	{
		stateManager->SetComputeShader(context ,_computeShader.Get());
	}

	void D3D11ComputePipelineState::LoadShader(const ComputePipelineStateDesc& pipelineStateDesc)
	{
		if (pipelineStateDesc._computeShader != nullptr && pipelineStateDesc._computeShader->GetShaderType() == ShaderType::COMPUTE)
		{
			_computeShader = TYPE_CAST(D3D11Shader*, pipelineStateDesc._computeShader)->GetNativeShader()._computeShader;
		}
	}
}
