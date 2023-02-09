#include "DUOLReflectionJson/JsonSerializer.h"

#include <fstream>

#include "DUOLCommon/MetaDataType.h"

#include "DUOLCommon/Util/UUID.h"

#include "rapidjson/istreamwrapper.h"

namespace DUOLReflectionJson
{
	JsonSerializer::JsonSerializer() :
		_uuidInstanceMap({})
		, _getUUID(rttr::type::get_by_name("ObjectBase").get_method("GetUUID"))
	{
		// 'std::wstring' => 'std::string'
		rttr::type::register_converter_func([](const std::wstring& value, bool& ok)
			{
				ok = true;

				return std::string(value.begin(), value.end());
			});

		// 'std::string' => 'std::wstring'
		rttr::type::register_converter_func([](const std::string& value, bool& ok)
			{
				ok = true;

				return std::wstring(value.begin(), value.end());
			});

		// 'uint64_t' => 'UUID'
		rttr::type::register_converter_func([](const uint64_t& id, bool& ok)
			{
				ok = true;

				return DUOLCommon::UUID{ id };
			});
	}

	JsonSerializer::~JsonSerializer()
	{
		// ID Mapper delete ?
	}

	void JsonSerializer::FromJsonRecursively(rttr::instance object, Value& jsonObject)
	{
		rttr::instance obj = object.get_type().get_raw_type().is_wrapper() ? object.get_wrapped_instance() : object;

		rttr::type dd = obj.get_derived_type();

		const auto properties = dd.is_valid() ? dd.get_properties() : obj.get_type().get_properties();

		for (auto prop : properties)
		{
			Value::MemberIterator ret = jsonObject.FindMember(prop.get_name().data());

			if (ret == jsonObject.MemberEnd())
				continue;

			const type valueType = prop.get_type();

			auto& jsonValue = ret->value;

			switch (jsonValue.GetType())
			{
				case kArrayType:
				{
					variant var;

					if (valueType.is_sequential_container())
					{
						var = prop.get_value(obj);

						auto sequentialView = var.create_sequential_view();

						WriteSequentialRecursively(sequentialView, jsonValue, (prop.get_metadata(DUOLCommon::MetaDataType::SerializeByUUID) == true));

						unsigned long long size = sequentialView.get_size();
					}
					else if (valueType.is_associative_container())
					{
						var = prop.get_value(obj);

						auto associativeView = var.create_associative_view();

						WriteAssociativeViewRecursively(associativeView, jsonValue);

						unsigned long long  size = 	associativeView.get_size();
					}

					bool ok = prop.set_value(obj, var);

					break;
				}

				case kObjectType:
				{
					// UUID 로 시리얼라이즈 하지 않는데 만약 포인터 타입이라면 ? => 객체를 만들어줍니다
					if ((prop.get_metadata(DUOLCommon::MetaDataType::SerializeByUUID) != true) && (valueType.is_pointer()))
					{
						auto ctor = valueType.get_raw_type().get_constructor();

						variant newObj = ctor.invoke();

						bool ok = prop.set_value(obj, newObj);
					}

					variant var = prop.get_value(obj);

					FromJsonRecursively(var, jsonValue);

					prop.set_value(obj, var);

					break;
				}

				default:
				{
					variant extracted_value = ExtractBasicTypes(jsonValue);

					if (extracted_value.convert(valueType)) // REMARK: CONVERSION WORKS ONLY WITH "const type", check whether this is correct or not!
						prop.set_value(obj, extracted_value);

					// 만약, 해당 프로퍼티가 UUID 였다면 ..!  => Instance를 넣어주고 다음에 매핑할 준비를 해줍니다.
					if (prop.get_name() == "_uuid")
					{
						// UUID - Instance
						_uuidInstanceMap.insert({ jsonValue.GetUint64(), obj} );
					}
				}
			}
		}
	}

