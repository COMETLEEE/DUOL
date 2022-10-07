#pragma once

namespace DUOLGameEngine
{
	// Node�� ���� ����
	enum class NodeType
	{
		// ������ �������� ����
		NONE,

		// ������ ������ ����
		ACTION,

		// �־��� ������ ����or���и� �Ǵ�
		CONDITION,

		/*
		 * ����� �帧 ����, 1 ~ N���� �ڽ� ��� ����
		 * Sequence, Selector(Fall back)
		*/
		CONTROL,

		/*
		 * �� 1���� �ڽ� ��带 �����ϰ�, �ڽ� ����� ��ȯ ���� ���� ó��
		 * Inverter, ForceSuccess, ForceFailure, Repeat, Retry
		*/
		DECORATOR,

		// ���ȭ�� Ʈ��
		SUBTREE
	};

	// Node�� ���� ����
	enum class NodeState
	{
		IDLE,		// ���� ����, IDLE�� Return ������ ������ ����
		RUNNING,	// ������
		SUCCESS,	// ������ �������� �Ϸ��
		FAILURE		// ������ ���з� �Ϸ��
	};
}