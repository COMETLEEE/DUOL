#pragma once
#include <vector>
#include <String>

#include "DUOLGraphicsEngine/Export.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Vertex.h"

#include "DUOLCommon/StringHelper.h"
#include "DUOLGraphicsLibrary/Core/Typedef.h"
#include "DUOLMath/DUOLMath.h"

#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
typedef  unsigned __int64 uint64;

namespace SerializeData
{
	class Mesh;
	class Bone;
}

namespace DUOLGraphicsLibrary
{
	class PipelineState;
	class Buffer;
}

namespace DUOLGraphicsEngine
{
	class IRenderInfo;
	class SerializeMesh;
	class Material;

	struct DUOLGRAPHICSENGINE_EXPORT SubMesh
	{
		SubMesh() :
			_submeshIndex(0)
			, _parentMeshIndex(0)
			, _drawIndex(0)
			, _indices()
			, _indexBuffer(nullptr)
		{

		}

		//subMesh의 인덱스
		int _submeshIndex;

		int _parentMeshIndex;

		DUOLGraphicsLibrary::Buffer* _indexBuffer;

		std::vector<UINT32> _indices;

		DUOLGraphicsLibrary::UINT32 _drawIndex;

		DUOLCommon::tstring _materialName;
		//TODO:: 좀더 깔끔한 방법이 없을까 너무 어렵다..
		//std::unordered_map<Material*, std::vector<IRenderInfo*>> _renderInfos;
	};

	struct Bone
	{
		friend class boost::serialization::access;

		std::string _boneName;
		int			_parentIndex;

		DUOLMath::Matrix _offsetMatrix = DirectX::XMMatrixIdentity();

		// 좌우반전때매 회전을 시키기위해서 넣어준 행렬이라는데..?
		DUOLMath::Matrix _nodeMatrix = DirectX::XMMatrixIdentity();

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _boneName;
			ar& _parentIndex;
			ar& _offsetMatrix;
			ar& _nodeMatrix;
		}
	};

	class DUOLGRAPHICSENGINE_EXPORT MeshBase
	{
	protected:
		MeshBase() :
			_subMeshCount(0)
			, _subMeshs()
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
		Mesh() :
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

		virtual MeshType GetMeshType() { return MeshType::Particle; }
	};

	//fbx파일을 로드했을 때 생기는 모델 파일
	// 모델 > 메쉬 > 서브메쉬
	class DUOLGRAPHICSENGINE_EXPORT Model
	{

	public:
		Model() :
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

		std::vector<SerializeMesh>& GetSerializeMesh()
		{
			return _serializemeshs;
		}

		MeshBase* GetMesh(unsigned int MeshIdx) const;

		MeshBase* GetMeshByName(const DUOLCommon::tstring& name) const;

		uint64 GetModelID()
		{
			return _modelID;
		}

		// 숨기고싶은 함수
		void SetIsSkinningModel(bool value);

		void SetMeshCount(int count);

		void AddMesh(MeshBase* mesh);

		void AddMeshWithName(const DUOLCommon::tstring& meshName, MeshBase* mesh);

	private:
		unsigned int _meshCount;

		std::vector<MeshBase*> _meshs;

		std::unordered_map<DUOLCommon::tstring, MeshBase*> _meshNameMap;







		// 시리얼라이즈가 완료되면 .. 위의 것들이 완성되어 있는 것인가요 >?
	protected:
		friend class boost::serialization::access;

		uint64 _modelID;

		std::vector<SerializeMesh> _serializemeshs;

		std::vector<DUOLGraphicsEngine::Bone> _bones;

		bool _isSkinningModel;

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _modelID;

			ar& _serializemeshs;
			ar& _bones;
			ar& _isSkinningModel;
		}
	};

	// Serialize Mesh데이터를 담을 클래스

	class SerializeMesh
	{
	public:
		friend class boost::serialization::access;

		//int				meshIndex = 0;		// Mesh Index

		std::string		nodeName;		// 노드 이름
		std::string		parentName;		// 부모 이름(부모 이름있으면 부모 O)
		bool			isParent;		// 부모가 있는지 확실하게 체크
		bool			isSkinned = false;		// 스키닝 메쉬인지

		std::vector<std::vector<unsigned int>>	indices;		// 만약 Mesh가 쪼개져 있으면 index를 쪼개준다. 

		DUOLMath::Matrix						nodeTM;

		std::vector<Vertex>						vertexList;		// 이 Mesh의 Vertex 정보

		std::vector<SerializeMesh>				childList;		// 자식들을 넣어준다

		std::vector<std::string>				materialName;	// 이 Mesh의 material 정보
		std::vector<unsigned int>				materialIndex;

		DUOLMath::Vector3 halfExtent;

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			//ar& meshIndex;

			ar& nodeName;
			ar& parentName;
			ar& isParent;
			ar& isSkinned;

			ar& indices;
			ar& nodeTM;

			ar& vertexList;

			ar& childList;

			ar& materialName;
			ar& materialIndex;

			ar& halfExtent;
		}
	};
}