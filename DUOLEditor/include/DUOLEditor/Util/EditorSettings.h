#pragma once

namespace DUOLEditor
{
	/**
	 * \brief ������ ���� (Ʈ������ ����� ��ȯ �ӵ� �� ..) �� ��� �����ϴ�.
	 */
	class EditorSettings
	{
	public:
		// ���� �������� 100�ȼ� �̵��ϸ� 5
		static constexpr float TRANSLATE_PER_PIXEL = 0.05f;

		// ���� �������� 100�ȼ� �̵��ϸ� 2��
		static constexpr float SCALE_PER_PIXEL = 0.02f;

		// ���� �������� 100 �ȼ� �̵��ϸ� 45�� ȸ��
		static constexpr float ROTATION_PER_PIXEL = 0.00785f;
	};
	
}