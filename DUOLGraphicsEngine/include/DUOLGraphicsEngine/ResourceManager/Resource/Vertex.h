#pragma once
#include "DUOLMath/DUOLMath.h"

namespace DUOLGraphicsEngine
{
	struct StaticMeshVertex
	{
		StaticMeshVertex() :
			_pos()
			, _normal()
			, _tangent()
			, _texcoord()
		{

		}

		StaticMeshVertex(DUOLMath::Vector3 pos, DUOLMath::Vector3 normal, DUOLMath::Vector3 tangent, DUOLMath::Vector2 texcoord) :
			_pos(pos)
			, _normal(normal)
			, _tangent(tangent)
			, _texcoord(texcoord)
		{

		}

		DUOLMath::Vector3 _pos;

		DUOLMath::Vector2 _texcoord;

		DUOLMath::Vector3 _normal;

		DUOLMath::Vector3 _tangent;
	};

	struct SKinnedMeshVertex : public  StaticMeshVertex
	{
		SKinnedMeshVertex() :
			StaticMeshVertex()
			, _boneWeight{ 0.f, 0.f,0.f, 0.f }
			, _bondIndex{ 0, 0, 0, 0 }
		{

		}

		SKinnedMeshVertex(DUOLMath::Vector3 pos, DUOLMath::Vector3 normal, DUOLMath::Vector3 tangent, DUOLMath::Vector2 texcoord) :
			StaticMeshVertex(pos, normal, tangent, texcoord)
			, _boneWeight{ 0.f, 0.f,0.f, 0.f }
			, _bondIndex{ 0, 0, 0, 0 }
		{

		}

		float _boneWeight[4];

		int _bondIndex[4];
	};

	using MeshIndex = uint32_t;
}
