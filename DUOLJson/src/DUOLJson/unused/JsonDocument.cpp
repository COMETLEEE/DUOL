#include "JsonDocument.h"
#include "rapidjson.h"


DUOLJson::JsonDocument::JsonDocument(const std::string& file)
{
	_document.Parse(file.c_str());
}

bool DUOLJson::JsonDocument::GetBoolObjectInfo(const std::string& objectID, const std::string& memberKey, bool& result)
{
	if (_document.HasMember(objectID.c_str()))
	{
		auto& object = _document[objectID.c_str()];

		if (object.HasMember(memberKey.c_str()))
		{
			auto& member = object[memberKey.c_str()];

			if (member.IsBool())
			{
				result = member.GetBool();

				return true;
			}
		}
	}

	return false;
}

bool DUOLJson::JsonDocument::GetIntObjectInfo(const std::string& objectID, const std::string& memberKey, int& result)
{
	if (_document.HasMember(objectID.c_str()))
	{
		auto& object = _document[objectID.c_str()];

		if (object.HasMember(memberKey.c_str()))
		{
			auto& member = object[memberKey.c_str()];

			if (member.IsInt())
			{
				result = member.GetInt();

				return true;
			}
		}
	}

	return false;
}

bool DUOLJson::JsonDocument::GetStringObjectInfo(const std::string& objectID, const std::string& memberKey,
	std::string& result)
{
	if (_document.HasMember(objectID.c_str()))
	{
		auto& object = _document[objectID.c_str()];

		if (object.HasMember(memberKey.c_str()))
		{
			auto& member = object[memberKey.c_str()];

			if (member.IsString())
			{
				result = member.GetString();

				return true;
			}
		}
	}

	return false;
}

bool DUOLJson::JsonDocument::GetArrayStringObjectInfo(const std::string& objectID, const std::string& memberKey,
	std::vector<const char*>& result)
{
	if (_document.HasMember(objectID.c_str()))
	{
		auto& object = _document[objectID.c_str()];

		if (object.HasMember(memberKey.c_str()))
		{
			auto& member = object[memberKey.c_str()];

			if (member.IsArray())
			{
				result.reserve(member.Size());

				for (auto& arr : member.GetArray())
				{
					if (arr.IsString())
					{
						result.emplace_back(arr.GetString());
					}
				}

				return true;
			}
		}
	}

	return false;
}

