#pragma once
#include "DUOLCommon/ImGui/imgui.h"
#include "DUOLCommon/StringHelper.h"

namespace DUOLEditor
{
	/**
	 * \brief add-ons are used to add functionality to widgets.
	 */
	class AddOnBase
	{
	public:
		virtual ~AddOnBase() = default;

		/**
		 * \brief Execute this add-in's function.
		 */
		virtual void Execute() = 0;

		void* _userData = nullptr;
	};
}