#include "DUOLGameEngine//Util/Octree.h"

#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/MeshFilter.h"
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"

#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"
#include "DUOLGameEngine/Util/Geometries.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"

namespace DUOLGameEngine
{
	Octree::Octree(const DUOLMath::Vector3& origin, const DUOLMath::Vector3& halfExtents, int depth) :
		_origin(origin)
		, _halfExtents(halfExtents)
		, _datas{}
		, _currentDepth(depth)
		, _parent(nullptr)
	{
		for (int i = 0; i < 8; i++)
			_children[i] = nullptr;
	}

	Octree::~Octree()
	{
		for (int i = 0; i < 8; i++)
			delete _children[i];
	}

	Octree* Octree::BuildOctree(DUOLGameEngine::Scene* scene)
	{
		DUOL_INFO(DUOL_CONSOLE, "Build Octree Start !");

		DUOLMath::Vector3 min{ FLT_MAX, FLT_MAX, FLT_MAX };
		DUOLMath::Vector3 max { FLT_MIN, FLT_MIN, FLT_MIN };

		std::vector<OctreeData> octreeDatas {};

		auto gameObjects = scene->GetAllGameObjects();

		for (auto gameObject: gameObjects)
		{
			// TODO : 스태틱 게임 오브젝트에 대해서만 진행합니다.
			// if (gameObject->GetIsStatic())
			{
				auto meshFilter = gameObject->GetComponent<DUOLGameEngine::MeshFilter>();

				auto meshRenderer = gameObject->GetComponent<DUOLGameEngine::MeshRenderer>();

				if ((meshFilter == nullptr) || (meshRenderer == nullptr))
					continue;

				auto primitiveMesh = meshFilter->GetMesh()->GetPrimitiveMesh();

				if (primitiveMesh != nullptr)
				{
					const DUOLMath::Vector3& halfExtents = primitiveMesh->_halfExtents;

					const DUOLMath::Vector3& center = primitiveMesh->_center;

					OctreeData octreeData{  gameObject->GetTransform(), &meshRenderer->_renderObjectInfo, halfExtents, center };

					octreeDatas.push_back(octreeData);

					const DUOLMath::Vector3& position = octreeData._transform->GetWorldPosition();

					const DUOLMath::Matrix& worldMatrix = octreeData._transform->GetWorldMatrix();

					DUOLMath::Vector4 scaledxV4 = DUOLMath::Vector4::Transform(DUOLMath::Vector4{ octreeData._halfExtents.x, 0, 0, 0 }, worldMatrix);
					DUOLMath::Vector4 scaledyV4 = DUOLMath::Vector4::Transform(DUOLMath::Vector4{ 0.f, octreeData._halfExtents.y, 0.f, 0.f }, worldMatrix);
					DUOLMath::Vector4 scaledzV4 = DUOLMath::Vector4::Transform(DUOLMath::Vector4{ 0.f, octreeData._halfExtents.z, 0.f, 0.f }, worldMatrix);
					DUOLMath::Vector4 scaledCenter = DUOLMath::Vector4::Transform(DUOLMath::Vector4{ center.x, center.y ,center.z, 1.f }, worldMatrix);

					DUOLMath::Vector3 scaledx{ scaledxV4 };
					DUOLMath::Vector3 scaledy{ scaledyV4 };
					DUOLMath::Vector3 scaledz{ scaledzV4 };

					// 월드 좌표계에서 점을 나타내기 위한 각 박스 정점으로의 이동량
					float x = std::fabs(scaledx.Dot(DUOLMath::Vector3::Right)) + fabs(scaledy.Dot(DUOLMath::Vector3::Right)) + fabs(scaledz.Dot(DUOLMath::Vector3::Right));
					float y = std::fabs(scaledx.Dot(DUOLMath::Vector3::Up)) + fabs(scaledy.Dot(DUOLMath::Vector3::Up)) + fabs(scaledz.Dot(DUOLMath::Vector3::Up));
					float z = std::fabs(scaledx.Dot(DUOLMath::Vector3::Forward)) + fabs(scaledy.Dot(DUOLMath::Vector3::Forward)) + fabs(scaledz.Dot(DUOLMath::Vector3::Forward));

					min.x = std::min(min.x, scaledCenter.x - x);
					min.y = std::min(min.y, scaledCenter.y - y);
					min.z = std::min(min.z, scaledCenter.z - z);

					max.x = std::max(max.x, scaledCenter.x + x);
					max.y = std::max(max.y, scaledCenter.y + y);
					max.z = std::max(max.z, scaledCenter.z + z);
				}
			}
		}

		float x = std::fabs(max.x - min.x) * 0.5f;
		float y = std::fabs(max.y - min.y) * 0.5f;
		float z = std::fabs(max.z - min.z) * 0.5f;

		float halfExtents = std::max(std::max(x, y), z);

		Octree* topNode = new Octree(DUOLMath::Vector3{ max.x + min.x, max.y + min.y, max.z + min.z } *0.5f
			, DUOLMath::Vector3{ halfExtents }
			,0);

		for (auto octreeData : octreeDatas)
			topNode->Insert(octreeData);

		DUOL_INFO(DUOL_CONSOLE, "Build Octree Success !");

		return topNode;
	}

