#pragma once
#include <unordered_map>
#include "DUOLCommon/StringHelper.h"

namespace DUOLPhysics
{
	constexpr unsigned long long OBJECT_LAYER_COUNT = 16;

	enum ObjectLayer
	{
		Layer_0 = (1 << 0)				// 모든 것들이랑 충돌할 수 있습니다. 디폴트 레이어.
		, Layer_1 = (1 << 1)			// 유저 셋팅 가능
		, Layer_2 = (1 << 2)			// 유저 셋팅 가능
		, Layer_3 = (1 << 3)			// 사용하세요
		, Layer_4 = (1 << 4)			// 사용하세요
		, Layer_5 = (1 << 5)			// 사용하세요
		, Layer_6 = (1 << 6)			// 사용하세요
		, Layer_7 = (1 << 7)			// 사용하세요
		, Layer_8 = (1 << 8)			// 사용하세요
		, Layer_9 = (1 << 9)			// 사용하세요
		, Layer_10 = (1 << 10)			// 사용하세요
		, Layer_11 = (1 << 11)			// 사용하세요
		, Layer_12 = (1 << 12)			// 사용하세요
		, Layer_13 = (1 << 13)			// 사용하세요
		, Layer_14 = (1 << 14)			// 사용하세요
		, Layer_15 = (1 << 15)			// 사용하세요
	};

	struct ObjectLayerControl
	{
	public:
		// 순서대로 넣습니다 ..! 이름으로 레이어 추상화
		static std::unordered_map<DUOLCommon::tstring, unsigned long long> _layers;

		static DUOLPhysics::ObjectLayer _layerNumbers[OBJECT_LAYER_COUNT];

		static DUOLPhysics::ObjectLayer GetLayerNumber(const DUOLCommon::tstring& layer);

		static void AddLayer(const DUOLCommon::tstring& layerName);
	};
}