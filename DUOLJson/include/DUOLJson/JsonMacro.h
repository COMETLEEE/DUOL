#pragma once
#include <utility>
#include "DUOLCommon/StringHelper.h"

#define JSON_SERIALIZE_ENUM(ENUM_TYPE, ...)														\
	inline bool StringToEnum(const DUOLCommon::tstring& str, ENUM_TYPE& e)                       \
	{                                                                                           \
		static_assert(std::is_enum<ENUM_TYPE>::value, #ENUM_TYPE "must be Enum");				\
		static const std::pair<ENUM_TYPE, DUOLCommon::tstring> m[] = __VA_ARGS__;						\
		auto it = std::find_if(std::begin(m), std::end(m),										\
		[&str](const std::pair<ENUM_TYPE, DUOLCommon::tstring>& enums) -> bool							\
		{                                                                                       \
			return enums.second == str;                                                         \
		});                                                                                     \
																								\
		if(it != std::end(m))																	\
		{																						\
			e = it->first;																		\
			return true;																		\
		}																						\
		else																					\
		{																						\
			return false;																		\
		}																						\
	};		                                                                                     \
																								\
	inline bool EnumToString(const ENUM_TYPE& e, DUOLCommon::tstring& str)                              \
	{                                                                                           \
		static_assert(std::is_enum<ENUM_TYPE>::value, #ENUM_TYPE "must be Enum");		        \
		static const std::pair<ENUM_TYPE, DUOLCommon::tstring> m[] = __VA_ARGS__;					    \
		auto it = std::find_if(std::begin(m), std::end(m),										\
		[e](const std::pair<ENUM_TYPE, DUOLCommon::tstring>& enums) -> bool								\
		{                                                                                       \
			return enums.first == e;															\
		});																						\
																								\
		if(it != std::end(m))																	\
		{																						\
			str = it->second;																		\
			return true;																		\
		}																						\
		else																					\
		{																						\
			return false;																		\
		}																						\
	};	                                                                                        \
