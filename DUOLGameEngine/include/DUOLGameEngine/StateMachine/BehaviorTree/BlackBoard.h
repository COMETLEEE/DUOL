/**

	@file    BlackBoard.h
	@brief   Behavior Tree�� ���Ǵ� Data���� ����ϴ� Ŭ����
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
		@brief   Behavior Tree�� ���Ǵ� Data���� ����ϴ� Ŭ����
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
			@brief   BlackBoard Ŭ���� default ������
			@details -
		**/
		BlackBoard() = default;

		/**
			@brief   BlackBoard Ŭ���� �Ҹ���
			@details Data Type�� �Ҹ��ڸ� ȣ���Ͽ� DataTable ����
		**/
		~BlackBoard();

		/**
			@brief	 DataTable�� Data ����
			@details -
			@tparam  T       - ������ DataType
			@param   data    - ������ Data
			@param   keyName - ������ Data�� Key ��
		**/
		template<typename T>
		void Push(T data, const std::string& keyName);

		/**
			@brief	 DataTable�κ��� Data �ޱ�
			@details -
			@tparam  T       - ������ DataType
			@param   keyName - ������ Data�� Key ��
			@retval  ������ Data
		**/
		template<typename T>
		T Get(const std::string& keyName);

		/**
			@brief	 DataTable�κ��� Data ��������
			@details -
			@tparam  T       - ������ DataType
			@param   keyName - ������ Data�� Key ��
			@retval  ������ Data
		**/
		template<typename T>
		T Pop(const std::string& keyName);

	private:
		/**
			@brief	 DataType�� ���� ������ Key ��
			@details -
			@tparam  T - DataType
			@retval  ������ Key ��
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