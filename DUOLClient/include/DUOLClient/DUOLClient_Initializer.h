/**

	@file      DUOLClient_Initializer.h
	@brief     초기화용 함수를 가지고 있는 클래스...! 네임 스페이스 같은 역할을 하는 클래스이다.
	@details   ~ 클래스로 만든 이유는 구현과 선언을 분리하기 위해
	@author    SinSeongHyeon
	@date      13.03.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once

namespace DUOLClient
{
	class DUOLClient_Initializer
	{
	public:
		DUOLClient_Initializer() = delete;

	private:
		static void BehaviorTreeFactory_Initialize();

		static void AnimatorController_Initialize();

		static void Overdrive_Sword_Initialize();

		static void Overdrive_Fist_Initialize();

		static void LoadSound();

		static void Load_Player_Sound();

		static void Load_BGM_Sound();

		static bool _isInit;

	public:
		static void DUOLClient_Initialize();
	};
}