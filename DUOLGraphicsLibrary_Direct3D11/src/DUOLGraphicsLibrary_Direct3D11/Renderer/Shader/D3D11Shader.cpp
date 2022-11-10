#include "D3D11Shader.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXHelper.h"
#include <string>
#include <vector>
#include "d3d11shader.h"
#include "d3dcompiler.h"

namespace DUOLGraphicsLibrary
{
	D3D11Shader::D3D11Shader(const UINT64& guid, ID3D11Device* device, const ShaderDesc& shaderDesc) :
		Shader(guid, shaderDesc._type)
	{
		if (CreateShader(device, shaderDesc))
		{
			DUOLGRAPHICS_ASSERT("Shader Create Error");
		}
	}

	bool D3D11Shader::CreateShader(ID3D11Device* device, const ShaderDesc& shaderDesc)
	{

		CompileShader(device, shaderDesc);
		CreateNativeShaderFromBlob(device, shaderDesc);

		return true;
	}

	bool D3D11Shader::CreateNativeShaderFromBlob(ID3D11Device* device, const ShaderDesc& shaderDesc)
	{
		if (_shaderBlob == nullptr)
		{
			return false;
		}

		HRESULT hr = S_OK;
		ComPtr<ID3D11ShaderReflection> ShaderReflector = nullptr;
		D3DReflect(_shaderBlob->GetBufferPointer(), _shaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)ShaderReflector.GetAddressOf());

		switch (shaderDesc._type)
		{
		case ShaderType::UNKNOWN:
		{
			DUOLGRAPHICS_ASSERT("Shader Type Can't Be Unkown");
			break;
		}
		case ShaderType::VERTEX:
		{
			hr = device->CreateVertexShader(
				_shaderBlob->GetBufferPointer()
				, _shaderBlob->GetBufferSize(),
				nullptr,
				_nativeShader._vertexShader.ReleaseAndGetAddressOf());

			BuildInputLayout(device, ShaderReflector);
			break;
		}
		case ShaderType::HULL:
		{
			break;
		}
		case ShaderType::DOMAINS:
		{
			break;
		}
		case ShaderType::GEOMETRY:
		{
			break;
		}
		case ShaderType::PIXEL:
		{
			hr = device->CreatePixelShader(
				_shaderBlob->GetBufferPointer()
				, _shaderBlob->GetBufferSize(),
				nullptr,
				_nativeShader._pixelShader.ReleaseAndGetAddressOf());
			break;
		}
		case ShaderType::COMPUTE:
		{
			break;
		}
		default:
		{
			break;
		}
		}

		DXThrowError(hr, "D3D11Shader CreateNativeShader Failed");

		return false;
	}

	bool D3D11Shader::CompileShader(ID3D11Device* device, const ShaderDesc& shaderDesc)
	{
		ComPtr<ID3DBlob> errorBlob = nullptr;

		D3D_SHADER_MACRO shaderMacro;

		shaderMacro.Name = shaderDesc._shaderMacro.Name;
		shaderMacro.Definition = shaderDesc._shaderMacro.Definition;

		const char* entry = shaderDesc._entryPoint;
		const char* profile = shaderDesc._profile;

		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
		flags |= D3DCOMPILE_DEBUG;
		flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		flags |= shaderDesc._flags;

		std::string strfilepath(shaderDesc._source);
		std::wstring filePath(strfilepath.begin(), strfilepath.end());

		HRESULT hr;

		if (shaderDesc._shaderMacro.Name != nullptr)
		{
			hr = D3DCompileFromFile(
				filePath.c_str()
				, &shaderMacro
				, D3D_COMPILE_STANDARD_FILE_INCLUDE
				, entry
				, profile
				, flags
				, 0
				, _shaderBlob.ReleaseAndGetAddressOf()
				, errorBlob.ReleaseAndGetAddressOf());

		DXThrowError(hr, "D3D11Shader CreateShader Error");
		}
		else
		{
			hr = D3DCompileFromFile(
				filePath.c_str()
				, nullptr
				, D3D_COMPILE_STANDARD_FILE_INCLUDE
				, entry
				, profile
				, flags
				, 0
				, _shaderBlob.ReleaseAndGetAddressOf()
				, errorBlob.ReleaseAndGetAddressOf());

			DXThrowError(hr, "D3D11Shader CreateShader Error");
		}

		if (hr == E_FAIL)
		{
			if (errorBlob.Get())
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}
		}

		return true;
	}

	bool D3D11Shader::BuildInputLayout(ID3D11Device* device, ComPtr<ID3D11ShaderReflection> ShaderReflector)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDesc;

		D3D11_SHADER_DESC ShaderDesc;
		ShaderReflector->GetDesc(&ShaderDesc);

		for (unsigned InputIndex = 0; InputIndex < ShaderDesc.InputParameters; InputIndex++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			ShaderReflector->GetInputParameterDesc(InputIndex, &paramDesc);

			D3D11_INPUT_ELEMENT_DESC elementDesc;

			elementDesc.SemanticName = paramDesc.SemanticName;
			elementDesc.SemanticIndex = paramDesc.SemanticIndex; //동일한 시멘틱스를 사용하는 매개변수 구분
			elementDesc.InputSlot = 0; //d3d11에 정의된 0~15값 input assembler
			elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT; //편의를 위해 이걸로, 
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;

			//Mask which indicates which components of a register are used.

			if (paramDesc.Mask == 1)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			}
			else if (paramDesc.Mask <= 3)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			}
			else if (paramDesc.Mask <= 7)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else if (paramDesc.Mask <= 15)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}

			InputLayoutDesc.push_back(elementDesc);
		}

		//_shaderblob을 통해 유효성 검사를 한다
		auto hr = device->CreateInputLayout(&InputLayoutDesc.front(), InputLayoutDesc.size(), _shaderBlob->GetBufferPointer(), _shaderBlob->GetBufferSize(), _inputLayout.GetAddressOf());

		DXThrowError(hr, "D3D11Shader CreateInputLayout Error");

		return true;
	}
}
