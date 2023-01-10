/**

	@file      PassBase.h
	@brief     /// PassBase를 상속 받은 Pass 제작 후 리소스 매니저에 패스 추가. Renderer에서 다형적으로 Draw동작 수형.
	@details   ~
	@author    SinSeongHyeon
	@date      9.11.2022
	@copyright © SinSeongHyeon, 2022. All right reserved.

**/
#pragma once
#include <d3dcompiler.h>
#include "util/TypeDefine.h"
#include "util/STLInclude.h"
#include "Export/RenderingData.h"
#include "Core/DirectX11/DXEngine.h"
#include "Core/DirectX11/Renderer.h"
#include "util/VertexDesc.h"
#include "Core/DirectX11/RasterizerState.h"


#if defined(DEBUG) || defined(_DEBUG)
static uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#else
static uint32 compileFlag = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#endif

namespace MuscleGrapics
{
	template <typename T>
	class PassBase abstract
	{
	public:
		PassBase(D3D11_PRIMITIVE_TOPOLOGY topology);

		virtual ~PassBase();

	private:
		std::unordered_map<unsigned int, ID3D11InputLayout*> _inputLayout;

		D3D11_PRIMITIVE_TOPOLOGY _topolgy;

		std::unordered_map<unsigned int, ID3D11VertexShader*> _vertexShader;

		std::unordered_map<unsigned int, ID3D11PixelShader*> _pixelShader;

		std::unordered_map<unsigned int, ID3D11GeometryShader*> _geometryShader;

		std::map<UINT, ID3D11Buffer*> _constantBuffers;

	protected:
		ID3D11DeviceContext* _d3dImmediateContext;

		float _left = -1.0f;
		float _right = 1.0f;
		float _top = 1.0f;
		float _bottom = -1.0f;

	protected:
		void OutputShaderErrorMessage(ID3DBlob* errorMessage, HWND hwnd, const WCHAR* shaderFileName);
		/**
		 * \brief 오버라이딩된 Draw 함수에서 실행시켜야 하는 함수들.
		 */
		void SetShader(UINT shaderIndex = 0);

		virtual void SetConstants(T& renderingData) abstract;

		/**
		* \brief 다형적 동작으로 구현 하려고 했으나, 모든 함수를 정의 하는 것이 더 사용하기 편하고, 직관적인듯 하다.
		*/
		void CompileVertexShader(const WCHAR* fileName, const CHAR* entryName, D3D11_INPUT_ELEMENT_DESC polygonLayout[], UINT size, UINT shaderIndex = 0, const D3D_SHADER_MACRO* macro = nullptr);

		void CompilePixelShader(const WCHAR* fileName, const CHAR* entryName, UINT shaderIndex = 0, const D3D_SHADER_MACRO* macro = nullptr);

		void CompileGeometryShader(const WCHAR* fileName, const CHAR* entryName, bool useStreamOut, UINT shaderIndex = 0, const D3D_SHADER_MACRO* macro = nullptr);

		void CreateConstantBuffer(UINT slot, UINT bufferSize);


		template<class DATATYPE>
		void UpdateConstantBuffer(UINT slot, DATATYPE& data);
	public:
		virtual void Draw(T& renderingData) abstract;

		void SetDrawRectangle(float left, float right, float top, float bottom); // 이 함수 정의 하자!
	};

	template <typename T>
	PassBase<T>::PassBase(D3D11_PRIMITIVE_TOPOLOGY topology) :
		_topolgy(topology), _inputLayout(),
		_d3dImmediateContext(DXEngine::GetInstance()->Getd3dImmediateContext()),
		_vertexShader(),
		_pixelShader(),
		_geometryShader()

	{

	}

	template <typename T>
	PassBase<T>::~PassBase()
	{
		for (auto& iter : _inputLayout)
		{
			if (iter.second)
				iter.second->Release();
		}
		for (auto& iter : _vertexShader)
		{
			if (iter.second)
				iter.second->Release();
		}
		for (auto& iter : _pixelShader)
		{
			if (iter.second)
				iter.second->Release();
		}
		for (auto& iter : _geometryShader)
		{
			if (iter.second)
				iter.second->Release();
		}

		for (auto& iter : _constantBuffers)
		{
			if (iter.second)
				iter.second->Release();
		}

	}

