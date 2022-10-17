/**

	@file    BehaviorTree.h
	@brief	 Behavior Tree State Machine
	@details -
	@author  JKim
	@date    14.10.2022

**/
#pragma once
/* Sub Tree Header */
#include "SubTree.h"

/* Control Node Header */
#include "Nodes/SequenceNode.h"
#include "Nodes/SelectorNode.h"

/* Decorator Node Header */
#include "Nodes/ConditionNode.h"
#include "Nodes/InverterNode.h"
#include "Nodes/AlwaysFailureNode.h"
#include "Nodes/AlwaysSuccessNode.h"
#include "Nodes/RepeatNode.h"
#include "Nodes/RetryNode.h"

/* Action Node Header */
#include "Nodes/ActionNode.h"

namespace DUOLGameEngine
{
	/**

		@class   BehaviorTree
		@brief	 Behavior Tree State Machine
		@details -

	**/
	class BehaviorTree
	{
		SubTree _root;
		std::weak_ptr<BlackBoard> _blackBoard;

	public:
		/**
			@brief   BehaviorTree Ŭ���� ������
			@details -
			@param   name -
		**/
		BehaviorTree(const std::string& name);

		/**
			@brief   BehaviorTree Ŭ���� default �Ҹ���
			@details -
		**/
		~BehaviorTree() = default;

		/**
			@brief	 BlackBoard ���
			@details -
			@param   blackBoard - ����� BlackBoard
		**/
		void RegistBlackBoard(const std::shared_ptr<BlackBoard>& blackBoard);

		/**
			@brief	 RootNode�� �޾ƿ´�.
			@details -
			@retval  Behavior Tree������ SubTree == root
		**/
		SubTree* GetRoot() { return &_root; }

		/**
			@brief	 Root���� �����Ѵ�.
			@details -
		**/
		void Execute();
	};
}