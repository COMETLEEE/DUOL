#pragma once
#include "RenderConstantBuffer.h"

namespace DUOLGraphicsEngine
{
	struct Mesh;

	struct RenderObject
	{
		Mesh* mesh;

		ConstantBufferPerObject PerObjectData;
	};
}