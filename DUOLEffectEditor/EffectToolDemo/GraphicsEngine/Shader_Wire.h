#pragma once
class Shader_Wire
{
public:
	Shader_Wire(wstring VS_Path, wstring PS_Path);

	void SetConstant(std::shared_ptr<RenderingData_3D>& renderingData);
	void Render(std::shared_ptr<RenderingData_3D>& renderingData);


	unsigned int _drawIndex;


	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;

	ID3D11InputLayout* _inputLayOut;
	ID3D11Buffer* _matrixBuffer;


};

