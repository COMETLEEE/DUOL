#pragma once
#include "d3d11.h"

namespace VertexDesc
{
	static constexpr UINT BasicVertexSize = 1;
	static D3D11_INPUT_ELEMENT_DESC BasicVertex[BasicVertexSize] =
	{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	static constexpr UINT DeferredVertexSize = 2;
	static D3D11_INPUT_ELEMENT_DESC DeferredVertexDesc[DeferredVertexSize] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV",    0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

}

namespace ConstantBuffDesc
{


	/**
	 * \brief ������Ʈ���� ����Ǵ� contant ���� ����ü, ������ �� �׻� ���̴� �ڵ嵵 ���� ��������. 16 ����Ʈ ���� ��������.
	 */
	struct CB_PerObject 
	{
		XMMATRIX worldViewProj;
	};

	struct CB_PerFream
	{
		DirectionalLight gDirLight;
		PointLight gPointLight[10];
		SpotLight gSpotLight;
		XMFLOAT3 gEyePosW;
		int gPointCount;
		Matrix gLightViewProj;
		Matrix gCurrentViewProj; // ���� ���� ��! �� �ʿ������?
		Matrix gPrevViewProj; // ���� ���� ��!
	};
}

