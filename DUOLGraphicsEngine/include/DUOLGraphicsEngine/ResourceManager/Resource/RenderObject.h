#pragma once
#include "RenderContantBuffer.h"

namespace DUOLGraphicsEngine
{
	struct Mesh;

	struct RenderObject
	{
		Mesh* mesh;

		ConstantBufferPerObject PerObjectData;
	};

}
