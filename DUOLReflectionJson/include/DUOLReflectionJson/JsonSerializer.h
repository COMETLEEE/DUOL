#pragma once
#define RAPIDJSON_HAS_STDSTRING 1

#include <rapidjson/prettywriter.h>
#include <rapidjson/document.h>

#include <rttr/type>
#include <rttr/method.h>

#include "DUOLReflectionJson/Export_ReflectionJson.h"
#include "DUOLCommon/StringHelper.h"
#include "DUOLCommon/Util/UUID.h"

// TODO : Common 으로 빼자 ..!
namespace DUOLGameEngine
{
	class ObjectBase;
}

namespace DUOLReflectionJson
{
	using namespace rapidjson;
	using namespace rttr;

	/**
	 * \brief Json Write & Read With RTTR DUOL assets.
	 */
	class DUOL_REFLECTION_JSON_API JsonSerializer
	{
	public:
		JsonSerializer();

		~JsonSerializer();

		JsonSerializer(JsonSerializer&) = delete;

		JsonSerializer& operator =(JsonSerializer&) = delete;

		JsonSerializer(JsonSerializer&&) = delete;

		JsonSerializer& operator =(JsonSerializer&&) = delete;

		// 포인터 및 이미 있는 개체의 맵핑을 위한 함수들
		rttr::method _getUUID;

		rttr::method _getString;

		rttr::method _getAddress;

		std::function<DUOLGameEngine::ObjectBase*(DUOLCommon::UUID)> _uuidObjectFunc;

		// String Mapping 은 이미 준비되어 있는 녀석들에 대해서 검색하는 용도이므로 JSON 파일을 읽을 때와 동시에 진행됩니다.
		std::function<DUOLGameEngine::ObjectBase* (DUOLCommon::tstring&)> _stringObjectFunc;

		std::string _typeName;

	private:
		// ----------------------- Read -----------------------
		void FromJsonRecursively(rttr::instance object, Value& jsonObject);

		void UUIDMappingRecursively(rttr::instance object);

		void UUIDMappingSequentialRecursively(const rttr::variant_sequential_view& view);

		void UUIDMappingAssociativeRecursively(const rttr::variant_associative_view& view);

		void WriteSequentialRecursively(variant_sequential_view& view, Value& jsonValue, bool isSerializedByUUID = false);

		void WriteSequentialTString(variant_sequential_view& view, Value& jsonValue);

		void WriteAssociativeViewRecursively(variant_associative_view& view, Value& jsonValue);

		rttr::variant ExtractValue(Value::MemberIterator& iter, const type& t);

		rttr::variant ExtractBasicTypes(Value& jsonValue);

		// Key : UUID / Value : 그 녀석이 가르키는 객체의 포인터들의 Map.
		std::unordered_map<DUOLCommon::UUID, void*> _uuidInstanceMap;











		// ----------------------- Write -----------------------
		bool WriteVariant(const rttr::variant& var, PrettyWriter<StringBuffer>& writer);

		bool WritePrimitiveTypes(const rttr::type& t, const rttr::variant& var, PrettyWriter<StringBuffer>& writer);

		void WriteSequentialContainer(const rttr::variant_sequential_view& view, PrettyWriter<StringBuffer>& writer);

		void WriteAssociativeContainer(const rttr::variant_associative_view& view, PrettyWriter<StringBuffer>& writer);

		void ToJsonRecursively(const rttr::instance& target, PrettyWriter<StringBuffer>& writer);

		void WriteUUIDSequentialContainer(const rttr::variant& var, PrettyWriter<StringBuffer>& writer);

		void WriteUUIDAssociativeContainer(const rttr::variant& var, PrettyWriter<StringBuffer>& writer);

		void WriteStringSequentialContainer(const rttr::variant& var, PrettyWriter<StringBuffer>& writer);

		void WriteStringAssociativeContainer(const rttr::variant& var, PrettyWriter<StringBuffer>& writer);

	public:
		/**
		 * \brief Deserializer json file from this.
		 * \param filePath The path of the file to deserializer.
		 * \param object 
		 * \return Success => True / Failed => false
		 */
		bool FromJson(const DUOLCommon::tstring& filePath, rttr::instance object);

		/**
		 * \brief Serialize 'rttr::instance' by rapidjson.
		 * \param object The object to serialize.
		 * \return The content of the json file.
		 */
		std::string ToJson(rttr::instance object);

		void SetUUIDObjectFunc(std::function<DUOLGameEngine::ObjectBase*(DUOLCommon::UUID)> uuidObjectFunc);

		void SetStringObjectFunc(std::function<DUOLGameEngine::ObjectBase*(DUOLCommon::tstring&)> tstringObjectFunc);
	};
}
