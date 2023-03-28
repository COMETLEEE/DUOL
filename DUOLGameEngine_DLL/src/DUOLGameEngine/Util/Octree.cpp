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
			// TODO : ����ƽ ���� ������Ʈ�� ���ؼ��� �����մϴ�.
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

					// ���� ��ǥ�迡�� ���� ��Ÿ���� ���� �� �ڽ� ���������� �̵���
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
		// ���� ����̰�
		if (IsLeafNode())
		{
			// ���� �����Ͱ� ���ٸ�
			if (_datas.empty())
			{
				// �־��ְ� ������. (=> �� �ɰ� �ʿ䰡 �����ϱ� ..!)
				_datas.push_back(octreeData);
			}
			else
			{
				if (_currentDepth == OCTREE_MAX_DEPTH)
				{
					_datas.push_back(octreeData);

					return;
				}

				// �׳� �ش� ��带 �ϴ� �ɰ�������. (�ִ� ������Ʈ�� �Ǵ� ������ ���� ������Ʈ���� ������ ��忡 �� Ȯ���� �����ϱ�)
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

				// ��尡 �ɰ������ϱ� ������ �����͸� ��ѷ�����
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
		// ���� ��尡 �ƴϴ�. ������.
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

		// ���� ��ǥ�迡�� ���� ��Ÿ���� ���� �� �ڽ� ���������� �̵���
		float x = std::fabs(scaledx.Dot(DUOLMath::Vector3::Right)) + fabs(scaledy.Dot(DUOLMath::Vector3::Right)) + fabs(scaledz.Dot(DUOLMath::Vector3::Right));
		float y = std::fabs(scaledx.Dot(DUOLMath::Vector3::Up)) + fabs(scaledy.Dot(DUOLMath::Vector3::Up)) + fabs(scaledz.Dot(DUOLMath::Vector3::Up));
		float z = std::fabs(scaledx.Dot(DUOLMath::Vector3::Forward)) + fabs(scaledy.Dot(DUOLMath::Vector3::Forward)) + fabs(scaledz.Dot(DUOLMath::Vector3::Forward));
		
		// �ڽ� ����� �ٿ�� �ڽ��� ũ��
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

			// �����͸� ���δ� �ڽ�, 8���� ���� �ش� ��忡 �� �� �ִٸ� ��ȯ����.
			for (int j = 0 ; j < 8 ; j++)
			{
				DUOLMath::Vector3 point = scaledCenter;

				point.x += (j & 4 ? x : -x);
				point.y += (j & 2 ? y : -y);
				point.z += (j & 1 ? z : -z);

				// ����.
				if (((min.x <= point.x) && (point.x <= max.x)) && ((min.y <= point.y) && (point.y <= max.y)) && ((min.z <= point.z) && (point.z <= max.z)))
					continue;
				else
				{
					flag = false;

					break;
				}
			}

			// �ش� ���ϵ� ��忡 �� �� �ִٰ� üũ�Ǹ�, �ش� ���ϵ� ����� �ε����� ��ȯ�մϴ�.
			if (flag)
				return i;
		}

		// ��� ���ϵ� ��忡�� �� �� �����ϴ�.
		return 8;
	}

	void Octree::ViewFrustumCullingAllNodes(DUOLGameEngine::Frustum& frustum, std::unordered_map<void*, bool>& outDatas)
	{
		if (GeometryHelper::ViewFrustumCullingAABB(_origin, _halfExtents, frustum))
		{
			for (auto data : _datas)
				outDatas.insert({ data._renderObjectInfo, true });

			// �ڽ� ��� �� �Ϻδ� �°����� .. �Ϻδ� �� ���� ���� �ִ� ..
			if (!IsLeafNode())
				for (auto child : _children)
					child->ViewFrustumCullingAllNodes(frustum, outDatas);
		}

		// �θ� ��尡 ���� �ʾ����� .. �ڽ� ���� �翬�� �� �´´�.
	}
}