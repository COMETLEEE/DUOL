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

#include "DXEngine.h"

#if defined(DEBUG) || defined(_DEBUG)
static uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#else
static uint32 compileFlag = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#endif

template <typename T>
class PassBase abstract
{
public:
	PassBase(D3D11_PRIMITIVE_TOPOLOGY topology);

	virtual ~PassBase();

private:
	ID3D11InputLayout* _inputLayout;

	D3D11_PRIMITIVE_TOPOLOGY _topolgy;

protected:
	ID3D11DeviceContext* _d3dImmediateContext;

	ID3D11VertexShader* _vertexShader;

	ID3D11PixelShader* _pixelShader;

	ID3D11GeometryShader* _geometryShader;

protected:
	/**
	 * \brief 오버라이딩된 Draw 함수에서 실행시켜야 하는 함수들.
	 */
	void SetShaer();

	virtual void SetConstants(T& renderingData) abstract;

	/**
	* \brief 다형적 동작으로 구현 하려고 했으나, 모든 함수를 정의 하는 것이 더 사용하기 편하고, 직관적인듯 하다.
	*/
	void CompileVertexShader(const WCHAR* fileName, const CHAR* entryName, D3D11_INPUT_ELEMENT_DESC polygonLayout[], UINT size);

	void CompilePixelShader(const WCHAR* fileName, const CHAR* entryName);

	void CompileGeometryShader(const WCHAR* fileName, const CHAR* entryName);

	void CreateConstantBuffer(ID3D11Buffer** buffer, UINT bufferSize);

public:
	virtual void Draw(T& renderingData) abstract;
};

template <typename T>
PassBase<T>::PassBase(D3D11_PRIMITIVE_TOPOLOGY topology) :
	_topolgy(topology), _inputLayout(nullptr),
	_d3dImmediateContext(DXEngine::GetInstance()->Getd3dImmediateContext()),
	_vertexShader(nullptr),
	_pixelShader(nullptr),
	_geometryShader(nullptr)

{

}

template <typename T>
PassBase<T>::~PassBase()
{
	if (_vertexShader)
		_vertexShader->Release();
	if (_pixelShader)
		_pixelShader->Release();
	if (_geometryShader)
		_geometryShader->Release();
}

template <typename T>
void PassBase<T>::CompileVertexShader(const WCHAR* fileName, const CHAR* entryName,
	D3D11_INPUT_ELEMENT_DESC polygonLayout[],UINT size)
{
	ID3DBlob* errorMessage = nullptr;
	ID3DBlob* vertexShaderBuffer = nullptr;

	auto device = DXEngine::GetInstance()->GetD3dDevice();


	if (FAILED(::D3DCompileFromFile(fileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryName, "vs_5_0", compileFlag, 0, &vertexShaderBuffer, &errorMessage)))
		::MessageBoxA(nullptr, "VS Shader Create Failed ! PassBase..", nullptr, MB_OK);

	if (FAILED(device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader)))
		::MessageBoxA(nullptr, "VS Shader Create Failed ! PassBase..", nullptr, MB_OK);

	if (FAILED(device->CreateInputLayout(polygonLayout, size,
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &_inputLayout)))
		::MessageBoxA(nullptr, "InputLayout Create Failed ! PassBase..", nullptr, MB_OK);



	vertexShaderBuffer->Release();

	if (errorMessage)
		errorMessage->Release();
}

template <typename T>
void PassBase<T>::CompilePixelShader(const WCHAR* fileName, const CHAR* entryName)
{
	ID3DBlob* pixelShaderBuffer = nullptr;
	ID3DBlob* errorMessage = nullptr;

	auto device = DXEngine::GetInstance()->GetD3dDevice();

	if (FAILED(::D3DCompileFromFile(fileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, entryName, "ps_5_0", compileFlag, 0, &pixelShaderBuffer, &errorMessage)))
		::MessageBoxA(nullptr, "PS Shader Compile Failed ! Shader..", nullptr, MB_OK);

	if (FAILED(device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader)))
		::MessageBoxA(nullptr, "PS Shader Create Failed ! Shader..", nullptr, MB_OK);

	pixelShaderBuffer->Release();
	if (errorMessage)
		errorMessage->Release();
}

template <typename T>
void PassBase<T>::CompileGeometryShader(const WCHAR* fileName, const CHAR* entryName)
{
}

template <typename T>
void PassBase<T>::CreateConstantBuffer(ID3D11Buffer** buffer, UINT bufferSize/*sizeof(ConstantBuffDesc::CB_PerObject)*/)
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

	// 상수 버퍼 포인터를 만들어 이 클래스에서 정점 셰이더 상수 버퍼에 접근할 수 있게 합니다.
	if (FAILED(device->CreateBuffer(&matrixBufferDesc, NULL, buffer)))
		::MessageBoxA(nullptr, "PS Shader Create Failed ! Shader..", nullptr, MB_OK);
}

template <typename T>
void PassBase<T>::SetShaer()
{
	assert(_inputLayout);

	_d3dImmediateContext->IASetInputLayout(_inputLayout);

	_d3dImmediateContext->IASetPrimitiveTopology(_topolgy);

	if (_vertexShader)
		_d3dImmediateContext->VSSetShader(_vertexShader, nullptr, 0);
	if (_geometryShader)
		_d3dImmediateContext->GSSetShader(_geometryShader, nullptr, 0);
	if (_pixelShader)
		_d3dImmediateContext->PSSetShader(_pixelShader, nullptr, 0);
}
