#pragma once
#include "PlayerStateBase.h"

#include "DUOLClient/Util/BinaryTree.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorController.h"

namespace DUOLClient
{
	constexpr DUOLMath::Vector3 SWORD_FOURTH_HIT_BOX = { 6.f, 3.f, 3.f };

	constexpr float SWORD_FOURTH_HIT_RANGE = 3.f;

	/**
	 * \brief �÷��̾� ���� ����
	 */
	enum class Player_AttackType
	{
		SWORD
		, FIST
	};

	/**
	 * \brief �÷��̾� �⺻ ���� ������
	 */
	struct Player_NormalAttack
	{
		Player_AttackType _attackType;

		// �ִϸ����� �Ķ���� �̸� / �ִϸ����� �Ķ���� Ÿ�� / ������ ��
		std::vector<std::tuple<DUOLCommon::tstring, DUOLGameEngine::AnimatorControllerParameterType, std::any>> _animatorParameterTable;

		// �÷��̾� �߽����κ��� Local ���� ���� Forward, Right, Up Offset
		DUOLMath::Vector3 _hitCenterOffset;

		// ���� ������ ������
		float _hitRadius;

		// ���� ������ �ִ� �Ÿ�
		float _hitMaxDistance;

		// ����� �迭 �����̶�� .. �ʿ��� �ɼ��� ���� ������ �ӵ� ?
	};

	class PlayerState_Attack : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Attack(DUOLClient::Player* player);

		virtual ~PlayerState_Attack();

	private:
		/**
		 * \brief ��� �⺻ �� ���� �޺��� Ʈ��
		 */
		BinaryTree<Player_NormalAttack> _swordComboTree;

		/**
		 * \brief ��� �⺻ �ָ� ���� �޺��� �迭
		 */
		BinaryTree<Player_NormalAttack> _fistComboTree;

		/**
		 * \brief ���� �÷��̾ ���� ���� �޺� ���
		 */
		BinaryTree<Player_NormalAttack>* _currentComboTreeNode;

		/**
		 * \brief ���� �޺� ���
		 */
		BinaryTree<Player_NormalAttack>* _nextComboTreeNode;

		/**
		 * \brief ĵ�� ������ ������ ����
		 */
		bool _isInCancle;

		/**
		 * \brief ĵ�� ������ ������ �����մϴ�.
		 */
		void StartCancleFrame();

		/**
		 * \brief ĵ�� ������ ������ �����մϴ�.
		 */
		void EndCancleFrame();

		/**
		 * \brief ĵ�� �������� ���� ��, ���� �޺����� ��ȯ�� üũ�մϴ�.
		 */
		void CheckCanEnterNextAttack();

		/**
		 * \brief TODO : ��ȹ���� ������ ������ �޾Ƽ� �޺� Ʈ���� �����մϴ�.
		 */
		void BuildComboTree();

		/**
		 * \brief ���� ���� ������Ʈ�� �°� �ִϸ��̼� �Ķ���� ���� ��ȯ�մϴ�.
		 */
		void SettingCurrentComboNodeState();

		/**
		 * \brief ������ ����˴ϴ�. ������ ����� �����մϴ�.
		 */
		void EndAttack();

#pragma region ������ �迭 ���� �Լ�
		/**
		 * \brief ������ �ϰ��� ������ �ǽ�Ʈ ���� ����
		 */
		void FistHit();

		/**
		 * \brief ����� �迭 ���� ���� (��� ������ �ϰ�)
		 */
		void WaveHit();
#pragma endregion

		DUOLGameEngine::CoroutineHandler SetPostDelay(float delayTime);

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}