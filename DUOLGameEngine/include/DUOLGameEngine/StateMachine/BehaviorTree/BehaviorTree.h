/**

	@file    BehaviorTree.h
	@brief
	@details -
	@author  JKim
	@date    14.10.2022

**/
#pragma once
#include "SubTree.h"

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

		NodeBase* GetRoot() { return &_root; }
	};
}