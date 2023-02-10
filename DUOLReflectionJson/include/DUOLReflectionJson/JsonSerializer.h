#pragma once
#define RAPIDJSON_HAS_STDSTRING 1

#include <rapidjson/prettywriter.h>
#include <rapidjson/document.h>

#include <rttr/type>
#include <rttr/method.h>

#include "DUOLReflectionJson/Export_ReflectionJson.h"
#include "DUOLCommon/StringHelper.h"
#include "DUOLCommon/Util/UUID.h"

namespace DUOLReflectionJson
{
	using namespace rapidjson;
	using namespace rttr;

	/**
	 * \brief Json Write & Read With RTTR
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

		rttr::method _getUUID;

		rttr::method _getAddress;

	private:
		// ----------------------- Read -----------------------
		void FromJsonRecursively(rttr::instance object, Value& jsonObject);

		void UUIDMappingRecursively(rttr::instance object);

		void UUIDMappingSequentialRecursively(const rttr::variant_sequential_view& view);

		void UUIDMappingAssociativeRecursively(const rttr::variant_associative_view& view);

		void WriteSequentialRecursively(variant_sequential_view& view, Value& jsonValue, bool isSerializedByUUID = false);

		void WriteAssociativeViewRecursively(variant_associative_view& view, Value& jsonValue);

		rttr::variant ExtractValue(Value::MemberIterator& iter, const type& t);

		rttr::variant ExtractBasicTypes(Value& jsonValue);

		// UUID 와 그 녀석이 가르키는 녀석
		std::unordered_map<DUOLCommon::UUID, void*> _uuidInstanceMap;







		// ----------------------- Write -----------------------
		bool WriteVariant(const rttr::variant& var, PrettyWriter<StringBuffer>& writer);

		bool WritePrimitiveTypes(const rttr::type& t, const rttr::variant& var, PrettyWriter<StringBuffer>& writer);

		void WriteSequentialContainer(const rttr::variant_sequential_view& view, PrettyWriter<StringBuffer>& writer);

		void WriteAssociativeContainer(const rttr::variant_associative_view& view, PrettyWriter<StringBuffer>& writer);

		void ToJsonRecursively(const rttr::instance& target, PrettyWriter<StringBuffer>& writer);

		void WriteUUIDSequentialContainer(const rttr::variant& var, PrettyWriter<StringBuffer>& writer);

		void WriteUUIDAssociativeContainer(const rttr::variant& var, PrettyWriter<StringBuffer>& writer);





	public:
		/**
		 * \brief Deserializer json file from this.
		 * \param filePath The path of the file to deserializer.
		 * \param object 
		 * \return 
		 */
		bool FromJson(const DUOLCommon::tstring& filePath, rttr::instance object);

		/**
		 * \brief Serialize 'rttr::instance' by rapidjson.
		 * \param object The object to serialize.
		 * \return The content of the json file.
		 */
		std::string ToJson(rttr::instance object);
	};
}