	bool Octree::IsLeafNode()
	{
		return _children[0] == nullptr;
	}

	void Octree::Insert(OctreeData& octreeData)
	{
		// 꼬리 노드이고
		if (IsLeafNode())
		{
			// 아직 데이터가 없다면
			if (_datas.empty())
			{
				// 넣어주고 끝낸다. (=> 더 쪼갤 필요가 없으니까 ..!)
				_datas.push_back(octreeData);
			}
			else
			{
				if (_currentDepth == OCTREE_MAX_DEPTH)
				{
					_datas.push_back(octreeData);

					return;
				}

				// 그냥 해당 노드를 일단 쪼개버린다. (있던 오브젝트들 또는 다음에 들어올 오브젝트들이 각각의 노드에 들어갈 확률이 높으니까)
				for (int i = 0; i < 8; i++)
				{
					DUOLMath::Vector3 newOrigin = _origin;
					newOrigin.x += _halfExtents.x * (i & 4 ? .5f : -.5f);
					newOrigin.y += _halfExtents.y * (i & 2 ? .5f : -.5f);
					newOrigin.z += _halfExtents.z * (i & 1 ? .5f : -.5f);

					Octree* child = new Octree(newOrigin, _halfExtents * 0.5f, _currentDepth + 1);

					child->_parent = this;

					_children[i] = child;
				}

				int childNodeIndex = GetChildNodeOfContainingData(octreeData);

				childNodeIndex != 8 ? _children[childNodeIndex]->Insert(octreeData) : _datas.push_back(octreeData);

				// 노드가 쪼개졌으니까 기존의 데이터를 흩뿌려주자
				std::erase_if(_datas, [this](OctreeData& data)
				{
						int childNodeIndex = GetChildNodeOfContainingData(data);

						if (childNodeIndex != 8)
						{
							_children[childNodeIndex]->Insert(data);

							return true;
						}
						else
							return false;
				});
			}
		}
		// 꼬리 노드가 아니다. 구하자.
		else
		{
			int childNodeIndex = GetChildNodeOfContainingData(octreeData);

			childNodeIndex != 8 ? _children[childNodeIndex]->Insert(octreeData) : _datas.push_back(octreeData);
		}
	}

