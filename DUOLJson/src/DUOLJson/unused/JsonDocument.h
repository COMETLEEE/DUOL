#pragma once
#include "DUOLJson/Document.h"
#include "document.h"

/*
 * if (std::is_same<int, T>::value)
 * 위와 같은 코드를 활용한 템플릿 함수를 만들고싶은데.. 또 상속도 사용하고싶음.. 어떻게 해야하나 
*/
namespace DUOLJson
{
 /**

     @class   JsonDocument
     @brief   노출할 Document를 상속한 jsonDoc
     @details ~
     @author  KyungMin Oh

 **/
	class JsonDocument : public Document
	{

	public:
		JsonDocument(const std::string& file);
		virtual ~JsonDocument() override = default;

		virtual bool GetBoolObjectInfo(const std::string& objectID, const std::string& memberKey, bool& result) override;

		virtual bool GetIntObjectInfo(const std::string& objectID, const std::string& memberKey, int& result) override;

		virtual bool GetStringObjectInfo(const std::string& objectID, const std::string& memberKey, std::string& result) override;

		virtual bool GetArrayStringObjectInfo(const std::string& objectID, const std::string& memberKey, std::vector<const char*>& result) override;

	private:
		rapidjson::Document _document;

		rapidjson::GenericValue<rapidjson::UTF8<>> _currentNode;

	};
}