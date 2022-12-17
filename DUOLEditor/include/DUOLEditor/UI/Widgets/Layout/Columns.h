#pragma once
#include "DUOLEditor/UI/Widgets/WidgetGroupBase.h"

namespace DUOLEditor
{
	/**
	 * \brief ������ �׷����ϸ鼭 �÷�ȭ�� �ǽ��ϴ� ����
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

			// ������ �׷����� ������ ������ ��ġ�� �׸��� ���ؼ� �� ĭ �о��ݴϴ�.
			ImGui::Columns(1);
		}

	public:
		std::array<float, Size> _widths;
	};
}