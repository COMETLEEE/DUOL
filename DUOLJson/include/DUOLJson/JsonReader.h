#pragma once
#include <memory>
#include <mutex>
#include <unordered_map>

#include "DUOLCommon/StringHelper.h"
#include "DUOLJson/document.h"
#include "DUOLJson/Export.h"
#include "DUOLJson/JsonMacro.h"

namespace DUOLJson
{
	using JsonUnicode = rapidjson::UTF8<TCHAR>;
	using TDocument = rapidjson::GenericDocument<JsonUnicode>;

/**
@class   JsonReader
@brief   jsonReader ???? ????? ???? rapidjson
@details ~
@author  KyungMin Oh
**/
	class DUOLJSON_EXPORT JsonReader
	{
	protected:
		JsonReader();
		~JsonReader() = default;

	public:
		JsonReader(const JsonReader& other) = delete;
		JsonReader& operator =(const JsonReader& other) = delete;

	public:
		struct DUOLJSON_EXPORT Deleter
		{
			void operator()(JsonReader* instance)
			{
				delete instance;
			}
		};

		//singletone
	private:
		static std::unique_ptr<JsonReader, Deleter> _instance;

		static std::once_flag _flag;

		std::unordered_map<DUOLCommon::tstring, std::unique_ptr<DUOLJson::TDocument>> _documents;

	public:
		static  JsonReader* const GetInstance();

		static void Release();

	public:
		DUOLJson::TDocument* LoadJson(const DUOLCommon::tstring& jsonPath);

		void UnloadJson(const DUOLCommon::tstring& jsonPath);
		bool WriteJson(const DUOLCommon::tstring& jsonPath, rapidjson::Document& document);
	};

}