	void JsonSerializer::UUIDMappingRecursively(rttr::instance object)
	{
		rttr::instance obj = object.get_type().get_raw_type().is_wrapper() ? object.get_wrapped_instance() : object;

		rttr::type dd = obj.get_derived_type();

		const auto properties = dd.is_valid() ? dd.get_properties() : obj.get_type().get_properties();

		for (auto prop : properties)
		{
			type propType = prop.get_type();

			variant propValue = prop.get_value(obj);

			if (propType.is_sequential_container())
			{
				// 배열 내부에 대해서 모두 체크하면 된다.
				UUIDMappingSequentialRecursively(propValue.create_sequential_view());
			}
			else if (propType.is_associative_container())
			{
				UUIDMappingAssociativeRecursively(propValue.create_associative_view());
			}
			else
			{
				if (prop.get_metadata(DUOLCommon::MetaDataType::SerializeByUUID) == true 
					&& prop.get_metadata(DUOLCommon::MetaDataType::UUIDSerializeType) == DUOLCommon::UUIDSerializeType::FileUUID 
					&& propType.is_pointer())
				{
					// _propCount로 검색할 수 있을 것 같은데 ..
					{
						// rttr::instance object = _uuidInstanceMap.at(50);

						// prop.set_value(obj, object);
					}
				}

				UUIDMappingRecursively(propValue);
			}
		}
	}

	void JsonSerializer::UUIDMappingSequentialRecursively(const rttr::variant_sequential_view& view)
	{
		for (const auto& item : view)
		{
			if (item.is_sequential_container())
			{
				UUIDMappingSequentialRecursively(item.create_sequential_view());
			}
			else
			{
				rttr::variant wrappedVar = item.extract_wrapped_value();

				type valueType = wrappedVar.get_type();

				if (valueType.is_arithmetic() || valueType.is_enumeration() || valueType == type::get<std::string>() || valueType == type::get<std::wstring>())
				{
					// 넘어가고
				}
				else
				{
					UUIDMappingRecursively(wrappedVar);
				}
			}
		}
	}

	void JsonSerializer::UUIDMappingAssociativeRecursively(const rttr::variant_associative_view& view)
	{
		if (view.is_key_only_type())
		{
			for (auto& item : view)
			{
				UUIDMappingRecursively(item.first);
			}
		}
		else
		{
			for (auto& item : view)
			{
				UUIDMappingRecursively(item.first);

				UUIDMappingRecursively(item.second);
			}
		}
	}

	void JsonSerializer::WriteSequentialRecursively(variant_sequential_view& view, Value& jsonValue, bool isSerializedByUUID)
	{
		view.set_size(jsonValue.Size());

		for (SizeType i = 0 ; i < jsonValue.Size() ; i++)
		{
			auto& jsonIndexValue = jsonValue[i];

			if (jsonIndexValue.IsArray())
			{
				auto subArrayView = view.get_value(i).create_sequential_view();

				WriteSequentialRecursively(subArrayView, jsonIndexValue);
			}
			else if (jsonIndexValue.IsObject())
			{
				// 근데 이 녀석이 nullptr 이면 만들어줘야하는거잖아.
				variant tempVar = view.get_value(i);

				variant wrappedVar = tempVar.extract_wrapped_value();

				type t = wrappedVar.get_type();

				// 널 포인터인데 만약 UUID 시리얼라이즈가 아니면 .. 당연히 만들어주고 해야한다.
				if ((tempVar == nullptr) && (!isSerializedByUUID))
				{
					type tempVarType = tempVar.get_type();

					auto valueType = tempVar.get_type();

					auto wrappedType = valueType.is_wrapper() ? valueType.get_wrapped_type() : valueType;

					auto cons = wrappedType.get_raw_type().get_constructor();

					tempVar = cons.invoke();

					FromJsonRecursively(tempVar, jsonIndexValue);

					view.set_value(i, tempVar);
				}
			}
			else
			{
				const type arrayType = view.get_rank_type(i);

				variant extractedValue = ExtractBasicTypes(jsonIndexValue);

				if (extractedValue.convert(arrayType))
					view.set_value(i, extractedValue);
			}
		}
	}

	void JsonSerializer::WriteAssociativeViewRecursively(variant_associative_view& view, Value& jsonValue)
	{
		for (SizeType i = 0; i < jsonValue.Size(); ++i)
		{
			auto& jsonIndexValue = jsonValue[i];

			if (jsonIndexValue.IsObject()) // a key-value associative view
			{
				Value::MemberIterator keyIter = jsonIndexValue.FindMember("key");
				Value::MemberIterator valueIter = jsonIndexValue.FindMember("value");

				if (keyIter != jsonIndexValue.MemberEnd() &&
					valueIter != jsonIndexValue.MemberEnd())
				{
					auto keyVar = ExtractValue(keyIter, view.get_key_type());
					auto valueVar = ExtractValue(valueIter, view.get_value_type());
					
					if (keyVar && valueVar)
					{
						auto pa = view.insert(keyVar, valueVar);
					}
				}
			}
			else // a key-only associative view
			{
				variant extractedValue = ExtractBasicTypes(jsonIndexValue);

				if (extractedValue && extractedValue.convert(view.get_key_type()))
					view.insert(extractedValue);
			}
		}
	}

