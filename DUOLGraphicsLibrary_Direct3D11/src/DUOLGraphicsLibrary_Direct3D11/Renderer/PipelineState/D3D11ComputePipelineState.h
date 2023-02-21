#pragma once
#include "D3D11PipelineState.h"
#include "DUOLGraphicsLibrary_Direct3D11/Direct3D11.h"
#include "DUOLGraphicsLibrary_Direct3D11/ComPtr.h"

namespace DUOLGraphicsLibrary
{
	class D3D11StateManager;
	struct ComputePipelineStateDesc;

	class D3D11ComputePipelineState : public D3D11PipelineState
	{
	public:
		D3D11ComputePipelineState(const UINT64& guid, ID3D11Device* device, const ComputePipelineStateDesc& pipelineStateDesc);

		virtual bool HasGeometryShader() override;

		void BindPipeline(D3D11StateManager* stateManager, ID3D11DeviceContext* context);

	private:
		void LoadShader(const ComputePipelineStateDesc& pipelineStateDesc);

	private:
			ComPtr<ID3D11ComputeShader> _computeShader;
	};
}
