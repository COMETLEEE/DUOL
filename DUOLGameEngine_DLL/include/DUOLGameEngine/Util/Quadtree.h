/**

    @file      Quadtree.h
    @brief     2, 2.5차원 공간의 분할에 사용합니다.
    @details   ~
    @author    COMETLEE
    @date      28.03.2023
    @copyright © COMETLEE, 2023. All right reserved.

**/
#pragma once
#include "DUOLMath/DUOLMath.h"

namespace DUOLGraphicsEngine
{
	struct RenderObject;
}

namespace DUOLGameEngine
{
	struct Frustum;
	class Scene;
	class Transform;
}

namespace DUOLGameEngine
{
	constexpr uint32_t QUADTREE_MAX_DEPTH = 5;

	constexpr float QUADTREE_HALF_HEIGHT = 30.f;

	struct QuadtreeData
	{
		DUOLGameEngine::Transform* _transform;

		DUOLGraphicsEngine::RenderObject* _renderObjectInfo;

		DUOLMath::Vector3 _halfExtents;

		DUOLMath::Vector3 _center;
	};

	/**
	 * \brief 2 ~ 2.5 차원 정도의 분할에 유용하게 쓰일 쿼드트리 자료구조
	 */
	class Quadtree
	{
	public:
		Quadtree(const DUOLMath::Vector2& origin, const DUOLMath::Vector2& halfExtents, int depth);

		~Quadtree();

	public:
		/**
		 * \brief 해당 노드의 중심점을 의미합니다. (Vector3 - X, 0, Z)
		 */
		DUOLMath::Vector2 _origin;

		/**
		 * \brief 해당 노드의 바운딩 박스를 의미합니다. (X : Width, Y : Depth (Z))
		 */
		DUOLMath::Vector2 _halfExtents;

		/**
		 * \brief 해당 노드에 있는 { Transform, 원 스케일 기본 바운딩 박스 } 의 리스트입니다.
		 */
		std::list<QuadtreeData> _datas;

		/**
		 * \brief 자식 노드들입니다.
		 * 자식 노드들은 다음과 같은 패턴을 따릅니다.
		 *		child:	0 1 2 3
		 *		x:      - - + +						 & 2
		 *		z:      - + - +  					 & 1
		 */
		Quadtree* _children[4];

		/**
		 * \brief 부모 노드입니다.
		 */
		Quadtree* _parent;

		/**
		 * \brief 현재 노드의 깊이입니다.
		 */
		int _currentDepth;

	public:
		/**
		 * \brief 씬의 오브젝트를 관리할 수 있는 쿼드트리를 빌드하여 반환합니다.
		 * \param scene 쿼드트리를 빌드할 씬입니다.
		 * \return 빌드된 쿼드트리
		*/
		static Quadtree* BuildQuadtree(DUOLGameEngine::Scene* scene);

		/**
		 * \brief 해당 노드가 꼬리 노드인지 검사합니다.
		 * \return 꼬리 노드인지 Yes or no. 
		 */
		bool IsLeafNode();

		/**
		 * \brief 적합한 노드에 해당 데이터를 넣습니다.
		 * \param quadtreeData 
		 */
		void Insert(QuadtreeData& quadtreeData);

		/**
		 * \brief 옥트리 데이터가 들어갈 수 있는 차일드 노드를 판단합니다.
		 * \param quadtreeData 옥트리에 들어갈 데이터
		 * \return 0 ~ 7 / 8일 경우 하나도 들어갈 수 있는 노드가 없다 ..! (=> 모든 자식 노드에 걸치는 상황, 현재 노드에 넣자)
		 */
		int GetChildNodeOfContainingData(QuadtreeData& quadtreeData);

		/**
		 * \brief 쿼드트리의 노드들에 대해서 프러스텀 컬링을 진행합니다.
		 * \param frustum 타게팅된 프러스텀
		 * \param outDatas 프러스텀과 충돌할 가능성이 있는 데이터들을 반환합니다.
		 */
		void ViewFrustumCullingAllNodes(DUOLGameEngine::Frustum& frustum, std::unordered_map<void*, bool>& outDatas);


	};
}