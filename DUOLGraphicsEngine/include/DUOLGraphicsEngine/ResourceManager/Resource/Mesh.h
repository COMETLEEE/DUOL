#pragma once
#include <vector>
#include <String>

#include "DUOLGraphicsEngine/Export.h"

#include "DUOLCommon/StringHelper.h"
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
			, _indexBuffer(nullptr)
			, _materialID(0)
		{

		}

		//subMesh의 인덱스
		int _submeshIndex;

		int _parentMeshIndex;

		DUOLGraphicsLibrary::UINT32 _drawIndex;

		DUOLGraphicsLibrary::Buffer* _indexBuffer;

		DUOLGraphicsLibrary::UINT64 _materialID;
	};

	struct Bone
	{
		std::wstring _boneName;

		int				_parentIndex;

		DUOLMath::Matrix _offsetMatrix = DirectX::XMMatrixIdentity();

		// 좌우반전때매 회전을 시키기위해서 넣어준 행렬이라는데..?
		DUOLMath::Matrix _nodeMatrix = DirectX::XMMatrixIdentity();
	};

	class DUOLGRAPHICSENGINE_EXPORT MeshBase
	{
	protected:
		MeshBase():
			_subMeshCount(0)
			,_subMeshs()
			, _halfExtents()
			, _vertexBuffer(nullptr)
			, _meshName(TEXT(""))
		{
		}

	public:
		virtual ~MeshBase() = default;

		enum class MeshType
		{
			Mesh
			, SkinnedMesh
		};

	public:
		SubMesh* GetSubMesh(int MeshIdx);

		unsigned int GetSubMeshCount() const { return _subMeshCount; }

		virtual MeshType GetMeshType() abstract;

	public:
		unsigned int _subMeshCount;

		DUOLGraphicsLibrary::Buffer* _vertexBuffer;

		std::vector<SubMesh> _subMeshs;

		DUOLMath::Vector3 _halfExtents;

		DUOLCommon::tstring _meshName;
	};

	class DUOLGRAPHICSENGINE_EXPORT Mesh : public MeshBase
	{
	public:
		Mesh():
			MeshBase()
		{
		}

		~Mesh() override = default;

	public:
		virtual MeshType GetMeshType() override { return MeshType::Mesh; }
	};

	class DUOLGRAPHICSENGINE_EXPORT SkinnedMesh : public MeshBase
	{
	public:
		SkinnedMesh() :
			MeshBase()
		{
		}

		~SkinnedMesh() override = default;

		std::vector<Bone> _bones;

	public:
		virtual MeshType GetMeshType() override { return MeshType::SkinnedMesh; }

	};

	//fbx파일을 로드했을 때 생기는 모델 파일
	// 모델 > 메쉬 > 서브메쉬
	class DUOLGRAPHICSENGINE_EXPORT Model
	{

	public:
		Model():
			_meshCount(0)
			, _meshs()
			, _bones()
			, _isSkinningModel(false)
		{
		}

	public:
		bool GetisIsSkinningModel() const
		{
			return _isSkinningModel;
		}

		unsigned GetMeshCount() const
		{
			return _meshCount;
		}

		std::vector<Bone>& GetBones()
		{
			return _bones;
		}

		MeshBase* GetMesh(unsigned int MeshIdx) const;

		// 숨기고싶은 함수
		void SetIsSkinningModel(bool value);

		void SetMeshCount(int count);

		void AddMesh(MeshBase* mesh);

	private:
		bool _isSkinningModel;

		unsigned int _meshCount;

		std::vector<MeshBase*> _meshs;

		std::vector<Bone> _bones;
	};
}