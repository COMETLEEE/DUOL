#pragma once
#include <d3d11shader.h>
#include "DUOLGraphicsLibrary/Renderer/Shader.h"
#include "DUOLGraphicsLibrary_Direct3D11/ComPtr.h"
#include "DUOLGraphicsLibrary_Direct3D11/Direct3D11.h"

namespace DUOLGraphicsLibrary
{

    union D3D11NativeShader
    {
        inline D3D11NativeShader() :
            _vertexShader{ nullptr }
        {
        }
        inline D3D11NativeShader(const D3D11NativeShader& shader) :
            _vertexShader{ shader._vertexShader }
        {
        }
        inline D3D11NativeShader& operator = (const D3D11NativeShader& shader)
        {
            _vertexShader = shader._vertexShader;
            return *this;
        }
        inline ~D3D11NativeShader()
        {
            _vertexShader.Reset();
        }

        ComPtr<ID3D11VertexShader>      _vertexShader;
        ComPtr<ID3D11HullShader>        _hullShader;
        ComPtr<ID3D11DomainShader>      _domainShader;
        ComPtr<ID3D11GeometryShader>    _geometryShader;
        ComPtr<ID3D11PixelShader>       _pixelShader;
        ComPtr<ID3D11ComputeShader>     _computeShader;
    };


	class D3D11Shader : public Shader
	{
	public:
        D3D11Shader(const UINT64& guid, ID3D11Device* device, const ShaderDesc& shaderDesc);

	private:
        D3D11NativeShader _nativeShader;

        ComPtr<ID3DBlob> _shaderBlob;

        ComPtr<ID3D11InputLayout> _inputLayout;

    private:
		bool CreateShader(ID3D11Device* device, const ShaderDesc& shaderDesc);

        bool CreateNativeShaderFromBlob(ID3D11Device* device, const ShaderDesc& shaderDesc);

        bool CompileShader(ID3D11Device* device, const ShaderDesc& shaderDesc);

        bool BuildInputLayout(ID3D11Device* device, ComPtr<ID3D11ShaderReflection> ShaderReflector);

	public:
        D3D11NativeShader GetNativeShader(){ return _nativeShader; }

        ComPtr<ID3D11InputLayout> GetInputLayout() const { return _inputLayout; }
	};
}
