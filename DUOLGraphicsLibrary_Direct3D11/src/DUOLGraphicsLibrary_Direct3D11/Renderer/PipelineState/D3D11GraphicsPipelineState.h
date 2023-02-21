#pragma once
#include "D3D11PipelineState.h"

namespace DUOLGraphicsLibrary
{
	class D3D11GraphicsPipelineState : public D3D11PipelineState
	{
	public:
		D3D11GraphicsPipelineState(const UINT64& guid, ID3D11Device* device, const PipelineStateDesc& pipelineStateDesc);
		
		virtual bool HasGeometryShader() override;

		virtual void BindPipeline(D3D11StateManager* stateManager, ID3D11DeviceContext* context) override;

	private:
		ComPtr<ID3D11InputLayout>      _inputLayout;

		ComPtr<ID3D11VertexShader>      _vertexShader;

		ComPtr<ID3D11HullShader>        _hullShader;

		ComPtr<ID3D11DomainShader>      _domainShader;

		ComPtr<ID3D11GeometryShader>    _geometryShader;

		ComPtr<ID3D11PixelShader>       _pixelShader;

		ComPtr<ID3D11DepthStencilState> _depthStencilState;

		ComPtr<ID3D11RasterizerState> _rasterizerState;

		ComPtr<ID3D11BlendState>	 _blendState;

		D3D11_PRIMITIVE_TOPOLOGY        _primitiveTopology;

	private:
		void CreateDepthStencilState(ID3D11Device* device, const PipelineStateDesc& pipelineStateDesc);

		void CreateRasterizerState(ID3D11Device* device, const PipelineStateDesc& pipelineStateDesc);

		void CreateBlendState(ID3D11Device* device, const PipelineStateDesc& pipelineStateDesc);

		void LoadShader(const PipelineStateDesc& pipelineStateDesc);
	};
}