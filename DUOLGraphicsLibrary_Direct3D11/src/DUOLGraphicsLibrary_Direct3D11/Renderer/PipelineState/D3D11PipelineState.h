#pragma once
#include "DUOLGraphicsLibrary/Renderer/PipelineState.h"
#include "DUOLGraphicsLibrary_Direct3D11/Direct3D11.h"
#include "DUOLGraphicsLibrary_Direct3D11/ComPtr.h"

namespace DUOLGraphicsLibrary
{
	class D3D11StateManager;
	/**

     @class   D3D11PipelineState
     @brief   D3D11 PipelineState
     @details ~
     @author  KyungMin Oh

 **/
	class D3D11RenderContext;

	class D3D11PipelineState : public PipelineState
	{

	public:
		D3D11PipelineState(const UINT64& guid, ID3D11Device* device, const PipelineStateDesc& pipelineStateDesc);

		void BindPipeline(D3D11StateManager* stateManager, ID3D11DeviceContext* context);

	private:
		ComPtr<ID3D11VertexShader>      _vertexShader;
		ComPtr<ID3D11InputLayout>      _inputLayout;

		ComPtr<ID3D11HullShader>        _hullShader;
		ComPtr<ID3D11DomainShader>      _domainShader;
		ComPtr<ID3D11GeometryShader>    _geometryShader;
		ComPtr<ID3D11PixelShader>       _pixelShader;

		ComPtr<ID3D11DepthStencilState> _depthStencilState;
		ComPtr<ID3D11RasterizerState> _rasterizerState;
		ComPtr<ID3D11BlendState>	 _blendState;

		D3D11_PRIMITIVE_TOPOLOGY        _primitiveTopology;

		//bool                            stencilRefDynamic_ = false;
		//UINT                            stencilRef_ = 0;
		//bool                            blendFactorDynamic_ = false;
		//FLOAT                           blendFactor_[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		//UINT                            sampleMask_ = UINT_MAX;

	private:
		void CreateDepthStencilState(ID3D11Device* device, const PipelineStateDesc& pipelineStateDesc);

		void CreateRasterizerState(ID3D11Device* device, const PipelineStateDesc& pipelineStateDesc);

		void CreateBlendState(ID3D11Device* device, const PipelineStateDesc& pipelineStateDesc);

		void LoadShader(const PipelineStateDesc& pipelineStateDesc);
	};
}

