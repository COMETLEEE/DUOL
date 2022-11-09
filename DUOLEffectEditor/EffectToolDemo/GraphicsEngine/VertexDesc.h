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
	 * \brief ������Ʈ���� ����Ǵ� contant ���� ����ü, ������ �� �׻� ���̴� �ڵ嵵 ���� ��������.
	 */
	struct CB_PerObject 
	{
		XMMATRIX worldViewProj;
	};
}
