/**
	@file    BlackBoard.h
	@brief   Behavior Tree에 사용되는 Data들을 기록하는 클래스
	@details -
	@author  JKim
	@date    17.10.2022
**/
#pragma once
#include <map>
#include <functional>

#include "DUOLCommon/StringHelper.h"

namespace DUOLGameEngine
{
	/**
		@class   BlackBoard
		@brief   Behavior Tree에 사용되는 Data들을 기록하는 클래스
		@details -
	**/
	class BlackBoard
	{
		/**
			@struct  Data
			@brief	 Data와 Data의 소멸자를 가지는 구조체
			@details -
		**/
		struct DataStructure
		{
			std::map<DUOLCommon::tstring, void*> _dataStructure;
			std::function<void(void*)> _destructor;
		};

		using TypeKey = unsigned long long;

	public:
		/**
			@brief   BlackBoard 클래스 default 생성자
			@details -
		**/
		BlackBoard() = default;

		/**
			@brief   BlackBoard 클래스 소멸자
			@details Data Type의 소멸자를 호출하여 DataTable 정리
		**/
		~BlackBoard();

	private:
		std::map<TypeKey, DataStructure> _dataTable;

	public:
		/**
			@brief	 DataTable에 Data 저장
			@details -
			@tparam  T       - DataType
			@param   data    - Data
			@param   keyName - Data의 Key 값
		**/
		template<typename T>
		void Push(T data, const DUOLCommon::tstring& keyName);

		/**
			@brief	 DataTable로부터 Data 받기
			@details -
			@tparam  T       - DataType
			@param   keyName - Data의 Key 값
			@retval  Data
		**/
		template<typename T>
		T& Get(const DUOLCommon::tstring& keyName);

		/**
			@brief	 DataTable로부터 Data 꺼내오기
			@details -
			@tparam  T       - DataType
			@param   keyName - Data의 Key 값
			@retval  Data
		**/
		template<typename T>
		T Pop(const DUOLCommon::tstring& keyName);

	private:
		/**
			@brief	 DataType에 따른 고유한 Key 값
			@details -
			@tparam  T - DataType
			@retval  고유한 Key 값
		**/
		template<typename T>
		TypeKey GetTypeKey();
	};

	template<typename T>
	inline void BlackBoard::Push(T data, const DUOLCommon::tstring& keyName)
	{
		static std::function<void(void*)> destructor = [](void* data)
		{
			delete static_cast<T*>(data);
		};

		auto& targetStructure = _dataTable[GetTypeKey<T>()]._dataStructure;
		auto result = targetStructure.find(keyName);

		if (result != targetStructure.end())
		{
			T& savedData = *static_cast<T*>(result->second);
			savedData = data;
		}
		else
		{
			_dataTable[GetTypeKey<T>()]._dataStructure[keyName] = static_cast<void*>(new T(data));
			_dataTable[GetTypeKey<T>()]._destructor = destructor;
		}
	}

	template<typename T>
	inline T& BlackBoard::Get(const DUOLCommon::tstring& keyName)
	{
		auto& targetStructure = _dataTable[GetTypeKey<T>()]._dataStructure;
		auto result = targetStructure.find(keyName);

		if (result == targetStructure.end())
			throw "No Data";

		return *static_cast<T*>(result->second);
	}

	template<typename T>
	inline T BlackBoard::Pop(const DUOLCommon::tstring& keyName)
	{
		auto& targetStructure = _dataTable[GetTypeKey<T>()]._dataStructure;
		auto result = targetStructure.find(keyName);

		if (result == targetStructure.end())
			throw "No Data";

		void* data = result->second;

		targetStructure.erase(result);

		T ret = *static_cast<T*>(data);

		_dataTable[GetTypeKey<T>()]._destructor(data);

		return ret;
	}

	template<typename T>
	inline BlackBoard::TypeKey BlackBoard::GetTypeKey()
	{
		static T* typePtr;

		return (TypeKey)&typePtr;
	}
}