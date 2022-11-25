#pragma once
#include "RenderConstantBuffer.h"

namespace DUOLGraphicsEngine
{
	struct Mesh;

	struct RenderObject
	{
		Mesh* _mesh;

		ConstantBufferPerObject _perObjectData;
	};
}