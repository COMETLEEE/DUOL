/**

	@file      Octree.h
	@brief     3차원 공간을 높이까지 포함하여 분할합니다. 
	@details   ~
	@author    COMETLEE
	@date      25.03.20223
	@copyright © COMETLEE, 2023. All right reserved.

**/
#pragma once
#include "DUOLMath/DUOLMath.h"

namespace DUOLGameEngine
{
	class Scene;
	class Transform;
}

namespace DUOLGameEngine
{
	constexpr uint32_t OCTREE_MAX_DEPTH = 10;

	struct OctreeData
	{
		DUOLGameEngine::Transform* _transform;

		DUOLMath::Vector3 _halfExtents;

		DUOLMath::Vector3 _center;
	};

	/**
	 * \brief 3차원 씬 공간을 높이까지 포함하여 분할합니다. 바운딩 박스로 계산할 수 있는 대상에 대하여.
	 */
	class Octree
	{
	public:
		Octree(const DUOLMath::Vector3& origin, const DUOLMath::Vector3& halfExtents, int depth);

		~Octree();

	public:
		/**
		 * \brief 해당 노드의 중심점을 의미합니다.
		 */
		DUOLMath::Vector3 _origin;

		/**
		 * \brief 해당 노드의 바운딩 박스를 의미합니다. (Width, Height, Depth)
		 */
		DUOLMath::Vector3 _halfExtents;

		/**
		 * \brief 해당 노드에 있는 { Transform, 원 스케일 기본 바운딩 박스 } 의 리스트입니다.
		 */
		std::list<OctreeData> _datas;

		/**
		 * \brief 자식 노드들입니다.
		 * 자식 노드들은 다음과 같은 패턴을 따릅니다.
		 *		child:	0 1 2 3 4 5 6 7
		 *		x:      - - - - + + + +
		 *		y:      - - + + - - + +
		 *		z:      - + - + - + - +
		 */
		Octree* _children[8];

		/**
		 * \brief 부모 노드입니다.
		 */
		Octree* _parent;

		/**
		 * \brief 현재 노드의 깊이입니다.
		 */
		int _currentDepth;

	public:
		// TODO : Transform deleted 이벤트.

		/**
		 * \brief 씬의 오브젝트를 관리할 수 있는 옥트리를 빌드하여 반환합니다.
		 * \param scene 옥트리를 빌드할 씬입니다.
		 * \return 빌드된 옥트리
		 */
		static Octree* BuildOctree(DUOLGameEngine::Scene* scene);

		bool IsLeafNode();

		/**
		 * \brief 적합한 노드에 해당 데이터를 넣습니다.
		 * \param octreeData 
		 */
		void Insert(OctreeData& octreeData);

		/**
		 * \brief 옥트리 데이터가 들어갈 수 있는 차일드 노드를 판단합니다.
		 * \param octreeData 옥트리에 들어갈 데이터 
		 * \return 0 ~ 7 / 8일 경우 하나도 들어갈 수 있는 노드가 없다 ..! (=> 모든 자식 노드에 걸치는 상황, 현재 노드에 넣자)
		 */
		int GetChildNodeOfContainingData(OctreeData& octreeData);
	};
}