#pragma once
#include "DUOLJson/Document.h"
#include "document.h"

/*
 * if (std::is_same<int, T>::value)
 * ���� ���� �ڵ带 Ȱ���� ���ø� �Լ��� ����������.. �� ��ӵ� ����ϰ����.. ��� �ؾ��ϳ� 
*/
namespace DUOLJson
{
 /**

     @class   JsonDocument
     @brief   ������ Document�� ����� jsonDoc
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