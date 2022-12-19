/**

	@file      HotKey.h
	@brief     단축키를 등록하기 위한 클래스.
	@details   ~
	@author    SinSeongHyeon
	@date      19.12.2022
	@copyright © SinSeongHyeon, 2022. All right reserved.

**/
#pragma once

#include <tuple>
#include <functional>
class HotKey
{
protected:
	HotKey() = default;

	~HotKey() = default;
public:
	static HotKey& Get();
	/**
		 @brief 핫키를 등록하는 함수.
	**/
	void RegisterHotKey(unsigned int virtualKey, unsigned int Modifiers, std::function<void()> func);
	/**
		@brief 프로시저에서 업데이트를 돌릴 함수.
	**/
	void UpdateHotKey(unsigned int message);

private:
	/**
		@brief ID, 함수 객체를 관리하는 변수.
	**/
	std::vector < std::pair<unsigned int, std::function<void()>> > _registedHotKey;
	/**
		@brief ID 변수.
	**/
	unsigned int _keyID = 0;
};

