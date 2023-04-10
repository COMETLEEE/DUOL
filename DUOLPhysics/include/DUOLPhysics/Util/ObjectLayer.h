#pragma once
#include <unordered_map>
#include "DUOLCommon/StringHelper.h"

namespace DUOLPhysics
{
	constexpr unsigned long long OBJECT_LAYER_COUNT = 16;

	enum ObjectLayer
	{
		Layer_0 = (1 << 0)				// ��� �͵��̶� �浹�� �� �ֽ��ϴ�. ����Ʈ ���̾�.
		, Layer_1 = (1 << 1)			// ���� ���� ����
		, Layer_2 = (1 << 2)			// ���� ���� ����
		, Layer_3 = (1 << 3)			// ����ϼ���
		, Layer_4 = (1 << 4)			// ����ϼ���
		, Layer_5 = (1 << 5)			// ����ϼ���
		, Layer_6 = (1 << 6)			// ����ϼ���
		, Layer_7 = (1 << 7)			// ����ϼ���
		, Layer_8 = (1 << 8)			// ����ϼ���
		, Layer_9 = (1 << 9)			// ����ϼ���
		, Layer_10 = (1 << 10)			// ����ϼ���
		, Layer_11 = (1 << 11)			// ����ϼ���
		, Layer_12 = (1 << 12)			// ����ϼ���
		, Layer_13 = (1 << 13)			// ����ϼ���
		, Layer_14 = (1 << 14)			// ����ϼ���
		, Layer_15 = (1 << 15)			// ����ϼ���
	};

	struct ObjectLayerControl
	{
	public:
		// ������� �ֽ��ϴ� ..! �̸����� ���̾� �߻�ȭ
		static std::unordered_map<DUOLCommon::tstring, unsigned long long> _layers;

		static DUOLPhysics::ObjectLayer _layerNumbers[OBJECT_LAYER_COUNT];

		static DUOLPhysics::ObjectLayer GetLayerNumber(const DUOLCommon::tstring& layer);

		static void AddLayer(const DUOLCommon::tstring& layerName);
	};
}