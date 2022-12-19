#pragma once
#include "DUOLCommon/ImGui/imgui.h"
#include "DUOLCommon/StringHelper.h"

namespace DUOLEditor
{
	/**
	 * \brief ImGui => ImFont wrapper.
	 */
	struct Font
	{
	public:
		Font(const DUOLCommon::tstring& fontID, ImFont* imFont);


		ImFont* GetImFont();

		DUOLCommon::tstring GetFontID();

	private:
		/**
		 * \brief ImFont id in imgui system.
		 */
		DUOLCommon::tstring _fontID;

		/**
		 * \brief ImGui Font
		 */
		ImFont* _imFont;
	};
}
