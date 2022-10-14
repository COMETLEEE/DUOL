/**

	@file    BehaviorTree.h
	@brief
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

/* Action Node Header */
#include "Nodes/ActionNode.h"

namespace DUOLGameEngine
{
	/**

		@class   BehaviorTree
		@brief
		@details -

	**/
	class BehaviorTree
	{
		SubTree _root;

	public:
		/**
			@brief   BehaviorTree object constructor
			@details -
			@param   name -
		**/
		BehaviorTree(const std::string& name);

		/**
			@brief   BehaviorTree object destructor
			@details -
		**/
		~BehaviorTree() = default;

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