	template <typename T>
	void PassBase<T>::CompileVertexShader(const WCHAR* fileName, const CHAR* entryName,
		D3D11_INPUT_ELEMENT_DESC polygonLayout[], UINT size, UINT shaderIndex, const D3D_SHADER_MACRO* macro)
	{
		ID3DBlob* errorMessage = nullptr;
		ID3DBlob* vertexShaderBuffer = nullptr;

		auto device = DXEngine::GetInstance()->GetD3dDevice();


		if (_vertexShader.find(shaderIndex) != _vertexShader.end())
			assert(false);

		assert(!_vertexShader[shaderIndex]);

		if (FAILED(::D3DCompileFromFile(fileName, macro, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryName, "vs_5_0", compileFlag, 0, &vertexShaderBuffer, &errorMessage)))
		{
			if (errorMessage)
				OutputShaderErrorMessage(errorMessage, nullptr, fileName);
			else
				::MessageBoxA(nullptr, "VS Shader Compile Failed ! PassBase..", nullptr, MB_OK);
		}

		if (FAILED(device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader[shaderIndex])))
		{
			if (errorMessage)
				OutputShaderErrorMessage(errorMessage, nullptr, fileName);
			else
				::MessageBoxA(nullptr, "VS Shader Create Failed ! PassBase..", nullptr, MB_OK);
		}

		if (FAILED(device->CreateInputLayout(polygonLayout, size,
			vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &_inputLayout[shaderIndex])))
		{
			if (errorMessage)
				OutputShaderErrorMessage(errorMessage, nullptr, fileName);
			else
				::MessageBoxA(nullptr, "InputLayout Create Failed ! PassBase..", nullptr, MB_OK);
		}



		vertexShaderBuffer->Release();

