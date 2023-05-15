/**

	@file      TimeManager.h
	@brief     Time Management Class
	@details   ~
	@author    COMETLEE
	@date      17.10.2022
	@copyright © COMETLEE, 2022. All right reserved.

**/

#pragma once
#include <chrono>
#include <cassert>

#include "DUOLGameEngine/Util/SingletonBase.h"
#include "DUOLGameEngine/Util/Constants.h"
#include "DUOLGameEngine/Util/Defines.h"

namespace DUOLGameEngine
{
	/**
	 * \brief 게임 진행 중 시간에 대한 사항들을 담당하는 매니저
	 */
	class DUOL_GAMEENGINE_API TimeManager : public SingletonBase<TimeManager>
	{
		DECLARE_SINGLETON(TimeManager)

			DELETE_COPY_MOVE(TimeManager)

	private:
		virtual ~TimeManager() override;

	public:
		void Initialize();

		void UnInitialize();

		void Update();

	private:
		std::chrono::high_resolution_clock::time_point _prevTimePoint;

		std::chrono::high_resolution_clock::time_point _currTimePoint;

		/**
		 * \brief 응용 프로그램 시작 후 총 프레임 수
		 */
		uint64_t _frameCount;

		/**
		 * \brief 현재 프레임 기준 1초 당 프레임 수
		 */
		uint32_t _fps;

		/**
		 * \brief 응용 프로그램 시작 후 총합 시간 (초 단위, 시간 스케일에 영향을 받지 않습니다.)
		 */
		float _realtimeSinceStartup;

		/**
		* \brief 응용 프로그램 시작 후 총합 시간 (초 단위, 시간 스케일에 영향을 받습니다.)
		*/
		float _realtimeScaledSinceStartup;

		/**
		 * \brief 지난 프레임과 현재 프레임 간의 시간 간격 (초 단위, TimeScale에 영향을 받는다.)
		 */
		float _deltaTime;

		/**
		 * \brief 지난 프레임과 현재 프레임 간의 시간 간격 (초 단위, TimeScale에 영향을 받지 않는다. == RealTime)
		 */
		float _unscaledDeltaTime;

		/**
		 * \brief 게임 레벨에서 시간이 흐르는 스케일 (1.f => 기본 시간 흐름, 2.f => 2배 빠른 시간,
		 * 0.5f = > 2배 느린 시간)
		 */
		float _timeScale;

	private:
		/**
		 * \brief 시간 변화량에 관한 멤버 변수들을 모두 업데이트합니다.
		 */
		void UpdateDeltaTime();

		/**
		 * \brief Frame Per Sec에 관한 멤버 변수들을 모두 업데이트합니다.
		 */
		void UpdateFPS();

	public:
		inline float GetRealtimeSinceStartup() const { return _realtimeSinceStartup; }

		inline float GetRealtimeScaledSinceStartup() const { return _realtimeScaledSinceStartup; }

		inline float GetDeltaTime() const { return _deltaTime; }

		inline float GetUnscaledDeltaTime() const { return _unscaledDeltaTime; }

		inline float GetTimeScale() const { return _timeScale; }

		inline float GetFPS() const { return _fps; }

		void SetTimeScale(float scale);
	};
}