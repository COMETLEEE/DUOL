#pragma once

#include <memory>
#include <vector>
#include <array>
#include <Windows.h>
#include <DirectXMath.h>
#include "../../DUOLMath/include/DUOLMath/DUOLMath.h"

#include "ComponentsHead.h"

namespace Muscle
{
	using namespace DirectX;
	class QuadTree
	{
	public:
		QuadTree()
		{
			_root = std::make_shared<Node>();
		}
		~QuadTree()
		{
			ClearNode();

			_root.reset();

		}
		// Box Size

		float _startWidth = 0; // X 시작
		float _startHeight = 0; // Z 시작
		float _endWidth = 0; // X 끝
		float _endHeight = 0; // Z 끝
		// 매 프레임마다 사이즈를 바꿀까?
	public:
		struct Node
		{
			Node() {}

			~Node()
			{
				for (auto& iter : _children)
				{
					iter.reset();
				}
				_values.clear();
			}

			int _depth = 0;
			std::array<std::shared_ptr<Node>, 4> _children; // 순환 참조는 안할것이다..
			// 0 LT
			// 1 RT
			// 2 LB
			// 3 RB
			std::unordered_map<UINT, std::shared_ptr<Collider>> _values; //루트에 보관중인 충돌체들.

			// box 구조체 만들기 귀찮다!
			float _posX = 0;
			float _posY = 0;
			float _width = 0;
			float _height = 0;
		};
	private:


		std::shared_ptr<Node> _root; // 시작 노드.

		std::vector<std::shared_ptr<Node>> leafNodes; // 마지막 노드들.

	public:
		void ClearNode()
		{
			for (auto& iter : _root->_children)
			{
				iter.reset();
			}

			_root->_values.clear();

			leafNodes.clear();
		}

		const std::shared_ptr<Node>& GetRoot() { return _root; }

		const std::vector<std::shared_ptr<Node>>& GetLeafNodes() { return leafNodes; }

		void StartSplit(const std::unordered_map<UINT, std::shared_ptr<Collider>>& colliders)
		{
			ClearNode();
			_root->_values = colliders; // 아아아ㅏㅏ 매번 복사하기 싫은뎅! 어떡하지~
			//_root->_values.resize(colliders.size());
			//memcpy(&_root->_values[0], &colliders[0], sizeof(std::shared_ptr<Collider>) * colliders.size());


			_startWidth = 999999.9f; // X 시작
			_startHeight = 999999.9f; // Z 시작
			_endWidth = -999999.9f; // X 끝
			_endHeight = -999999.9f; // Z 끝


			//;
			//max()
			for (auto& iter : colliders)
			{
				if (!iter.second->GetIsEnable()) continue;

				const DUOLMath::Vector3 pos = iter.second->GetWorldPosition(); // 이거 하나 떄문에 SimpleMath를 가져와야하다니!
				_startWidth = min(pos.x, _startWidth);
				_endWidth = max(pos.x, _endWidth);
				_startHeight = min(pos.z, _startHeight);
				_endHeight = max(pos.z, _endHeight);
			}

			SplitSection(_root, _startWidth, _endWidth, _startHeight, _endHeight);
		}

	private:


