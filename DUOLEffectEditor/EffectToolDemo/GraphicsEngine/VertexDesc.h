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
	 * \brief 오브젝트마다 공통되는 contant 버퍼 구조체, 수정할 때 항상 쉐이더 코드도 같이 수정하자. 16 바이트 정렬 잊지말자.
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
		Matrix gCurrentViewProj; // 블러를 위한 것! 엥 필요없었네?
		Matrix gPrevViewProj; // 블러를 위한 것!
	};
}