		if (errorMessage)
			errorMessage->Release();
	}

	template <typename T>
	void PassBase<T>::CompilePixelShader(const WCHAR* fileName, const CHAR* entryName, UINT shaderIndex, const D3D_SHADER_MACRO* macro)
	{
		ID3DBlob* pixelShaderBuffer = nullptr;
		ID3DBlob* errorMessage = nullptr;

		auto device = DXEngine::GetInstance()->GetD3dDevice();

		if (_pixelShader.find(shaderIndex) != _pixelShader.end())
			assert(false);

		if (FAILED(::D3DCompileFromFile(fileName, macro, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, entryName, "ps_5_0", compileFlag, 0, &pixelShaderBuffer, &errorMessage)))
		{
			if (errorMessage)
				OutputShaderErrorMessage(errorMessage, nullptr, fileName);
			else
				::MessageBoxA(nullptr, "PS Shader Compile Failed ! Shader..", nullptr, MB_OK);
		}

		if (FAILED(device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader[shaderIndex])))
		{
			if (errorMessage)
				OutputShaderErrorMessage(errorMessage, nullptr, fileName);
			else
				::MessageBoxA(nullptr, "PS Shader Create Failed ! Shader..", nullptr, MB_OK);
		}


		pixelShaderBuffer->Release();
		if (errorMessage)
			errorMessage->Release();
	}

	template <typename T>
	void PassBase<T>::CompileGeometryShader(const WCHAR* fileName, const CHAR* entryName, bool useStreamOut, UINT shaderIndex, const D3D_SHADER_MACRO* macro)
	{
		if (_geometryShader.find(shaderIndex) != _geometryShader.end())
			assert(false);

		ID3DBlob* geometryShader = nullptr;
		ID3DBlob* errorMessage = nullptr;

		auto device = DXEngine::GetInstance()->GetD3dDevice();

		if (FAILED(::D3DCompileFromFile(fileName, macro, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, entryName, "gs_5_0", compileFlag, 0, &geometryShader, &errorMessage)))
		{
			if (errorMessage)
				OutputShaderErrorMessage(errorMessage, nullptr, fileName);
			else
				::MessageBoxA(nullptr, "Geometry Create Failed !", nullptr, MB_OK);
		}

		if (useStreamOut)
		{
			constexpr int size = 14;

			//define the system output declaration entry, i.e. what will be written in the SO
			D3D11_SO_DECLARATION_ENTRY pDecl[size] =
			{
				//position, semantic name, semantic index, start component, component count, output slot
				{0,"POSITION", 0, 0, 3, 0 }, // output first 3 components of SPEED
				{0, "VELOCITY", 0, 0, 3, 0 }, // output first 3 components of "POSITION"
				{0, "SIZE", 0, 0, 2, 0 }, // output first 2 components of SIZE
				{0, "AGE", 0, 0, 1, 0 }, // output AGE
				{0, "TYPE",0, 0, 1, 0 }, // output TYPE
				{0, "VERTEXID",0, 0, 1, 0 }, // output TYPE
				{0, "LIFETIME",0, 0, 1, 0 }, // output TYPE
				{0, "ROTATION",0, 0, 1, 0 }, // output TYPE
				{0, "COLOR",0, 0, 4, 0 }, // output TYPE
				{0, "GRAVITY",0, 0, 1, 0 }, // output TYPE
				{0, "QUADTEX",0, 0, 2, 0 }, // output TYPE
				{0, "QUADTEX",1, 0, 2, 0 }, // output TYPE
				{0, "QUADTEX",2, 0, 2, 0 }, // output TYPE
				{0, "QUADTEX",3, 0, 2, 0 }, // output TYPE
			};

			UINT strides[1] = { sizeof(Vertex::Particle) };

			device->CreateGeometryShaderWithStreamOutput(geometryShader->GetBufferPointer(), geometryShader->GetBufferSize(), pDecl,
				size, strides, 1, D3D11_SO_NO_RASTERIZED_STREAM, NULL, &_geometryShader[shaderIndex]);
		}
		else
		{
			device->CreateGeometryShader(geometryShader->GetBufferPointer(), geometryShader->GetBufferSize(), NULL, &_geometryShader[shaderIndex]);
		}


	}

	template <typename T>
	void PassBase<T>::CreateConstantBuffer(UINT slot, UINT bufferSize/*sizeof(ConstantBuffDesc::CB_PerObject)*/)
	{
		auto device = DXEngine::GetInstance()->GetD3dDevice();

		// 정점 셰이더에 있는 행렬 상수 버퍼의 구조체를 작성합니다.
		D3D11_BUFFER_DESC matrixBufferDesc;
		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		matrixBufferDesc.ByteWidth = bufferSize;
		matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		matrixBufferDesc.MiscFlags = 0;
		matrixBufferDesc.StructureByteStride = 0;

		if (_constantBuffers.end() != _constantBuffers.find(slot))
		{
			::MessageBoxA(nullptr, "This Slot is not empty! Shader..", nullptr, MB_OK);
		}
		else
		{
			_constantBuffers[slot] = nullptr;
			// 상수 버퍼 포인터를 만들어 이 클래스에서 정점 셰이더 상수 버퍼에 접근할 수 있게 합니다.
			if (FAILED(device->CreateBuffer(&matrixBufferDesc, NULL, &_constantBuffers[slot])))
				::MessageBoxA(nullptr, "ConstatntBuffer Create Failed ! Shader..", nullptr, MB_OK);
		}
	}

	template <typename T>
	void PassBase<T>::SetDrawRectangle(float left, float right, float top, float bottom)
	{
		_left = left / DXEngine::GetInstance()->GetWidth() * 2 - 1;
		_right = right / DXEngine::GetInstance()->GetWidth() * 2 - 1;
		_top = -(top / DXEngine::GetInstance()->GetHeight() * 2 - 1);
		_bottom = -(bottom / DXEngine::GetInstance()->GetHeight() * 2 - 1);
	}

	template <typename T>
	template <class DATATYPE>
	void PassBase<T>::UpdateConstantBuffer(UINT slot, DATATYPE& data)
	{

		// 상수 버퍼의 내용을 쓸 수 있도록 잠급니다.
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		_d3dImmediateContext->Map(_constantBuffers[slot], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);


		// 상수 버퍼의 데이터에 대한 포인터를 가져옵니다.
		DATATYPE* dataPtr = static_cast<DATATYPE*>(mappedResource.pData);

		// 상수 버퍼에 행렬을 복사합니다.
		*dataPtr = data;

		// 상수 버퍼의 잠금을 풉니다.
		_d3dImmediateContext->Unmap(_constantBuffers[slot], 0);


		// 마지막으로 셰이더의 상수 버퍼를 바뀐 값으로 바꿉니다.
		_d3dImmediateContext->VSSetConstantBuffers(slot, 1, &_constantBuffers[slot]);

		_d3dImmediateContext->GSSetConstantBuffers(slot, 1, &_constantBuffers[slot]);

		_d3dImmediateContext->PSSetConstantBuffers(slot, 1, &_constantBuffers[slot]);
	}

	template <typename T>
	void PassBase<T>::OutputShaderErrorMessage(ID3DBlob* errorMessage, HWND hwnd, const WCHAR* shaderFileName)
	{
		char* compileErrors;
		unsigned long long bufferSize, i;
		std::ofstream fout;

		// Get a pointer to the error message text buffer.
		compileErrors = (char*)(errorMessage->GetBufferPointer());

		// Get the length of the message.
		bufferSize = errorMessage->GetBufferSize();

		// Open a file to write the error message to.
		fout.open("shader-error.txt");

		// Write out the error message.
		for (i = 0; i < bufferSize; i++)
		{
			fout << compileErrors[i];
		}

		// Close the file.
		fout.close();

		// Release the error message.
		errorMessage->Release();
		errorMessage = 0;

		// Pop a message up on the screen to notify the user to check the text file for compile errors.
		MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt for message.",
			shaderFileName, MB_OK);

		return;
	}

	template <typename T>
	void PassBase<T>::SetShader(UINT shaderIndex)
	{
		if (_inputLayout.find(shaderIndex) == _inputLayout.end())
			assert(false);

		_d3dImmediateContext->IASetInputLayout(_inputLayout[shaderIndex]);

		_d3dImmediateContext->IASetPrimitiveTopology(_topolgy);

		_d3dImmediateContext->VSSetShader(_vertexShader[shaderIndex], nullptr, 0);

		_d3dImmediateContext->GSSetShader(_geometryShader[shaderIndex], nullptr, 0);

		_d3dImmediateContext->PSSetShader(_pixelShader[shaderIndex], nullptr, 0);
	}
}
