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
		// Ŭ������ �� �Լ��� ������ �� �ֵ��� �ش� Ŭ������ friend�� ����
		friend class boost::serialization::access;

		DUOLMath::Vector3		position;		// ��ġ��
		DUOLMath::Vector2		uv;				// �ؽ��� ��ǥ
		DUOLMath::Vector3		normal;			// �븻��
		DUOLMath::Vector3		tangent;		// ź��Ʈ

		// Skinning
		int boneIndices[8];
		float boneWeight[8];

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			// ������ �����Ǹ� �ѵ�
			ar& position;
			ar& uv;
			ar& normal;
			ar& tangent;

			ar& boneIndices;
			ar& boneWeight;
		}

	};
}
