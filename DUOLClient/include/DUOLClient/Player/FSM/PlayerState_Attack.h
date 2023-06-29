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

	constexpr float OVERDRIVE_ATTACK_SPEED_SWORD = 1.1f;

	constexpr float OVERDRIVE_ATTACK_SPEED_FIST = 0.9f;


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

		// ���̺� �迭 ������ ���� �ڽ��� ũ��
		DUOLMath::Vector3 _startWaveBoxHalfExtents;

		// ���̺� �迭 ������ ������ �ڽ��� ũ��
		DUOLMath::Vector3 _endWaveBoxHalfExtents;

		// ī�޶� ����ũ ��� ����
		bool _useCamShake = false;

		// ī�޶� ����ũ �Ŀ�
		DUOLMath::Vector2 _shakePower = DUOLMath::Vector2::Zero;
	};

	class PlayerState_Attack : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Attack(DUOLClient::Player* player);

		virtual ~PlayerState_Attack();

	private:
		/**������
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

		/**
		 * \brief ������ Ÿ�� ���� ����Ʈ IDX
		 */
		PlayerSoundTable _currentAudioClip;

		DUOLGameEngine::ParticleRenderer* _leftFistFormAura;

		DUOLGameEngine::ParticleRenderer* _rightFistFormAura;

		DUOLGameEngine::ParticleRenderer* _normalSwordTrail;

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
		 * \brief �⺻ �� Ʈ���� ����Ʈ Ȱ��ȭ
		 */
		void StartSwordTrailFrame();

		/**
		 * \brief �⺻ �� Ʈ���� ����Ʈ ��Ȱ��ȭ
		 */
		void EndSwordTrailFrame();

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

		/**
		* \brief ��������̺� �ҵ� ���ǵ弳��;
		*/
		void OverdriveSwordDefaultSpeed();

		/**
		* \brief ��������̺� �ҵ� ���ǵ弳��;
		*/
		void OverdriveFistDefaultSpeed();

#pragma endregion

		// �ĵ� 
		DUOLGameEngine::CoroutineHandler SetPostDelay(float delayTime);

		// ��Ÿ ���� �� �뽬���� �� �ϵ��� ����
		DUOLGameEngine::CoroutineHandler SetCannotDash();

		DUOLGameEngine::CoroutineHandler LaunchWave();

		DUOLGameEngine::CoroutineHandler LaunchAreaWave();

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}