		void SplitSection(std::shared_ptr<Node> _node, float startWidth, float endWidth, float startHeight, float endHeight) // 공간 나누기.
		{
			_node->_posX = (startWidth + endWidth) / 2;
			_node->_posY = (startHeight + endHeight) / 2;
			_node->_width = endWidth - startWidth;
			_node->_height = endHeight - startHeight;
			// 재귀적으로 호출 할 것.
			float halfWidth = (endWidth - startWidth) / 2;
			float halfHeight = (endHeight - startHeight) / 2;


			if (halfWidth < 2.0f || halfHeight < 2.0f)
			{
				leafNodes.emplace_back(_node);
				return; // 일정 크기보다 작아지면 공간분할 종료.
			}
			if (_node->_values.size() <= 1)
			{
				leafNodes.emplace_back(_node);
				return; // 나눌 콜라이더가 없으면 공간 분할 종료.
			}
			for (auto& iter : _node->_values)
			{
				if (!iter.second->GetIsEnable()) continue;

				const auto& position = iter.second->GetWorldPosition();
				// 지금은 일단 간단하게 포지션으로만 구분하지만 구, 박스, 캡슐등 형태에 따라 구분요소가 늘어날것. 
				const DUOLMath::Vector3& lt = DUOLMath::Vector3(position.x + iter.second->_minX, 0, position.z + iter.second->_maxZ);
				const DUOLMath::Vector3& rt = DUOLMath::Vector3(position.x + iter.second->_maxX, 0, position.z + iter.second->_maxZ);
				const DUOLMath::Vector3& lb = DUOLMath::Vector3(position.x + iter.second->_minX, 0, position.z + iter.second->_minZ);
				const DUOLMath::Vector3& rb = DUOLMath::Vector3(position.x + iter.second->_maxX, 0, position.z + iter.second->_minZ);
				bool isCheck[4] = { false,false, false, false };

				isCheck[InsertValue(lt, _node, iter, startWidth, startHeight, halfWidth, halfHeight, isCheck)] = true;
				isCheck[InsertValue(rt, _node, iter, startWidth, startHeight, halfWidth, halfHeight, isCheck)] = true;
				isCheck[InsertValue(lb, _node, iter, startWidth, startHeight, halfWidth, halfHeight, isCheck)] = true;
				isCheck[InsertValue(rb, _node, iter, startWidth, startHeight, halfWidth, halfHeight, isCheck)] = true;
			}





			if (_node->_children[0])// 0 LT
				SplitSection(_node->_children[0], startWidth, startWidth + halfWidth, startHeight + halfHeight, endHeight);
			if (_node->_children[1])// 1 RT
				SplitSection(_node->_children[1], startWidth + halfWidth, endWidth, startHeight + halfHeight, endHeight);
			if (_node->_children[2])// 2 LB
				SplitSection(_node->_children[2], startWidth, startWidth + halfWidth, startHeight, startHeight + halfHeight);
			if (_node->_children[3])// 3 RB
				SplitSection(_node->_children[3], startWidth + halfWidth, endWidth, startHeight, startHeight + halfHeight);



		}

		// 공간에 맞게 삽입하는 함수!
		UINT InsertValue(const DUOLMath::Vector3& position, std::shared_ptr<Node>& node, std::pair<const UINT, std::shared_ptr<Collider>>& iter,
			float& startWidth, float& startHeight, float& halfWidth, float& halfHeight, bool* isCheck)
		{
			if (position.x < startWidth + halfWidth) // L
			{
				if (position.z > startHeight + halfHeight) // T
				{
					if (isCheck[0])
						return 0;
					// LT 0
					if (!node->_children[0])
					{
						node->_children[0] = std::make_shared<Node>();
						node->_children[0]->_depth = node->_depth + 1;
					}
					node->_children[0]->_values[iter.first] = iter.second;

					return 0;
				}
				else // B
				{
					if (isCheck[2])
						return 2;
					// LB 2
					if (!node->_children[2])
					{
						node->_children[2] = std::make_shared<Node>();
						node->_children[2]->_depth = node->_depth + 1;
					}
					node->_children[2]->_values[iter.first] = iter.second;
					return 2;
				}
			}
			else // R
			{
				if (position.z > startHeight + halfHeight) // T
				{
					if (isCheck[1])
						return 1;
					// RT 1
					if (!node->_children[1])
					{
						node->_children[1] = std::make_shared<Node>();
						node->_children[1]->_depth = node->_depth + 1;
					}
					node->_children[1]->_values[iter.first] = iter.second;
					return 1;
				}
				else // B
				{
					if (isCheck[3])
						return 3;
					// RB 3
					if (!node->_children[3])
					{
						node->_children[3] = std::make_shared<Node>();
						node->_children[3]->_depth = node->_depth + 1;
					}
					node->_children[3]->_values[iter.first] = iter.second;
					return 3;
				}
			}
		}


	};
}