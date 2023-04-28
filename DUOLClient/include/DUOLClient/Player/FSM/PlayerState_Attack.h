#pragma once
#include "PlayerStateBase.h"

#include "DUOLClient/Util/BinaryTree.h"
#include "DUOLGameEngine/ECS/Object/AnimatorController/AnimatorController.h"

namespace DUOLGameEngine
{
	class ParticleRenderer;
}

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
		, SWORD_WAVE
		, FIST_WAVE
		, SWORD_AREA_WAVE
	};

	/**
	 * \brief �÷��̾� �⺻ ���� ������. ������ �ִϸ��̼ǵ��� �� ����Ʈ�� ������ �ִ�.
	 */
	struct Player_AttackData
	{
		Player_AttackType _attackType;

		// �ִϸ����� �Ķ���� �̸� / �ִϸ����� �Ķ���� Ÿ�� / ������ ��
		std::vector<std::tuple<DUOLCommon::tstring, DUOLGameEngine::AnimatorControllerParameterType, std::any>> _animatorParameterTable;

		// �÷��̾� �߽����κ��� Local ���� ���� Forward, Right, Up Offset
		DUOLMath::Vector3 _hitCenterOffset;

		// ���� ������
		float _hitRadius;

		// ���� �ִ� �Ÿ�
		float _hitMaxDistance;

		// ���̺� �迭 ������ �ӵ�
		DUOLMath::Vector3 _waveVelocity;

		// ���̺� �迭 ������ ���� �ð�
		float _waveTime;

		// ���̺� �迭 ������ �ڽ��� ũ��
		DUOLMath::Vector3 _waveBoxhalfExtents;
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
		BinaryTree<Player_AttackData> _swordComboTree;

		/**
		 * \brief ��� �⺻ �ָ� ���� �޺��� Ʈ��
		 */
		BinaryTree<Player_AttackData> _fistComboTree;

		/**
		 * \brief ��� ��������̺� ���� ���� �޺��� Ʈ��
		 */
		BinaryTree<Player_AttackData> _overdriveSwordComboTree;

		/**
		 * \brief ��� ��������̺� ���� ���� �޺��� Ʈ��
		 */
		BinaryTree<Player_AttackData> _overdriveFistComboTree;

		/**
		 * \brief ���� �÷��̾ ���� ���� �޺� ���
		 */
		BinaryTree<Player_AttackData>* _currentComboTreeNode;

		/**
		 * \brief ���� �޺� ���
		 */
		BinaryTree<Player_AttackData>* _nextComboTreeNode;

		/**
		 * \brief ĵ�� ������ ������ ����
		 */
		bool _isInCancle;

		DUOLGameEngine::ParticleRenderer* _leftFistFormAura;

		DUOLGameEngine::ParticleRenderer* _rightFistFormAura;

		/**
		 * \brief ĵ�� ������ ������ �����մϴ�.
		 */
		void StartCancleFrame();

		/**
		 * \brief ��� �ӵ��� ������ �ϴ� �������� �����մϴ�.
		 */
		void StartSlowFrame();

		/**
		 * \brief ĵ�� ������ ������ �����մϴ�.
		 */
		void EndCancleFrame();

		/**
		 * \brief �⺻ �� Ȱ��ȭ
		 */
		void StartSwordAttackFrame();

		/**
		 * \brief �⺻ �� ��Ȱ��ȭ
		 */
		void EndSwordAttackFrame();

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

		/**
		 * \brief ��������̺� �ҵ� ��Ÿ ����
		 */
		void AreaWaveHit();
#pragma endregion

		// �ĵ� 
		DUOLGameEngine::CoroutineHandler SetPostDelay(float delayTime);

		DUOLGameEngine::CoroutineHandler LaunchWave();

		DUOLGameEngine::CoroutineHandler LaunchAreaWave();

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}
