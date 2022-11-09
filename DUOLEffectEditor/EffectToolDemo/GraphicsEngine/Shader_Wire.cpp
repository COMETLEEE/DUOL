#include "pch.h"
#include "Shader_Wire.h"

#include <d3dcompiler.h>

#include "VertexDesc.h"

Shader_Wire::Shader_Wire(wstring VS_Path, wstring PS_Path)
{


#if defined(DEBUG) || defined(_DEBUG)
	uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#else
	uint32 compileFlag = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#endif



	ID3DBlob* errorMessage = nullptr;
	ID3DBlob* vertexShaderBuffer = nullptr;
	if (FAILED(::D3DCompileFromFile(VS_Path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
	"main", "vs_5_0", compileFlag, 0, &vertexShaderBuffer, &errorMessage)))
	{
		//::MessageBoxA(nullptr, "VS Shader Compile Failed !  Shader..", nullptr, MB_OK);
		OutputDebugStringA(reinterpret_cast<const char*>(errorMessage->GetBufferPointer()));
	}

	ID3DBlob* pixelShaderBuffer = nullptr;

	if (FAILED(::D3DCompileFromFile(PS_Path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, "main", "ps_5_0", compileFlag, 0, &pixelShaderBuffer, &errorMessage)))
		::MessageBoxA(nullptr, "PS Shader Compile Failed ! Shader..", nullptr, MB_OK);


	auto _device = DXEngine::GetInstance()->GetD3dDevice();





	if (FAILED(_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader)))
		::MessageBoxA(nullptr, "VS Shader Create Failed ! Shader..", nullptr, MB_OK);

	if (FAILED(_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader)))
		::MessageBoxA(nullptr, "PS Shader Create Failed ! Shader..", nullptr, MB_OK);

		



	vertexShaderBuffer->Release();
	pixelShaderBuffer->Release();


	// 정점 셰이더에 있는 행렬 상수 버퍼의 구조체를 작성합니다.
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(ConstantBuffDesc::CB_PerObject);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// 상수 버퍼 포인터를 만들어 이 클래스에서 정점 셰이더 상수 버퍼에 접근할 수 있게 합니다.
	if (FAILED(_device->CreateBuffer(&matrixBufferDesc, NULL, &_matrixBuffer)))
		::MessageBoxA(nullptr, "PS Shader Create Failed ! Shader..", nullptr, MB_OK);

}

void Shader_Wire::SetConstant(std::shared_ptr<RenderingData_3D>& renderingData)
{
	auto dc = DXEngine::GetInstance()->Getd3dImmediateContext();
	auto& perfreamData = Renderer::GetPerfreamData();

	Matrix test = renderingData->_geoInfo->_world * perfreamData->_cameraInfo->_viewMatrix * perfreamData->_cameraInfo->_projMatrix;


	// 상수 버퍼의 내용을 쓸 수 있도록 잠급니다.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	dc->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);


	// 상수 버퍼의 데이터에 대한 포인터를 가져옵니다.
	ConstantBuffDesc::CB_PerObject* dataPtr = (ConstantBuffDesc::CB_PerObject*)mappedResource.pData;

	// 상수 버퍼에 행렬을 복사합니다.
	dataPtr->worldViewProj = test;


	// 상수 버퍼의 잠금을 풉니다.
	dc->Unmap(_matrixBuffer, 0);

	// 정점 셰이더에서의 상수 버퍼의 위치를 설정합니다.
	unsigned bufferNumber = 0;

	// 마지막으로 정점 셰이더의 상수 버퍼를 바뀐 값으로 바꿉니다.
	dc->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);

	// 인덱스버퍼와 버텍스버퍼 셋팅
	UINT stride = sizeof(Vertex::Basic);
	UINT offset = 0;

	auto vbibMesh = DXEngine::GetInstance()->GetResourceManager()->GetVBIBMesh(renderingData->_objectInfo->_meshID);

	ID3D11DeviceContext* _d3dImmediateContext = DXEngine::GetInstance()->Getd3dImmediateContext();
	_drawIndex = vbibMesh->GetIndexSize();
	// 입력 배치 객체 셋팅
	_d3dImmediateContext->IASetVertexBuffers(0, 1, vbibMesh->GetVB(), &stride, &offset); //버텍스 버퍼
	_d3dImmediateContext->IASetIndexBuffer(*vbibMesh->GetIB(), DXGI_FORMAT_R32_UINT, 0); //인덱스 버퍼





}

void Shader_Wire::Render(std::shared_ptr<RenderingData_3D>& renderingData)
{
	SetConstant(renderingData);

	auto dc = DXEngine::GetInstance()->Getd3dImmediateContext();

	dc->IASetInputLayout(_inputLayOut);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	dc->VSSetShader(_vertexShader, nullptr, 0);
	dc->PSSetShader(_pixelShader, nullptr, 0);

	DXEngine::GetInstance()->GetDepthStencil()->OnDepthStencil(0);
	//dc->DrawAuto();
	dc->DrawIndexed(_drawIndex, 0, 0);


}
