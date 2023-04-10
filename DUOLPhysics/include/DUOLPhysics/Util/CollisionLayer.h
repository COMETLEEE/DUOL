#pragma once

namespace DUOLPhysics
{
	enum CollisionLayer
	{
		Layer_0 = (1 << 0)				// 모든 것들이랑 충돌할 수 있습니다. 디폴트 레이어.
		, Layer_1 = (1 << 1)			// 유저 셋팅 가능
		, Layer_2 = (1 << 2)			// 유저 셋팅 가능
		, Layer_3 = (1 << 3)
	};
}