#include "DUOLGameEngine/Util/Quadtree.h"
#include "DUOLCommon/Log/LogHelper.h"

#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"
#include "DUOLGameEngine/Util/Geometries.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"
#include "DUOLGameEngine/ECS/Component/MeshFilter.h"

#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"

namespace DUOLGameEngine
{
	Quadtree::Quadtree(const DUOLMath::Vector2& origin, const DUOLMath::Vector2& halfExtents, int depth) :
		_origin{origin}
		, _halfExtents(halfExtents)
		, _datas{}
		, _parent(nullptr)
		, _currentDepth(depth)
	{
		for (int i = 0; i < 4; i++)
			_children[i] = nullptr;
	}

	Quadtree::~Quadtree()
	{
		for (int i = 0; i < 4; i++)
			delete _children[i];
	}

	Quadtree* Quadtree::BuildQuadtree(DUOLGameEngine::Scene* scene)
	{
		DUOL_INFO(DUOL_CONSOLE, "Build Quadtree Start !");

		// ���� ���� min, max
		DUOLMath::Vector2 min{ FLT_MAX, FLT_MAX };
		DUOLMath::Vector2 max{ FLT_MIN, FLT_MIN };

		std::vector<QuadtreeData> quadtreeDatas{};

		auto gameObjects = scene->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			// TODO : ����ƽ ���� ������Ʈ�� ���ؼ��� �����մϴ�.
			// ������Ʈ�� �� �־� ..!
			// if (gameObject->GetIsStatic())
			{
				auto meshFilter = gameObject->GetComponent<DUOLGameEngine::MeshFilter>();

				auto meshRenderer = gameObject->GetComponent<DUOLGameEngine::MeshRenderer>();

				if ((meshFilter == nullptr) || (meshRenderer == nullptr))
					continue;


				auto gameMesh = meshFilter->GetMesh();
				if (gameMesh == nullptr)
					continue;

				auto primitiveMesh = gameMesh->GetPrimitiveMesh();

				if (primitiveMesh != nullptr)
				{
					const DUOLMath::Vector3& halfExtents = primitiveMesh->_halfExtents;

					const DUOLMath::Vector3& center = primitiveMesh->_center;

					QuadtreeData quadtreeData{ gameObject->GetTransform(), &meshRenderer->_renderObjectInfo, halfExtents, center };

					quadtreeDatas.push_back(quadtreeData);

					const DUOLMath::Matrix& worldMatrix = quadtreeData._transform->GetWorldMatrix();

					DUOLMath::Vector4 scaledxV4 = DUOLMath::Vector4::Transform(DUOLMath::Vector4{ quadtreeData._halfExtents.x, 0, 0, 0 }, worldMatrix);
					DUOLMath::Vector4 scaledyV4 = DUOLMath::Vector4::Transform(DUOLMath::Vector4{ 0.f, quadtreeData._halfExtents.y, 0.f, 0.f }, worldMatrix);
					DUOLMath::Vector4 scaledzV4 = DUOLMath::Vector4::Transform(DUOLMath::Vector4{ 0.f, 0.f, quadtreeData._halfExtents.z, 0.f }, worldMatrix);
					DUOLMath::Vector4 scaledCenter = DUOLMath::Vector4::Transform(DUOLMath::Vector4{ center.x, center.y ,center.z, 1.f }, worldMatrix);

					DUOLMath::Vector3 scaledx{ scaledxV4 };
					DUOLMath::Vector3 scaledy{ scaledyV4 };
					DUOLMath::Vector3 scaledz{ scaledzV4 };

					// ���� ��ǥ�迡�� ���� ��Ÿ���� ���� �� �ڽ� ���������� �̵��� (���� ����)
					float x = std::fabs(scaledx.Dot(DUOLMath::Vector3::Right)) + fabs(scaledy.Dot(DUOLMath::Vector3::Right)) + fabs(scaledz.Dot(DUOLMath::Vector3::Right));
					float z = std::fabs(scaledx.Dot(DUOLMath::Vector3::Forward)) + fabs(scaledy.Dot(DUOLMath::Vector3::Forward)) + fabs(scaledz.Dot(DUOLMath::Vector3::Forward));

					min.x = std::min(min.x, scaledCenter.x - x);
					min.y = std::min(min.y, scaledCenter.z - z);

					max.x = std::max(max.x, scaledCenter.x + x);
					max.y = std::max(max.y, scaledCenter.z + z);
				}
			}
		}

