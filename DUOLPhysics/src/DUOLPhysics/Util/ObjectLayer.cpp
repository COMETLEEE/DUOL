#include "DUOLPhysics/Util/ObjectLayer.h"

namespace DUOLPhysics
{
	std::unordered_map<DUOLCommon::tstring, unsigned long long> ObjectLayerControl::_layers = {};

	bool ObjectLayerControl::_physicsCollisionLayerMatrix[DUOLPhysics::OBJECT_LAYER_COUNT][DUOLPhysics::OBJECT_LAYER_COUNT] = {};


	DUOLPhysics::ObjectLayer ObjectLayerControl::_layerNumbers[OBJECT_LAYER_COUNT] =
		{ Layer_0, Layer_1, Layer_2, Layer_3, Layer_4, Layer_5, Layer_6, Layer_7
			,Layer_8 , Layer_9, Layer_10, Layer_11, Layer_12, Layer_13, Layer_14, Layer_15 };

	DUOLPhysics::ObjectLayer ObjectLayerControl::GetLayerNumber(const DUOLCommon::tstring& layer)
	{
		return _layers.contains(layer) ?_layerNumbers[(_layers.at(layer))] : ObjectLayer::Layer_0;
	}

	void ObjectLayerControl::AddLayer(const DUOLCommon::tstring& layerName)
	{
		_layers.insert({layerName, _layers.size() });
	}
}