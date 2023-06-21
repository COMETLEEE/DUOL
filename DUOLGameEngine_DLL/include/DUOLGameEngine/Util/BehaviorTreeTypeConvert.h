#pragma once
#include "behaviortree_cpp/basic_types.h"
#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLMath/DUOLMath.h"
#include "DUOLCommon/StringHelper.h"

namespace BT
{

	template <>
	inline DUOLMath::Vector4 convertFromString(StringView str)
	{
		//DUOL_TRACE(DUOL_CONSOLE, "Converting string: \"{0}\"\n", str.data());

		auto parts = splitString(str, ';');
		if (parts.size() != 4)
		{
			DUOL_CRITICAL("invalid input");
		}
		else
		{
			DUOLMath::Vector4 output;
			output.x = convertFromString<float>(parts[0]);
			output.y = convertFromString<float>(parts[1]);
			output.z = convertFromString<float>(parts[2]);
			output.w = convertFromString<float>(parts[3]);
			return output;
		}
	}

	template <>
	inline DUOLMath::Vector3 convertFromString(StringView str)
	{
		//DUOL_TRACE(DUOL_CONSOLE, "Converting string: \"{0}\"\n", str.data());

		auto parts = splitString(str, ';');
		if (parts.size() != 3)
		{
			DUOL_CRITICAL("invalid input");
		}
		else
		{
			DUOLMath::Vector3 output;
			output.x = convertFromString<float>(parts[0]);
			output.y = convertFromString<float>(parts[1]);
			output.z = convertFromString<float>(parts[2]);
			return output;
		}
	}

	template <>
	inline DUOLMath::Vector2 convertFromString(StringView str)
	{
		//DUOL_TRACE(DUOL_CONSOLE, "Converting string: \"{0}\"\n", str.data());

		auto parts = splitString(str, ';');
		if (parts.size() != 2)
		{
			DUOL_CRITICAL("invalid input");
		}
		else
		{
			DUOLMath::Vector2 output;
			output.x = convertFromString<float>(parts[0]);
			output.y = convertFromString<float>(parts[1]);
			return output;
		}
	}

	template <>
	inline DUOLCommon::tstring convertFromString(StringView str)
	{
		//DUOL_TRACE(DUOL_CONSOLE, "Converting string: \"{0}\"\n", str.data());

		DUOLCommon::tstring result = DUOLCommon::tstring(DUOLCommon::StringHelper::StringToWString(str.data()));

		return result;
	}


}   // end namespace BT