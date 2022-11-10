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


	// ���� ���̴��� �ִ� ��� ��� ������ ����ü�� �ۼ��մϴ�.
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(ConstantBuffDesc::CB_PerObject);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// ��� ���� �����͸� ����� �� Ŭ�������� ���� ���̴� ��� ���ۿ� ������ �� �ְ� �մϴ�.
	if (FAILED(_device->CreateBuffer(&matrixBufferDesc, NULL, &_matrixBuffer)))
		::MessageBoxA(nullptr, "PS Shader Create Failed ! Shader..", nullptr, MB_OK);

}

void Shader_Wire::SetConstant(std::shared_ptr<RenderingData_3D>& renderingData)
{
	auto dc = DXEngine::GetInstance()->Getd3dImmediateContext();
	auto& perfreamData = Renderer::GetPerfreamData();

	Matrix test = renderingData->_geoInfo->_world * perfreamData->_cameraInfo->_viewMatrix * perfreamData->_cameraInfo->_projMatrix;


	// ��� ������ ������ �� �� �ֵ��� ��޴ϴ�.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	dc->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);


	// ��� ������ �����Ϳ� ���� �����͸� �����ɴϴ�.
	ConstantBuffDesc::CB_PerObject* dataPtr = (ConstantBuffDesc::CB_PerObject*)mappedResource.pData;

	// ��� ���ۿ� ����� �����մϴ�.
	dataPtr->worldViewProj = test;


	// ��� ������ ����� Ǳ�ϴ�.
	dc->Unmap(_matrixBuffer, 0);

	// ���� ���̴������� ��� ������ ��ġ�� �����մϴ�.
	unsigned bufferNumber = 0;

	// ���������� ���� ���̴��� ��� ���۸� �ٲ� ������ �ٲߴϴ�.
	dc->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);

	// �ε������ۿ� ���ؽ����� ����
	UINT stride = sizeof(Vertex::Basic);
	UINT offset = 0;

	auto vbibMesh = DXEngine::GetInstance()->GetResourceManager()->GetVBIBMesh(renderingData->_objectInfo->_meshID);

	ID3D11DeviceContext* _d3dImmediateContext = DXEngine::GetInstance()->Getd3dImmediateContext();
	_drawIndex = vbibMesh->GetIndexSize();
	// �Է� ��ġ ��ü ����
	_d3dImmediateContext->IASetVertexBuffers(0, 1, vbibMesh->GetVB(), &stride, &offset); //���ؽ� ����
	_d3dImmediateContext->IASetIndexBuffer(*vbibMesh->GetIB(), DXGI_FORMAT_R32_UINT, 0); //�ε��� ����





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
