#include "DUOLEditor/UI/Widgets/Plots/PlotBase.h"

namespace DUOLEditor
{
	PlotBase::PlotBase(const std::vector<float>& data, float minScale, float maxScale, const DUOLMath::Vector2& size,
		const DUOLCommon::tstring& overlay, const DUOLCommon::tstring& label, int forceHover) :
		DataWidgetBase<std::vector<float>>(&_data)
		, _minScale(minScale)
		, _maxScale(maxScale)
		, _size(size)
		, _overlay(overlay)
		, _label(label)
		, _forceHover(forceHover)
	{
	}
}