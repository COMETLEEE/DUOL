#include "DUOLPhysics/Util/ObjectLayer.h"

namespace DUOLPhysics
{
	std::unordered_map<DUOLCommon::tstring, LayerState> ObjectLayerControl::_layerStates = {};
	std::unordered_map<DUOLCommon::tstring, unsigned int> ObjectLayerControl::_layers = {};

	DUOLPhysics::LayerState& ObjectLayerControl::GetLayerState(const DUOLCommon::tstring& layerName)
	{
		// if (_layerStates.contains(layerName))
			return _layerStates.at(layerName);
	}

	void ObjectLayerControl::AddLayer(const DUOLCommon::tstring& layerName)
	{
		if (!_layerStates.contains(layerName))
		{
			unsigned int currentLayer = 1 << _layerStates.size();

			unsigned int canColliderWithAllMask = 0xFFFFFFFF;

			_layerStates.insert({ layerName, {currentLayer, canColliderWithAllMask} });

			_layers.insert({ layerName, currentLayer });
		}
	}
}