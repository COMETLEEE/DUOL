/**

    @file      BinaryTree.h
    @brief     이진 트리입니다. 플레이어의 콤보 리스트 관리를 위해서 구현합니다.
    @details   ~
    @author    COMETLEE
    @date      11.04.2023
    @copyright © COMETLEE, 2023. All right reserved.

**/
#pragma once

namespace DUOLClient
{
	/**
	 * \brief 심플 이진 트리
	 */
	template <typename TData>
	struct BinaryTree
	{
		BinaryTree();

		BinaryTree(TData data);

		~BinaryTree() { delete _left; delete _right; }

		BinaryTree* _left;

		BinaryTree* _right;

		TData _data;

	public:
		BinaryTree* AddLeftNode(TData data);

		BinaryTree* AddRightNode(TData data);

		const TData& GetData();

		bool HasLeftNode();

		bool HasRightNode();

		BinaryTree* GetLeftNode();

		BinaryTree* GetRightNode();
	};

	template <typename TData>
	BinaryTree<TData>::BinaryTree() :
		_left(nullptr)
		, _right(nullptr)
		, _data()
	{
		_left = nullptr;

		_right = nullptr;

		_data = TData();
	}

	template <typename TData>
	BinaryTree<TData>::BinaryTree(TData data) :
		_left(nullptr)
		, _right(nullptr)
		, _data(data)
	{
		
	}

	template <typename TData>
	BinaryTree<TData>* BinaryTree<TData>::AddLeftNode(TData data)
	{
		if (_left != nullptr)
			return _left;

		_left = new BinaryTree(data);

		return _left;
	}

	template <typename TData>
	BinaryTree<TData>* BinaryTree<TData>::AddRightNode(TData data)
	{
		if (_right != nullptr)
			return _right;

		_right = new BinaryTree(data);

		return _right;
	}

	template <typename TData>
	const TData& BinaryTree<TData>::GetData()
	{
		return _data;
	}

	template <typename TData>
	bool BinaryTree<TData>::HasLeftNode()
	{
		return _left != nullptr;
	}

	template <typename TData>
	bool BinaryTree<TData>::HasRightNode()
	{
		return _right != nullptr;
	}

	template <typename TData>
	BinaryTree<TData>* BinaryTree<TData>::GetLeftNode()
	{
		return _left;
	}

	template <typename TData>
	BinaryTree<TData>* BinaryTree<TData>::GetRightNode()
	{
		return _right;
	}
}