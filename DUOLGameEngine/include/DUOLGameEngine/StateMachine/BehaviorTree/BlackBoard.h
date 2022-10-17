/**

	@file    BlackBoard.h
	@brief   Behavior Tree에 사용되는 Data들을 기록하는 클래스
	@details -
	@author  JKim
	@date    17.10.2022

**/
#pragma once
#include <map>
#include <string>
#include <functional>

namespace DUOLGameEngine
{
	/**

		@class   BlackBoard
		@brief   Behavior Tree에 사용되는 Data들을 기록하는 클래스
		@details -

	**/
	class BlackBoard
	{
		struct Data
		{
			void* _data;
			std::function<void(void*)> _destructor;
		};

		using TypeKey = unsigned int;
		using DataStructure = std::map<std::string, Data>;

		std::map<TypeKey, DataStructure> _dataTable;

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

		/**
			@brief	 DataTable에 Data 저장
			@details -
			@tparam  T       - 저장할 DataType
			@param   data    - 저장할 Data
			@param   keyName - 저장할 Data의 Key 값
		**/
		template<typename T>
		void Push(T data, const std::string& keyName);

		/**
			@brief	 DataTable로부터 Data 받기
			@details -
			@tparam  T       - 가져올 DataType
			@param   keyName - 가져올 Data의 Key 값
			@retval  가져올 Data
		**/
		template<typename T>
		T Get(const std::string& keyName);

		/**
			@brief	 DataTable로부터 Data 꺼내오기
			@details -
			@tparam  T       - 가져올 DataType
			@param   keyName - 가져올 Data의 Key 값
			@retval  가져올 Data
		**/
		template<typename T>
		T Pop(const std::string& keyName);

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
	inline void BlackBoard::Push(T data, const std::string& keyName)
	{
		static std::function<void(void*)> destructor = [](void* data)
		{
			delete static_cast<T*>(data);
		};

		_dataTable[GetTypeKey<T>()][keyName] = { static_cast<void*>(new T(data)), destructor };
	}

	template<typename T>
	inline T BlackBoard::Get(const std::string& keyName)
	{
		auto result = _dataTable[GetTypeKey<T>()].find(keyName);

		if (result == _dataTable[GetTypeKey<T>()].end())
			return T();

		return *static_cast<T*>(result->second._data);
	}

	template<typename T>
	inline T BlackBoard::Pop(const std::string& keyName)
	{
		auto result = _dataTable[GetTypeKey<T>()].find(keyName);

		if (result == _dataTable[GetTypeKey<T>()].end())
			return T();

		Data data = result->second;

		_dataTable[GetTypeKey<T>()].erase(result);

		T ret = *static_cast<T*>(data._data);

		data._destructor(data._data);

		return ret;
	}

	template<typename T>
	inline BlackBoard::TypeKey BlackBoard::GetTypeKey()
	{
		static T* typePtr;

		return (TypeKey)&typePtr;
	}
}