/**

    @file      PassBase.h
    @brief     /// PassBase를 상속 받은 Pass 제작 후 리소스 매니저에 패스 추가. Renderer에서 다형적으로 Draw동작 수형.
    @details   ~
    @author    SinSeongHyeon
    @date      9.11.2022
    @copyright © SinSeongHyeon, 2022. All right reserved.

**/
#pragma once

template <typename T>
class PassBase abstract
{
	enum class ShaderType
	{
		VertexShader,
		PixelShader
	};
public:
	PassBase(D3D11_PRIMITIVE_TOPOLOGY topology);

	virtual ~PassBase() = default;

protected:
	ID3D11DeviceContext* _d3dImmediateContext;

	ID3D11InputLayout* _inputLayout;

	D3D11_PRIMITIVE_TOPOLOGY _topolgy;

	ID3D11VertexShader* _vertexShader;

	ID3D10Blob* _shaderBuffer;
protected:
	virtual void SetConstants(std::shared_ptr<T>& renderingData) abstract;

public:
	virtual void Draw(std::shared_ptr<T>& renderingData) abstract;

	void CompileShader(ShaderType shaderType, WCHAR* fileName, CHAR* entryName );

	ID3D11VertexShader* SetInputLayout(D3D11_INPUT_ELEMENT_DESC polygonLayout[]);

};

template <typename T>
PassBase<T>::PassBase(D3D11_PRIMITIVE_TOPOLOGY topology) :
	_topolgy(topology), _inputLayout(nullptr),
	_d3dImmediateContext(DXEngine::GetInstance()->Getd3dImmediateContext()),
	_vertexShader(nullptr),
	_shaderBuffer(nullptr)

{

}

template <typename T>
void PassBase<T>::SetInputLayout(D3D11_INPUT_ELEMENT_DESC polygonLayout[])
{
	auto device = DXEngine::GetInstance()->GetD3dDevice();

	if (FAILED(device->CreateInputLayout(polygonLayout, sizeof(polygonLayout) / sizeof(polygonLayout[0]),
		_shaderBuffer->GetBufferPointer(), _shaderBuffer->GetBufferSize(), &_inputLayout)))
		::MessageBoxA(nullptr, "PS Shader Create Failed ! Shader..", nullptr, MB_OK);
}

