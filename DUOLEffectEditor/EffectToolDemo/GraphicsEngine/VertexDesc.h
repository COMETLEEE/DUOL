#pragma once
#include "d3d11.h"

namespace VertexDesc
{
	static D3D11_INPUT_ELEMENT_DESC BasicVertex[] =
	{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
	};


}

namespace ConstantBuffDesc
{
	/**
	 * \brief 오브젝트마다 공통되는 contant 버퍼 구조체, 수정할 때 항상 쉐이더 코드도 같이 수정하자.
	 */
	struct CB_PerObject 
	{
		XMMATRIX worldViewProj;
	};
}
