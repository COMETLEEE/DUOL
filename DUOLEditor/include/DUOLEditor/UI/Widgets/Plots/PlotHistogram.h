#pragma once
#include "DUOLEditor/UI/Widgets/Plots/PlotBase.h"

namespace DUOLEditor
{
	/**
	 * \brief Plot displayed as an data histogram.
	 */
	class PlotHistogram : public PlotBase
	{
	public:
		PlotHistogram(const std::vector<float>& data = std::vector<float>()
			, float minScale = std::numeric_limits<float>::min()
			, float maxScale = std::numeric_limits<float>::max()
			, const DUOLMath::Vector2& size = { 0.f, 0.f }
			, const DUOLCommon::tstring& overlay = TEXT("")
			, const DUOLCommon::tstring& label = TEXT("")
			, int forceHover = -1
		);

	protected:
		virtual void Draw_Impl() override;
	};
}
