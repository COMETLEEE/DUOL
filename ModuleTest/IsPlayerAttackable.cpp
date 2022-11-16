#include "IsPlayerAttackable.h"
#include "InputManager.h"

IsPlayerAttackable::IsPlayerAttackable() :
    ConditionNode(_T("PlayerAttackable"))
{

}

bool IsPlayerAttackable::ConditionCheck()
{
    int attackCombo = GetDataFromBlackBoard<int>(_T("AttackCombo"));

    if (InputManager::GetInstance()->GetInputState('A', KeyState::STAY) == true)
    {
        if (attackCombo >= 4)
            attackCombo = 0;

        attackCombo++;

        PushDataToBlackBoard<int>(0, _T("PlayerVelocityX"));
        PushDataToBlackBoard<int>(0, _T("PlayerVelocityY"));
        PushDataToBlackBoard<int>(attackCombo, _T("AttackCombo"));

        return true;
    }

    attackCombo = 0;
    PushDataToBlackBoard<int>(attackCombo, _T("AttackCombo"));

    return false;
}