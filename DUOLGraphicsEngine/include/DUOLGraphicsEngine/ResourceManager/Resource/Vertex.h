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

		float _boneWeight[8];

		int _bondIndex[8];
	};

	using MeshIndex = uint32_t;

	// Serialize Data
	class Vertex
	{
	protected:
		// 클래스가 이 함수에 접근할 수 있도록 해당 클래스를 friend로 선언
		friend class boost::serialization::access;

		DUOLMath::Vector3		position;		// 위치값
		DUOLMath::Vector2		uv;				// 텍스쳐 좌표
		DUOLMath::Vector3		normal;			// 노말값
		DUOLMath::Vector3		tangent;		// 탄젠트

		// Skinning
		int boneIndices[8];
		float boneWeight[8];

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			// 연산자 재정의를 한듯
			ar& position;
			ar& uv;
			ar& normal;
			ar& tangent;

			ar& boneIndices;
			ar& boneWeight;
		}

	};
}
