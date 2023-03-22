#include "DUOLGraphicsLibrary_Direct3D11/Direct3D11.h"
#include "DUOLGraphicsLibrary/PipelineStateFlags.h"
#include "DUOLGraphicsLibrary_Direct3D11/ComPtr.h"

namespace DUOLGraphicsLibrary
{
	class D3D11StateManager
	{
	public:
		D3D11StateManager()
		{
			
		}

	private:
		struct D3D11InputAssemblyState
		{
			D3D11InputAssemblyState() :
				_primitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED)
				, _inputLayout(nullptr)
			{

			}

			D3D11_PRIMITIVE_TOPOLOGY _primitiveTopology;

			ID3D11InputLayout* _inputLayout;
		};

		struct D3D11ShaderState
		{
			D3D11ShaderState() :
				_vertexShader(nullptr)
				, _hullShader(nullptr)
				, _domainShader(nullptr)
				, _geometryShader(nullptr)
				, _pixelShader(nullptr)
				, _computeShader(nullptr)
			{

			}

			ID3D11VertexShader* _vertexShader;
			ID3D11HullShader* _hullShader;
			ID3D11DomainShader* _domainShader;
			ID3D11GeometryShader* _geometryShader;
			ID3D11PixelShader* _pixelShader;
			ID3D11ComputeShader* _computeShader;
		};

		struct D3D11RenderState
		{
			D3D11RenderState() :
				_rasterizerState(nullptr)
				, _depthStencilState(nullptr)
				, _stencilRef(0)
				, _blendState(nullptr)
				, _sampleMask(0xffffffff)
				, _blendFactor{1.f, 1.f, 1.f, 1.f}
			{

			}

			ID3D11RasterizerState* _rasterizerState;
			ID3D11DepthStencilState* _depthStencilState;
			UINT _stencilRef;
			ID3D11BlendState* _blendState;
			FLOAT _blendFactor[4];
			UINT _sampleMask;
		};

		D3D11InputAssemblyState _inputAssemblyState;

		D3D11ShaderState _shaderState;

		D3D11RenderState _renderState;

	public:
		void Flush();
			
		void SetViewports(ID3D11DeviceContext* context, UINT numViewports, const Viewport* viewportArray);
		//void SetScissors(std::uint32_t numScissors, const Scissor* scissorArray);

		void SetPrimitiveTopology(ID3D11DeviceContext* context, D3D11_PRIMITIVE_TOPOLOGY primitiveTopology);

		void SetInputLayout(ID3D11DeviceContext* context, ID3D11InputLayout* inputLayout);

		void SetVertexShader(ID3D11DeviceContext* context, ID3D11VertexShader* shader);

		void SetHullShader(ID3D11DeviceContext* context, ID3D11HullShader* shader);

		void SetDomainShader(ID3D11DeviceContext* context, ID3D11DomainShader* shader);

		void SetGeometryShader(ID3D11DeviceContext* context, ID3D11GeometryShader* shader);

		void SetPixelShader(ID3D11DeviceContext* context, ID3D11PixelShader* shader);

		void SetComputeShader(ID3D11DeviceContext* context, ID3D11ComputeShader* shader);

		void SetRasterizerState(ID3D11DeviceContext* context, ID3D11RasterizerState* rasterizerState);

		void SetDepthStencilState(ID3D11DeviceContext* context, ID3D11DepthStencilState* depthStencilState);

		void SetDepthStencilState(ID3D11DeviceContext* context, ID3D11DepthStencilState* depthStencilState, UINT stencilRef);

		void SetStencilRef(ID3D11DeviceContext* context, UINT stencilRef);

		void SetBlendState(ID3D11DeviceContext* context, ID3D11BlendState* blendState, UINT sampleMask);

		void SetBlendState(ID3D11DeviceContext* context, ID3D11BlendState* blendState, const FLOAT* blendFactor, UINT sampleMask);

		void SetBlendFactor(ID3D11DeviceContext* context, const FLOAT* blendFactor);

		void SetConstantBuffers(
			ID3D11DeviceContext* context,
			UINT                    startSlot,
			UINT                    bufferCount,
			ID3D11Buffer* const* buffers,
			long                    stageFlags
		);

		void SetConstantBuffersArray(
			ID3D11DeviceContext* context,
			UINT                    startSlot,
			UINT                    bufferCount,
			ID3D11Buffer* const* buffers,
			const UINT* firstConstants,
			const UINT* numConstants,
			long                    stageFlags
		);

		void SetShaderResources(
			ID3D11DeviceContext* context,
			UINT                                startSlot,
			UINT                                resourceCount,
			ID3D11ShaderResourceView* const* views,
			long                                stageFlags
		);

		void SetUnorderedAccessView(
			ID3D11DeviceContext* context,
			UINT                                startSlot,
			UINT                                resourceCount,
			UINT* initialCount,
			ID3D11UnorderedAccessView* const* views,
			long                                stageFlags
		);


		void SetSamplers(
			ID3D11DeviceContext* context,
			UINT                        startSlot,
			UINT                       samplerCount,
			ID3D11SamplerState* const* samplers,
			long                        stageFlags
		);
	};
}