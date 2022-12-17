#include "DUOLEditor/UI/Widgets/Plots/PlotHistogram.h"

namespace DUOLEditor
{
	PlotHistogram::PlotHistogram(const std::vector<float>& data, float minScale, float maxScale,
		const DUOLMath::Vector2& size, const DUOLCommon::tstring& overlay, const DUOLCommon::tstring& label,
		int forceHover) :
		PlotBase(data, minScale, maxScale, size, overlay, label, forceHover)
	{
	}

	void PlotHistogram::Draw_Impl()
	{
		ImGui::PlotHistogram(DUOLCommon::StringHelper::ToString(_label + _tstringID).c_str(), _data.data(), static_cast<int>(_data.size()), 0, 
			DUOLCommon::StringHelper::ToString(_overlay).c_str(), _minScale, _maxScale, ImGuiHelper::ToImVec2(_size), sizeof(float));
	}
}