	int Octree::GetChildNodeOfContainingData(OctreeData& octreeData)
	{
		const DUOLMath::Vector3& position = octreeData._transform->GetWorldPosition();

		const DUOLMath::Matrix& worldMatrix = octreeData._transform->GetWorldMatrix();

		DUOLMath::Vector4 scaledxV4 = DUOLMath::Vector4::Transform(DUOLMath::Vector4{ octreeData._halfExtents.x, 0, 0, 0 }, worldMatrix);
		DUOLMath::Vector4 scaledyV4 = DUOLMath::Vector4::Transform(DUOLMath::Vector4{ 0.f, octreeData._halfExtents.y, 0.f, 0.f }, worldMatrix);
		DUOLMath::Vector4 scaledzV4 = DUOLMath::Vector4::Transform(DUOLMath::Vector4{ 0.f, 0.f,octreeData._halfExtents.z,0.f }, worldMatrix);
		DUOLMath::Vector4 scaledCenterV4 = DUOLMath::Vector4::Transform(DUOLMath::Vector4{ octreeData._center.x, octreeData._center.y,octreeData._center.z,1.f }, worldMatrix);

		DUOLMath::Vector3 scaledx{ scaledxV4 };
		DUOLMath::Vector3 scaledy{ scaledyV4 };
		DUOLMath::Vector3 scaledz{ scaledzV4 };
		DUOLMath::Vector3 scaledCenter{ scaledCenterV4 };

		// 월드 좌표계에서 점을 나타내기 위한 각 박스 정점으로의 이동량
		float x = std::fabs(scaledx.Dot(DUOLMath::Vector3::Right)) + fabs(scaledy.Dot(DUOLMath::Vector3::Right)) + fabs(scaledz.Dot(DUOLMath::Vector3::Right));
		float y = std::fabs(scaledx.Dot(DUOLMath::Vector3::Up)) + fabs(scaledy.Dot(DUOLMath::Vector3::Up)) + fabs(scaledz.Dot(DUOLMath::Vector3::Up));
		float z = std::fabs(scaledx.Dot(DUOLMath::Vector3::Forward)) + fabs(scaledy.Dot(DUOLMath::Vector3::Forward)) + fabs(scaledz.Dot(DUOLMath::Vector3::Forward));
		
		// 자식 노드의 바운딩 박스의 크기
		DUOLMath::Vector3 newHalfExtents = _halfExtents * 0.5f;

		for (int i = 0 ; i < 8 ; i++)
		{
			bool flag = true;

			DUOLMath::Vector3 newOrigin = _origin;
			newOrigin.x += _halfExtents.x * (i & 4 ? .5f : -.5f);
			newOrigin.y += _halfExtents.y * (i & 2 ? .5f : -.5f);
			newOrigin.z += _halfExtents.z * (i & 1 ? .5f : -.5f);

			DUOLMath::Vector3 min = DUOLMath::Vector3{ newOrigin.x - newHalfExtents.x , newOrigin.y - newHalfExtents.y, newOrigin.z - newHalfExtents.z };
			DUOLMath::Vector3 max = DUOLMath::Vector3{ newOrigin.x + newHalfExtents.x , newOrigin.y + newHalfExtents.y, newOrigin.z + newHalfExtents.z };

			// 데이터를 감싸는 박스, 8개의 점이 해당 노드에 들어갈 수 있다면 반환하자.
			for (int j = 0 ; j < 8 ; j++)
			{
				DUOLMath::Vector3 point = scaledCenter;

				point.x += (j & 4 ? x : -x);
				point.y += (j & 2 ? y : -y);
				point.z += (j & 1 ? z : -z);

				// 들어간다.
				if (((min.x <= point.x) && (point.x <= max.x)) && ((min.y <= point.y) && (point.y <= max.y)) && ((min.z <= point.z) && (point.z <= max.z)))
					continue;
				else
				{
					flag = false;

					break;
				}
			}

			// 해당 차일드 노드에 들어갈 수 있다고 체크되면, 해당 차일드 노드의 인덱스를 반환합니다.
			if (flag)
				return i;
		}

		// 어떠한 차일드 노드에도 들어갈 수 없습니다.
		return 8;
	}

	void Octree::ViewFrustumCullingAllNodes(DUOLGameEngine::Frustum& frustum, std::unordered_map<void*, bool>& outDatas)
	{
		if (GeometryHelper::ViewFrustumCullingAABB(_origin, _halfExtents, frustum))
		{
			for (auto data : _datas)
				outDatas.insert({ data._renderObjectInfo, true });

			// 자식 노드 중 일부는 맞겠지만 .. 일부는 안 맞을 수도 있다 ..
			if (!IsLeafNode())
				for (auto child : _children)
					child->ViewFrustumCullingAllNodes(frustum, outDatas);
		}

		// 부모 노드가 맞지 않았으면 .. 자식 노드는 당연히 안 맞는다.
	}
}