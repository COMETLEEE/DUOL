#pragma once
#include "DUOLEditor/UI/Widgets/WidgetGroupBase.h"

namespace DUOLEditor
{
	/**
	 * \brief 위젯을 그룹핑하면서 컬럼화를 실시하는 위젯
	 */
	template <size_t Size>
	class Columns : public WidgetBase, public WidgetGroupBase
	{
	public:
		Columns()
		{
			_widths.fill(-1.f);
		}

	protected:
		virtual void Draw_Impl() override
		{
			ImGui::Columns(static_cast<int>(Size),
				DUOLCommon::StringHelper::ToString(TEXT("##") + _tstringID).c_str(), false);

			int counter = 0;

			DestroyReservedWidgets();

			for (auto iter = _widgets.begin() ; iter != _widgets.end() ;)
			{
				(*iter)->Draw();

				++iter;

				if (iter != _widgets.end())
				{
					if (_widths[counter] != -1.f)
					{
						ImGui::SetColumnWidth(counter, _widths[counter]);
					}

					ImGui::NextColumn();
				}

				++counter;

				if (counter == Size)
					counter = 0;
			}

			// 다음에 그려지는 위젯을 정돈된 배치에 그리기 위해서 한 칸 밀어줍니다.
			ImGui::Columns(1);
		}

	public:
		std::array<float, Size> _widths;
	};
}