		float x = std::fabs(max.x - min.x) * 0.5f;
		float z = std::fabs(max.y - min.y) * 0.5f;

		float halfExtents = std::max(x,z);

		Quadtree* topNode = new Quadtree(DUOLMath::Vector2{ max.x + min.x, max.y + min.y } * 0.5f
			, DUOLMath::Vector2{ halfExtents }
			, 0);

		for (auto quadtreeData : quadtreeDatas)
			topNode->Insert(quadtreeData);

		DUOL_INFO(DUOL_CONSOLE, "Build Quadtree Success !");

		return topNode;
	}

	bool Quadtree::IsLeafNode()
	{
		return _children[0] == nullptr;
	}

	void Quadtree::Insert(QuadtreeData& quadtreeData)
	{
		if (IsLeafNode())
		{
			if (_datas.empty())
			{
				_datas.push_back(quadtreeData);
			}
			else
			{
				if (_currentDepth == QUADTREE_MAX_DEPTH)
				{
					_datas.push_back(quadtreeData);

					return;
				}

				for (int i = 0 ; i < 4 ; i++)
				{
					DUOLMath::Vector2 newOrigin = _origin;
					newOrigin.x += _halfExtents.x * (i & 2 ? .5f : -.5f);
					newOrigin.y += _halfExtents.y * (i & 1 ? .5f : -.5f);

					Quadtree* child = new Quadtree(newOrigin, _halfExtents * 0.5f, _currentDepth + 1);

					child->_parent = this;

					_children[i] = child;
				}

				int childNodeIndex = GetChildNodeOfContainingData(quadtreeData);

				childNodeIndex != 4 ? _children[childNodeIndex]->Insert(quadtreeData) : _datas.push_back(quadtreeData);

				// ��尡 �ɰ������ϱ� ������ �����͸� ��ѷ�����
				std::erase_if(_datas, [this](QuadtreeData& data)
					{
						int childNodeIndex = GetChildNodeOfContainingData(data);

						if (childNodeIndex != 4)
						{
							_children[childNodeIndex]->Insert(data);

							return true;
						}
						else
							return false;
					});
			}
		}
		else
		{
			int childNodeIndex = GetChildNodeOfContainingData(quadtreeData);

			childNodeIndex != 4 ? _children[childNodeIndex]->Insert(quadtreeData) : _datas.push_back(quadtreeData);
		}
	}

	int Quadtree::GetChildNodeOfContainingData(QuadtreeData& quadtreeData)
	{
		const DUOLMath::Matrix& worldMatrix = quadtreeData._transform->GetWorldMatrix();

		DUOLMath::Vector4 scaledxV4 = DUOLMath::Vector4::Transform(DUOLMath::Vector4{ quadtreeData._halfExtents.x, 0, 0, 0 }, worldMatrix);
		DUOLMath::Vector4 scaledyV4 = DUOLMath::Vector4::Transform(DUOLMath::Vector4{ 0.f, quadtreeData._halfExtents.y, 0.f, 0.f }, worldMatrix);
		DUOLMath::Vector4 scaledzV4 = DUOLMath::Vector4::Transform(DUOLMath::Vector4{ 0.f, 0.f,quadtreeData._halfExtents.z,0.f }, worldMatrix);
		DUOLMath::Vector4 scaledCenterV4 = DUOLMath::Vector4::Transform(DUOLMath::Vector4{ quadtreeData._center.x, quadtreeData._center.y,quadtreeData._center.z,1.f }, worldMatrix);

		DUOLMath::Vector3 scaledx{ scaledxV4 };
		DUOLMath::Vector3 scaledy{ scaledyV4 };
		DUOLMath::Vector3 scaledz{ scaledzV4 };
		DUOLMath::Vector2 scaledCenter{ scaledCenterV4.x, scaledCenterV4.z };			// ����, ����

		// ���� ��ǥ�迡�� ���� ��Ÿ���� ���� �� �ڽ� ���������� �̵���
		float x = std::fabs(scaledx.Dot(DUOLMath::Vector3::Right)) + fabs(scaledy.Dot(DUOLMath::Vector3::Right)) + fabs(scaledz.Dot(DUOLMath::Vector3::Right));
		float z = std::fabs(scaledx.Dot(DUOLMath::Vector3::Forward)) + fabs(scaledy.Dot(DUOLMath::Vector3::Forward)) + fabs(scaledz.Dot(DUOLMath::Vector3::Forward));

		// �ڽ� ����� �ٿ�� �ڽ��� ũ��
		DUOLMath::Vector2 newHalfExtents = _halfExtents * 0.5f;

		for (int i = 0; i < 4; i++)
		{
			bool flag = true;

			DUOLMath::Vector2 newOrigin = _origin;
			newOrigin.x += _halfExtents.x * (i & 2 ? .5f : -.5f);
			newOrigin.y += _halfExtents.y * (i & 1 ? .5f : -.5f);

			DUOLMath::Vector2 min = DUOLMath::Vector2 { newOrigin.x - newHalfExtents.x , newOrigin.y - newHalfExtents.y };
			DUOLMath::Vector2 max = DUOLMath::Vector2 { newOrigin.x + newHalfExtents.x , newOrigin.y + newHalfExtents.y };

			// �����͸� ���δ� �ڽ�, 4���� �� ����, ���ΰ� �ش� ��忡 �� �� �ִٸ� ��ȯ����.
			for (int j = 0; j < 4; j++)
			{
				DUOLMath::Vector2 point = scaledCenter;

				point.x += (j & 2 ? x : -x);				// X
				point.y += (j & 1 ? z : -z);				// Z

				// ����.
				if (((min.x <= point.x) && (point.x <= max.x)) && ((min.y <= point.y) && (point.y <= max.y)))
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

		return 4;
	}

	void Quadtree::ViewFrustumCullingAllNodes(DUOLGameEngine::Frustum& frustum,
		std::unordered_map<void*, bool>& outDatas)
	{
		DUOLMath::Vector3 vec3Ori = DUOLMath::Vector3(_origin.x, 0.f, _origin.y);

		DUOLMath::Vector3 vec3Half = DUOLMath::Vector3(_halfExtents.x, QUADTREE_HALF_HEIGHT, _halfExtents.y);

		if (GeometryHelper::ViewFrustumCullingAABB(vec3Ori, vec3Half, frustum))
		{
			for (auto data : _datas)
				outDatas.insert({ data._renderObjectInfo, true });

			// �ڽ� ��� �� �Ϻδ� �°����� .. �Ϻδ� �� ���� ���� �ִ� ..
			if (!IsLeafNode())
				for (auto child : _children)
					child->ViewFrustumCullingAllNodes(frustum, outDatas);
		}
		// ����� �������� �ø��� �����ϴ��� .. ����ƽ ������Ʈ�� �ƴ϶�� �ϴ� ����.
		else
		{
			// ����ƽ�� �ƴ� ��츸 �� ����.
			for (auto data : _datas)
				if (!data._transform->GetGameObject()->GetIsStatic())
					outDatas.insert({ data._renderObjectInfo, true });
		}

		// �θ� ��尡 ���� �ʾ����� .. �ڽ� ���� �翬�� �� �´´�.
	}
}