	variant JsonSerializer::ExtractValue(Value::MemberIterator& iter, const type& t)
	{
		auto& jsonValue = iter->value;
		variant extractedValue = ExtractBasicTypes(jsonValue);
		const bool couldConvert = extractedValue.convert(t);

		// Basic Type 아니라는 뜻 !
		if (!couldConvert)
		{
			if (jsonValue.IsObject())
			{
				rttr::constructor ctor = t.get_raw_type().get_constructor();

				for (auto& item : t.get_constructors())
				{
					if (item.get_instantiated_type() == t)
						ctor = item;
				}

				// TODO : 매개변수가 없는 생성자의 중요성
				extractedValue = ctor.invoke();

				FromJsonRecursively(extractedValue, jsonValue);
			}
		}

		return extractedValue;
	}

	variant JsonSerializer::ExtractBasicTypes(Value& jsonValue)
	{
		switch (jsonValue.GetType())
		{
			case kStringType:
			{
				return DUOLCommon::StringHelper::ToTString(std::string(jsonValue.GetString()));

				break;
			}

			case kNullType:    
			{
				break;
			}

			case kFalseType:
			case kTrueType:
			{
				return jsonValue.GetBool();

				break;
			}

			case kNumberType:
			{
				if (jsonValue.IsInt())
					return jsonValue.GetInt();
				else if (jsonValue.IsDouble())
					return jsonValue.GetDouble();
				else if (jsonValue.IsUint())
					return jsonValue.GetUint();
				else if (jsonValue.IsInt64())
					return jsonValue.GetInt64();
				else if (jsonValue.IsUint64())
					return jsonValue.GetUint64();
				break;
			}

			// we handle only the basic types here
			case kObjectType:
			case kArrayType: return variant();
		}

		return rttr::variant();
	}














	bool JsonSerializer::WriteVariant(const rttr::variant& var, PrettyWriter<StringBuffer>& writer)
	{
		auto valueType = var.get_type();
		auto wrappedType = valueType.is_wrapper() ? valueType.get_wrapped_type() : valueType;

		bool isWrapper = (wrappedType != valueType);

		// 원시 타입
		if (WritePrimitiveTypes(isWrapper ? wrappedType : valueType, isWrapper ? var.extract_wrapped_value() : var, writer))
		{
			// 오케이여
		}
		// 순차형 자료구조
		else if (var.is_sequential_container())
		{
			WriteSequentialContainer(var.create_sequential_view(), writer);
		}
		// 관계형 자료구조
		else if (var.is_associative_container())
		{
			WriteAssociativeContainer(var.create_associative_view(), writer);
		}
		else
		{
			auto childProps = isWrapper ? wrappedType.get_properties() : valueType.get_properties();

			if (!childProps.empty())
			{
				ToJsonRecursively(var, writer);
			}
			else
			{
				bool ok = false;
				auto text = var.to_string(&ok);

				if (!ok)
				{
					writer.String(text);

					return false;
				}

				writer.String(text);
			}
		}

		return true;
	}

