/**

    @file    NodeInfo.h
    @brief   Behavior Tree�� Node�� Type & State Info ���
    @details 
			 @li NodeType - Node�� ���� ����

				* NONE
				 - ������ �������� ����

				* ACTION
				 - ������ ������ ����

				* CONDITION
				 - �־��� ������ ���� or ���и� �Ǵ�

				* CONTROL
				 - ����� �帧 ����, 1 ~ N���� �ڽ� ��� ����
				 - Sequence, Selector(Fallback)
					
				* DECORATOR
				 - �� 1���� �ڽ� ��带 �����ϰ�, �ڽ� ����� ��ȯ ���� ���� ó��
				 - Inverter, ForceSuccess, ForceFailure, Repeat, Retry

				* SUBTREE
				 - ���ȭ�� Ʈ��

			 @n
			 @li NodeState - Node�� ���� ����

				* IDLE
				 - ���� ����, IDLE�� Return ������ ������ ����

				* RUNNING
				 - ������

				* SUCCESS
				 - ������ �������� �Ϸ��

				* FAILURE
				 - ������ ���з� �Ϸ��
				
			@n
    @author  JKim
    @date    11.10.2022

**/
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