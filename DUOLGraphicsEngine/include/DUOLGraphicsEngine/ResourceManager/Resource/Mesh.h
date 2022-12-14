#pragma once
#include <vector>

#include "DUOLGraphicsLibrary/Core/Typedef.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLGraphicsLibrary
{
	class PipelineState;
	class Buffer;
}

namespace DUOLGraphicsEngine
{
	class Material;

	struct SubMesh
	{
		SubMesh() :
			_submeshIndex(0)
			, _parentMeshIndex(0)
			, _drawIndex(0)
			, _vertexBuffer(nullptr)
			, _indexBuffer(nullptr)
			, _materialID(0)
		{

		}

		//subMesh의 인덱스
		int _submeshIndex;

		int _parentMeshIndex;

		DUOLGraphicsLibrary::UINT32 _drawIndex;

		DUOLGraphicsLibrary::Buffer* _vertexBuffer;

		DUOLGraphicsLibrary::Buffer* _indexBuffer;

		DUOLGraphicsLibrary::UINT64 _materialID;
	};

	struct Bone
	{
		int					parentIndex;

		DUOLMath::Matrix offsetMatrix = DirectX::XMMatrixIdentity();

		// 좌우반전때매 회전을 시키기위해서 넣어준 행렬이라는데..?
		DUOLMath::Matrix nodeMatrix = DirectX::XMMatrixIdentity();
	};

	class MeshBase
	{
	protected:
		MeshBase() = default;

	public:
		virtual ~MeshBase() = default;

		enum class MeshType
		{
			Mesh
			, SkinnedMesh
		};

	public:
		SubMesh* GetSubMesh(int MeshIdx){ return nullptr; };

		unsigned int GetSubMeshCount() const { return _subMeshCount; }

		virtual MeshType GetMeshType() abstract;

	public:
		unsigned int _subMeshCount;

		std::vector<SubMesh> _subMesh;

		DUOLMath::Vector3 _halfExtents;
	};

	class Mesh : public MeshBase
	{
	public:
		Mesh() = default;

		~Mesh() override = default;

	public:
		virtual MeshType GetMeshType() override { return MeshType::Mesh; }

	};

	class SkinnedMesh : public MeshBase
	{
	public:
		SkinnedMesh() = default;

		~SkinnedMesh() override = default;

	public:
		virtual MeshType GetMeshType() override { return MeshType::SkinnedMesh; }

		std::vector<Bone> _bones;
	};
}