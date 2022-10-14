/**

    @file    BlackBoard.h
    @brief   Behavior Tree에 사용되는 Data들을 기록하는 클래스
    @details -
    @author  JKim
    @date    17.10.2022

**/
#pragma once
#include <string>
#include <map>

namespace DUOLGameEngine
{
 /**

     @class   BlackBoard
     @brief   Behavior Tree에 사용되는 Data들을 기록하는 클래스
     @details -

 **/
	class BlackBoard
	{
        using Data = void;
        using TypeKey = unsigned int;
        using DataStructure = std::map<std::string, Data*>;

        std::map<TypeKey, DataStructure> _dataTable;

    public:

        BlackBoard() = default;


        ~BlackBoard() = default;

        template<typename T>
        void Push(T data, const std::string& keyName);

        template<typename T>
        const T& Get(const std::string& keyName);

        template<typename T>
        T Pop(const std::string& keyName);
        
    private:
        template<typename T>
        TypeKey GetTypeKey();
	};

    template<typename T>
    inline void BlackBoard::Push(T data, const std::string& keyName)
    {
        _dataTable[GetTypeKey<T>()][keyName] = static_cast<Data*>(new T(data));
    }

    template<typename T>
    inline const T& BlackBoard::Get(const std::string& keyName)
    {
        auto result = _dataTable[GetTypeKey<T>()].find(keyName);

        if (result == _dataTable[GetTypeKey<T>()].end())
            return T();

        return *static_cast<T*>(result->second);
    }

    template<typename T>
    inline T BlackBoard::Pop(const std::string& keyName)
    {
        auto result = _dataTable[GetTypeKey<T>()].find(keyName);

        if (result == _dataTable[GetTypeKey<T>()].end())
            return T();

        T ret = *static_cast<T*>(result->second);

        _dataTable[GetTypeKey<T>()].erase(result);

        return ret;
    }

    template<typename T>
    inline BlackBoard::TypeKey BlackBoard::GetTypeKey()
    {
        static T* typePtr;

        return (TypeKey)&typePtr;
    }
}