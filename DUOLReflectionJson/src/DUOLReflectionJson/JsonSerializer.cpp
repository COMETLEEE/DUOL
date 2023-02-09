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
					// UUID �� �ø�������� ���� �ʴµ� ���� ������ Ÿ���̶�� ? => ��ü�� ������ݴϴ�
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

					// ����, �ش� ������Ƽ�� UUID ���ٸ� ..!  => Instance�� �־��ְ� ������ ������ �غ� ���ݴϴ�.
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
				// �迭 ���ο� ���ؼ� ��� üũ�ϸ� �ȴ�.
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
					// _propCount�� �˻��� �� ���� �� ������ ..
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
					// �Ѿ��
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
				// �ٵ� �� �༮�� nullptr �̸� ���������ϴ°��ݾ�.
				variant tempVar = view.get_value(i);

				variant wrappedVar = tempVar.extract_wrapped_value();

				type t = wrappedVar.get_type();

				// �� �������ε� ���� UUID �ø������� �ƴϸ� .. �翬�� ������ְ� �ؾ��Ѵ�.
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

		// Basic Type �ƴ϶�� �� !
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

				// TODO : �Ű������� ���� �������� �߿伺
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

		// ���� Ÿ��
		if (WritePrimitiveTypes(isWrapper ? wrappedType : valueType, isWrapper ? var.extract_wrapped_value() : var, writer))
		{
			// �����̿�
		}
		// ������ �ڷᱸ��
		else if (var.is_sequential_container())
		{
			WriteSequentialContainer(var.create_sequential_view(), writer);
		}
		// ������ �ڷᱸ��
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
		// ��� Ÿ��
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

			// TODO : �ϴ� enumeration ���� ��� ���ڷ� �ֽ��ϴ�. Text �� �ְ� �ʹٸ� ������ converting function�� �ʿ��մϴ�.
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

		// ���� �Ʒ� �ϴܿ� �ִ� ������Ʈ�� ������Ƽ���� ����Ʈ�� �����ɴϴ�.
		auto properties = object.get_derived_type().get_properties();

		for (auto prop : properties)
		{
			if (!prop.get_metadata(DUOLCommon::MetaDataType::Serializable))
				continue;

			rttr::variant propValue = prop.get_value(object);

			// ���� ������ �ø���������� �� �����ϴ�.
			if (!propValue)
				continue;
			
			const auto name = prop.get_name();

			writer.String(name.data(), static_cast<rapidjson::SizeType>(name.length()), false);

			// TODO : UUID �� JSON�� ����ϴ�.
			// �ش� ������Ƽ�� UUID�� �ø�������� �϶�� ��Ÿ �����Ͱ� �ִٸ� ...
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
					// TODO : DUOLGameEngine::ObjectBase �� DUOLCommon ���� �Űܼ�
					// ��� ������Ʈ���� ������ �� �ֵ��� ������ �մϴ�.
					rttr::method getUUIDMethod = rttr::type::get_by_name("ObjectBase").get_method("GetUUID");

					const rttr::variant uuid = _getUUID.invoke(propValue);

					uuid.is_valid() ? writer.Uint64(uuid.get_value<uint64_t>()) : writer.Uint64(static_cast<uint64_t>(0));
				}
			}
			else if (!WriteVariant(propValue, writer))
			{
				// TODO : �α� �ý��� ���. ���µ� �����Ͽ����ϴ�.
			}
		}

		writer.EndObject();
	}

	void JsonSerializer::WriteUUIDSequentialContainer(const rttr::variant& var, PrettyWriter<StringBuffer>& writer)
	{
		// TODO : DUOLGameEngine::ObjectBase �� DUOLCommon ���� �Űܼ�
		// ��� ������Ʈ���� ������ �� �ֵ��� ������ �մϴ�.
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

		// TODO : DUOLGameEngine::ObjectBase �� DUOLCommon ���� �Űܼ�
		// ��� ������Ʈ���� ������ �� �ֵ��� ������ �մϴ�.
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

				// Key ���� UUID �� ��쿡 ���ؼ� ó���մϴ�.
				writer.String(key_name.data(), static_cast<rapidjson::SizeType>(key_name.length()), false);

				const rttr::variant uuid = _getUUID.invoke(item.first);

				uuid.is_valid() ? writer.Uint64(uuid.get_value<uint64_t>()) : writer.Uint64(static_cast<uint64_t>(0));

				// Value�� Variant !
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

		// object�� ������ �Ǿ����ٵ� .. object ��ȸ�ϸ鼭 ������ �غ�� ?
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