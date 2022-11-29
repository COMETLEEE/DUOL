#pragma once
#include <cassert>
#include <memory>
#include <string>
#include <vector>

#include "DUOLCommon/StringHelper.h"
#include "DUOLJson/Export.h"


namespace DUOLJson
{
	class DUOLJSON_EXPORT Document 
	{
	public:
		Document() = default;
		virtual  ~Document() = default;


	protected:

		virtual bool GetBoolObjectInfo(const std::string& objectID, const std::string& memberKey, bool& result) abstract;

		virtual bool GetIntObjectInfo(const std::string& objectID, const std::string& memberKey, int& result) abstract;

		virtual bool GetStringObjectInfo(const std::string& objectID, const std::string& memberKey, std::string& result) abstract;

		virtual bool GetArrayStringObjectInfo(const std::string& objectID, const std::string& memberKey, std::vector<const char*>& result) abstract;


	};
}
