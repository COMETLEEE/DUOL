#pragma once
#include "DUOLEditor/UI/Widgets/DataWidgetBase.h"

namespace DUOLEditor
{
	/**
	 * \brief Base class for any plot data widget.
	 */
	class PlotBase: public DataWidgetBase<std::vector<float>>
	{
	public:
		PlotBase(const std::vector<float>& data = std::vector<float>()
			, float minScale = std::numeric_limits<float>::min()
			, float maxScale = std::numeric_limits<float>::max()
			, const DUOLMath::Vector2& size = { 0.f, 0.f }
			, const DUOLCommon::tstring& overlay = TEXT("")
			, const DUOLCommon::tstring& label = TEXT("")
			, int forceHover = -1
		);

	protected:
		virtual void Draw_Impl() = 0;

	public:
		std::vector<float>		_data;
		float					_minScale;
		float					_maxScale;
		DUOLMath::Vector2		_size;
		DUOLCommon::tstring		_overlay;
		DUOLCommon::tstring		_label;
		int						_forceHover;
	};
}