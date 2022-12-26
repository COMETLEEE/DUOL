#pragma once
#include <vector>
#include <String>

#include "DUOLGraphicsEngine/Export.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Vertex.h"

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
			, _indices()
			, _indexBuffer(nullptr)
			, _materialID(0)
		{

		}

		//subMesh�� �ε���
		int _submeshIndex;

		int _parentMeshIndex;

		DUOLGraphicsLibrary::Buffer* _indexBuffer;

		std::vector<UINT32> _indices;

		DUOLGraphicsLibrary::UINT32 _drawIndex;

		DUOLGraphicsLibrary::UINT64 _materialID;

		//TODO ��ü�������� ����� ������ �༮�Դϴ� ���׸����� �̸��� ��� �ֽ��ϴ�.
		DUOLCommon::tstring _materialName;
	};

	struct Bone
	{
		std::wstring _boneName;

		int				_parentIndex;

		DUOLMath::Matrix _offsetMatrix = DirectX::XMMatrixIdentity();

		// �¿�������� ȸ���� ��Ű�����ؼ� �־��� ����̶�µ�..?
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
			, Particle
		};

	public:
		SubMesh* GetSubMesh(int meshIdx);

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
		std::vector<StaticMeshVertex> _vertices;

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

	public:
		std::vector<SKinnedMeshVertex> _vertices;

		virtual MeshType GetMeshType() override { return MeshType::SkinnedMesh; }
	};

	class DUOLGRAPHICSENGINE_EXPORT ParticleBuffer : public MeshBase
	{
	public:
		ParticleBuffer() :
			MeshBase()
		{
		}

		virtual ~ParticleBuffer() = default;

	public:
		DUOLGraphicsLibrary::Buffer* _streamOutBuffer;

		DUOLGraphicsLibrary::Buffer* _initBuffer;

		virtual MeshType GetMeshType() { return MeshType::Particle; }
	};

	//fbx������ �ε����� �� ����� �� ����
	// �� > �޽� > ����޽�
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
		bool IsSkinningModel() const
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

		// �������� �Լ�
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