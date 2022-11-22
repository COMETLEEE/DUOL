#include "DUOLJson/JsonReader.h"

#include "DUOLJson/filereadstream.h"
#include "DUOLJson/JsonAssert.h"
#include "DUOLJson/error/en.h"

namespace DUOLJson
{
	std::unique_ptr<DUOLJson::JsonReader, JsonReader::Deleter> JsonReader::_instance;
	std::once_flag JsonReader::_flag;

	DUOLJson::JsonReader::JsonReader()
	{
		//8개정도만 일단 예약.
		_documents.reserve(8);
	}

	JsonReader* const DUOLJson::JsonReader::GetInstance()
	{
		std::call_once(JsonReader::_flag, []()
			{
				_instance.reset(new JsonReader);
			});

		return _instance.get();
	}

	void DUOLJson::JsonReader::Release()
	{
		_instance.release();

		return;
	}

	DUOLJson::TDocument* JsonReader::LoadJson(const DUOLCommon::tstring& jsonPath)
	{
		if (_documents.size() > 0)
		{
			auto foundDoc = _documents.find(jsonPath);

			if (foundDoc != _documents.end())
			{
				return foundDoc->second.get();
			}
		}
		FILE* fp = nullptr;

#if defined(UNICODE)
		errno_t err = _wfopen_s(&fp, jsonPath.c_str(), _T("rb"));
#else
		errno_t err = _fopen_s(&fp_, jsonPath.c_str(), _T("rb"));
#endif

		if (err) 
		{
			DUOLJSON_ASSERT(_T("JSON file load error : %d\n"), err);
		}

		char readBuffer[23768];

		rapidjson::FileReadStream readStream{ fp, readBuffer, sizeof(readBuffer) };
		rapidjson::AutoUTFInputStream<unsigned, rapidjson::FileReadStream> eis(readStream);  // wraps bis into eis

		auto document = std::make_unique<DUOLJson::TDocument>();

		rapidjson::ParseResult result = document->ParseStream<0, rapidjson::AutoUTF<unsigned>>(eis);

		if (!result)
		{
			DUOLJSON_ASSERT(_T("JSON parse error: %s (%u)\n"), GetParseError_En(result.Code()), result.Offset());
			return nullptr;
		}

		fclose(fp);

		_documents.emplace(jsonPath, std::move(document));

		return _documents.find(jsonPath)->second.get();
	}

	void JsonReader::UnloadJson(const DUOLCommon::tstring& jsonPath)
	{
		_documents.erase(jsonPath);
	}

}
