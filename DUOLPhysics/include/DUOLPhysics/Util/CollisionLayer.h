#pragma once

namespace DUOLPhysics
{
	enum CollisionLayer
	{
		Layer_0 = (1 << 0)				// ��� �͵��̶� �浹�� �� �ֽ��ϴ�. ����Ʈ ���̾�.
		, Layer_1 = (1 << 1)			// ���� ���� ����
		, Layer_2 = (1 << 2)			// ���� ���� ����
		, Layer_3 = (1 << 3)
	};
}