	bool JsonSerializer::WritePrimitiveTypes(const rttr::type& t, const rttr::variant& var,
		PrettyWriter<StringBuffer>& writer)
	{
		// 산술 타입
		if (t.is_arithmetic())
		{
			if (t == type::get<bool>())
				writer.Bool(var.to_bool());
			else if (t == type::get<char>())
				writer.Bool(var.to_bool());
			else if (t == type::get<int8_t>())
				writer.Int(var.to_int8());
			else if (t == type::get<int16_t>())
				writer.Int(var.to_int16());
			else if (t == type::get<int32_t>())
				writer.Int(var.to_int32());
			else if (t == type::get<int64_t>())
				writer.Int64(var.to_int64());
			else if (t == type::get<uint8_t>())
				writer.Uint(var.to_uint8());
			else if (t == type::get<uint16_t>())
				writer.Uint(var.to_uint16());
			else if (t == type::get<uint32_t>())
				writer.Uint(var.to_uint32());
			else if (t == type::get<uint64_t>())
				writer.Uint64(var.to_uint64());
			else if (t == type::get<float>())
				writer.Double(var.to_double());
			else if (t == type::get<double>())
				writer.Double(var.to_double());

			return true;
		}
		// enum class
		else if (t.is_enumeration())
		{
			/*bool ok = false;

			auto result = var.to_string(&ok);

			if (ok)
			{
				writer.String(var.to_string());
			}
			else
			{
				ok = false;

				auto value = var.to_uint64(&ok);

				if (ok)
					writer.Uint64(value);
				else
					writer.Null();
			}*/

			// TODO : 일단 enumeration 들은 모두 숫자로 넣습니다. Text 로 넣고 싶다면 관련한 converting function이 필요합니다.
			bool ok = false;

			ok = false;

			auto value = var.to_uint64(&ok);

			if (ok)
				writer.Uint64(value);
			else
				writer.Null();

			return true;
		}
		// 'std::wstring'
		else if (t == type::get<std::wstring>())
		{
			std::string converted = var.convert<std::string>();

			writer.String(converted);

			return true;
		}
		// 'std::string'
		else if (t == type::get<std::string>())
		{
			writer.String(var.to_string());

			return true;
		}
		// 'UUID'
		else if (t == type::get<DUOLCommon::UUID>())
		{
			writer.Uint64(var.get_value<uint64_t>());

			return true;
		}

		return false;
	}

	void JsonSerializer::WriteSequentialContainer(const rttr::variant_sequential_view& view,
		PrettyWriter<StringBuffer>& writer)
	{
		writer.StartArray();

		for (const auto& item : view)
		{
			if (item.is_sequential_container())
			{
				WriteSequentialContainer(item.create_sequential_view(), writer);
			}
			else
			{
				rttr::variant wrappedVar = item.extract_wrapped_value();

				type valueType = wrappedVar.get_type();

				if (valueType.is_arithmetic() || valueType.is_enumeration() || valueType == type::get<std::string>() || valueType == type::get<std::wstring>())
				{
					WritePrimitiveTypes(valueType, wrappedVar, writer);
				}
				else
				{
					ToJsonRecursively(wrappedVar, writer);
				}
			}
		}

		writer.EndArray();
	}

	void JsonSerializer::WriteAssociativeContainer(const rttr::variant_associative_view& view,
		PrettyWriter<StringBuffer>& writer)
	{
		static const string_view key_name("key");
		static const string_view value_name("value");

		writer.StartArray();

		if (view.is_key_only_type())
		{
			for (auto& item : view)
			{
				WriteVariant(item.first, writer);
			}
		}
		else
		{
			for (auto& item : view)
			{
				writer.StartObject();

				writer.String(key_name.data(), static_cast<rapidjson::SizeType>(key_name.length()), false);

				WriteVariant(item.first, writer);

				writer.String(value_name.data(), static_cast<rapidjson::SizeType>(value_name.length()), false);

				WriteVariant(item.second, writer);

				writer.EndObject();
			}
		}

		writer.EndArray();
	}

	void JsonSerializer::ToJsonRecursively(const rttr::instance& target, PrettyWriter<StringBuffer>& writer)
	{
		writer.StartObject();

		instance object = target.get_type().get_raw_type().is_wrapper() ? target.get_wrapped_instance() : target;

		// 최종 아래 하단에 있는 오브젝트의 프로퍼티들의 리스트를 가져옵니다.
		auto properties = object.get_derived_type().get_properties();

		for (auto prop : properties)
		{
			if (!prop.get_metadata(DUOLCommon::MetaDataType::Serializable))
				continue;

			rttr::variant propValue = prop.get_value(object);

			// 값이 없으니 시리얼라이즈할 수 없습니다.
			if (!propValue)
				continue;
			
			const auto name = prop.get_name();

			writer.String(name.data(), static_cast<rapidjson::SizeType>(name.length()), false);

			// TODO : UUID 로 JSON에 남깁니다.
			// 해당 프로퍼티에 UUID로 시리얼라이즈 하라는 메타 데이터가 있다면 ...
			if (prop.get_metadata(DUOLCommon::MetaDataType::SerializeByUUID))
			{
				// Sequential UUID
				if (propValue.is_sequential_container())
					WriteUUIDSequentialContainer(propValue, writer);
				// Associative UUID
				else if (propValue.is_associative_container())
					WriteUUIDAssociativeContainer(propValue, writer);
				else
				{
					// TODO : DUOLGameEngine::ObjectBase 를 DUOLCommon 으로 옮겨서
					// 모든 프로젝트에서 참조할 수 있도록 만들어야 합니다.
					rttr::method getUUIDMethod = rttr::type::get_by_name("ObjectBase").get_method("GetUUID");

					const rttr::variant uuid = _getUUID.invoke(propValue);

					uuid.is_valid() ? writer.Uint64(uuid.get_value<uint64_t>()) : writer.Uint64(static_cast<uint64_t>(0));
				}
			}
			else if (!WriteVariant(propValue, writer))
			{
				// TODO : 로그 시스템 얹기. 적는데 실패하였습니다.
			}
		}

		writer.EndObject();
	}

