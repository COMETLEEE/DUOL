#pragma once

namespace DUOLEditor
{
	/**
	 * \brief 에디터 세팅 (트랜스폼 기즈모 변환 속도 등 ..) 을 모아 놓습니다.
	 */
	class EditorSettings
	{
	public:
		// 양의 방향으로 100픽셀 이동하면 5
		static constexpr float TRANSLATE_PER_PIXEL = 0.05f;

		// 양의 방향으로 100픽셀 이동하면 2배
		static constexpr float SCALE_PER_PIXEL = 0.02f;

		// 양의 방향으로 100 픽셀 이동하면 45도 회전
		static constexpr float ROTATION_PER_PIXEL = 0.00785f;
	};
	
}