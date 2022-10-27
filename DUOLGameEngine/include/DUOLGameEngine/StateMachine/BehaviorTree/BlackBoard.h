/**

	@file    BlackBoard.h
	@brief   Behavior Tree�� ���Ǵ� Data���� ����ϴ� Ŭ����
	@details -
	@author  JKim
	@date    17.10.2022

**/
#pragma once
#include <map>
#include <functional>

#include "DUOLGameEngine/Util/Defines.h"

namespace DUOLGameEngine
{
	/**

		@class   BlackBoard
		@brief   Behavior Tree�� ���Ǵ� Data���� ����ϴ� Ŭ����
		@details -

	**/
	class BlackBoard
	{
		/**
			@struct  Data
			@brief	 Data�� Data�� �Ҹ��ڸ� ������ ����ü
			@details -
		**/
		struct Data
		{
			void* _data;
			std::function<void(void*)> _destructor;
		};

		using TypeKey = unsigned int;
		using DataStructure = std::map<DUOLCommon::tstring, Data>;

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

	private:
		std::map<TypeKey, DataStructure> _dataTable;

	public:
		/**
			@brief	 DataTable�� Data ����
			@details -
			@tparam  T       - ������ DataType
			@param   data    - ������ Data
			@param   keyName - ������ Data�� Key ��
		**/
		template<typename T>
		void Push(T data, const DUOLCommon::tstring& keyName);

		/**
			@brief	 DataTable�κ��� Data �ޱ�
			@details -
			@tparam  T       - ������ DataType
			@param   keyName - ������ Data�� Key ��
			@retval  ������ Data
		**/
		template<typename T>
		T& Get(const DUOLCommon::tstring& keyName);

		/**
			@brief	 DataTable�κ��� Data ��������
			@details -
			@tparam  T       - ������ DataType
			@param   keyName - ������ Data�� Key ��
			@retval  ������ Data
		**/
		template<typename T>
		T Pop(const DUOLCommon::tstring& keyName);

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
	inline void BlackBoard::Push(T data, const DUOLCommon::tstring& keyName)
	{
		static std::function<void(void*)> destructor = [](void* data)
		{
			delete static_cast<T*>(data);
		};

		_dataTable[GetTypeKey<T>()][keyName] = { static_cast<void*>(new T(data)), destructor };
	}

	template<typename T>
	inline T& BlackBoard::Get(const DUOLCommon::tstring& keyName)
	{
		auto result = _dataTable[GetTypeKey<T>()].find(keyName);

		if (result == _dataTable[GetTypeKey<T>()].end())
			throw "No Data";

		return *static_cast<T*>(result->second._data);
	}

	template<typename T>
	inline T BlackBoard::Pop(const DUOLCommon::tstring& keyName)
	{
		auto result = _dataTable[GetTypeKey<T>()].find(keyName);

		if (result == _dataTable[GetTypeKey<T>()].end())
			throw "No Data";

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