	void JsonSerializer::WriteUUIDSequentialContainer(const rttr::variant& var, PrettyWriter<StringBuffer>& writer)
	{
		// TODO : DUOLGameEngine::ObjectBase 를 DUOLCommon 으로 옮겨서
		// 모든 프로젝트에서 참조할 수 있도록 만들어야 합니다.
		rttr::method getUUIDMethod = rttr::type::get_by_name("ObjectBase").get_method("GetUUID");

		auto view = var.create_sequential_view();

		writer.StartArray();

		for (const auto& item : view)
		{
			rttr::variant wrappedVar = item.extract_wrapped_value();

			const rttr::variant uuid = _getUUID.invoke(wrappedVar);

			uuid.is_valid() ? writer.Uint64(uuid.get_value<uint64_t>()) : writer.Uint64(static_cast<uint64_t>(0));
		}

		writer.EndArray();
	}

	void JsonSerializer::WriteUUIDAssociativeContainer(const rttr::variant& var, PrettyWriter<StringBuffer>& writer)
	{
		static const string_view key_name("key");
		static const string_view value_name("value");

		// TODO : DUOLGameEngine::ObjectBase 를 DUOLCommon 으로 옮겨서
		// 모든 프로젝트에서 참조할 수 있도록 만들어야 합니다.
		rttr::method getUUIDMethod = rttr::type::get_by_name("ObjectBase").get_method("GetUUID");

		auto view = var.create_associative_view();

		writer.StartArray();

		if (view.is_key_only_type())
		{
			for (auto& item : view)
			{
				const rttr::variant uuid = _getUUID.invoke(item.first);

				uuid.is_valid() ? writer.Uint64(uuid.get_value<uint64_t>()) : writer.Uint64(static_cast<uint64_t>(0));
			}
		}
		else
		{
			for (auto& item : view)
			{
				writer.StartObject();

				// Key 값이 UUID 인 경우에 대해서 처리합니다.
				writer.String(key_name.data(), static_cast<rapidjson::SizeType>(key_name.length()), false);

				const rttr::variant uuid = _getUUID.invoke(item.first);

				uuid.is_valid() ? writer.Uint64(uuid.get_value<uint64_t>()) : writer.Uint64(static_cast<uint64_t>(0));

				// Value는 Variant !
				writer.String(value_name.data(), static_cast<rapidjson::SizeType>(value_name.length()), false);

				WriteVariant(item.second, writer);

				writer.EndObject();
			}
		}

		writer.EndArray();
	}

	bool JsonSerializer::FromJson(const DUOLCommon::tstring& filePath, rttr::instance object)
	{
		Document doc {};

		std::ifstream ifs{ DUOLCommon::StringHelper::ToString(filePath) };

		if (!ifs.is_open())
		{
			// ERROR
		}

		IStreamWrapper isw{ ifs };

		doc.ParseStream(isw);

		if (doc.HasParseError())
			return false;

		FromJsonRecursively(object, doc);

		// object가 구현이 되었을텐데 .. object 순회하면서 매핑을 해볼까여 ?
		UUIDMappingRecursively(object);

		return true;
	}

	std::string JsonSerializer::ToJson(rttr::instance object)
	{
		if (!object.is_valid())
			return {};

		StringBuffer stringBuffer;
		PrettyWriter<StringBuffer> writer(stringBuffer);

		ToJsonRecursively(object, writer);

		return stringBuffer.GetString();
	}
}