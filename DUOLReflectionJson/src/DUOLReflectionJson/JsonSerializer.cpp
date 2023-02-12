#include "DUOLReflectionJson/JsonSerializer.h"

#include <fstream>

#include "DUOLCommon/MetaDataType.h"

#include "DUOLCommon/Util/UUID.h"

#include "rapidjson/istreamwrapper.h"

#include "DUOLGameEngine/ECS/ObjectBase.h"

namespace DUOLReflectionJson
{
	JsonSerializer::JsonSerializer() :
		_getUUID(rttr::type::get_by_name("ObjectBase").get_method("GetUUID"))
		, _getString(rttr::type::get_by_name("ObjectBase").get_method("GetName"))
		, _getAddress(rttr::type::get_by_name("ObjectBase").get_method("GetThis"))
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

		// 'int' => 'UUID'
		rttr::type::register_converter_func([](const int& id, bool& ok)
			{
				ok = true;

				return DUOLCommon::UUID{ static_cast<uint64_t>(id) };
			});

		// 'uint' => 'UUID'
		rttr::type::register_converter_func([](const uint32_t& id, bool& ok)
			{
				ok = true;

				return DUOLCommon::UUID{ id };
			});

		// 'int64_t' => 'UUID'
		rttr::type::register_converter_func([](const int64_t& id, bool& ok)
			{
				ok = true;

				return DUOLCommon::UUID{ static_cast<uint64_t>(id) };
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

						// 스트링으로 시리얼라이즈 했다는 것입니다.
						if (prop.get_metadata(DUOLCommon::MetaDataType::SerializeByString) == true)
							WriteSequentialTString(sequentialView, jsonValue);
						else
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
					if ((prop.get_metadata(DUOLCommon::MetaDataType::SerializeByUUID) != true) && valueType.is_pointer())
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

					type extracted_type = extracted_value.get_type();

					// UUID Serialize 타입이면 .. 어떻게든 포인터 변수가 가르키는 주소를 바꿔봅니다.
					if (prop.get_metadata(DUOLCommon::MetaDataType::SerializeByUUID))
					{
						uint64_t voPo = extracted_value.get_value<uint64_t>();

						variant con = voPo;

						bool ItsrealOK = con.convert(valueType);

						bool ItsOk = prop.set_value(obj, con);

						break;
					}

					// String Serialize 타입이면 .. _stringObjectFunc 으로 미리 준비되어 있는 녀석으로 매핑해준다.
					if (prop.get_metadata(DUOLCommon::MetaDataType::SerializeByString))
					{
						DUOLCommon::tstring tstr = extracted_value.get_value<DUOLCommon::tstring>();

						variant con = _stringObjectFunc(tstr);

						bool ItsrealOK = con.convert(valueType);

						bool ItsOk = prop.set_value(obj, con);

						break;
					}
					
					if (extracted_value.convert(valueType)) // REMARK: CONVERSION WORKS ONLY WITH "const type", check whether this is correct or not!
						prop.set_value(obj, extracted_value);

					// 만약, 해당 프로퍼티가 UUID 였다면 ..!  => 오브젝트의 주소를 넣어주고 다음에 쓸 수도 있으니까 .. 잘 보관해둡시다.
					if (prop.get_name() == "_uuid")
					{
						// UUID - Instance
						type t = obj.get_type();

						_uuidInstanceMap.insert({ _getUUID.invoke(obj).get_value<uint64_t>(), _getAddress.invoke(obj).get_value<void*>()});
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
			const type propType = prop.get_type();

			variant propValue = prop.get_value(obj);

			// 배열형 프로퍼티이다.
			if (propType.is_sequential_container())
			{
				// UUID 로 시리얼라이즈 되었고
				if (prop.get_metadata(DUOLCommon::MetaDataType::SerializeByUUID) == true
					// FileUUID, 즉, 시리얼라이즈 과정에서 얻은 UUID 개체를 참조한다면 ..
					&& prop.get_metadata(DUOLCommon::MetaDataType::MappingType) == DUOLCommon::MappingType::FileUUID)
				{
					auto view = propValue.create_sequential_view();

					type t = prop.get_type();

					// TODO: 일차원 배열만 지원합니다.
					for (int i = 0 ; i < view.get_size() ; i++)
					{
						rttr::variant wrappedVar = view.get_value(i).extract_wrapped_value();

						bool thatsok =	wrappedVar.convert<uint64_t>();

						const type valueType = wrappedVar.get_type();

						// UUID를 뽑아낸다.
						uint64_t uuid = wrappedVar.get_value<uint64_t>();

						// 만약, 뽑아냈다면
						if (_uuidInstanceMap.contains(uuid))
						{
							// 해당 uuid를 가진 객체의 포인터 주소이자 uint64_t
							variant mappedPointer = reinterpret_cast<uint64_t>(_uuidInstanceMap.at(uuid));

							// uint64_t 컨버팅합니다.
							bool isOk = mappedPointer.convert(valueType);

							type mappedType = mappedPointer.get_type();

							uint64_t yeah =	mappedPointer.get_value<uint64_t>();

							bool Isreok = view.set_value(i, mappedPointer);

							variant viewVal = view.get_value(i);

							uint64_t yeee = viewVal.get_value<uint64_t>();
						}
					}

					prop.set_value(obj, propValue);
				}
				// UUID 로 시리얼라이즈 되었고
				else if (prop.get_metadata(DUOLCommon::MetaDataType::SerializeByUUID) == true
						// Resource .. 검색하면 된다 !
						&& prop.get_metadata(DUOLCommon::MetaDataType::MappingType) == DUOLCommon::MappingType::Resource)
				{
					auto view = propValue.create_sequential_view();

					type t = prop.get_type();

					// TODO: 일차원 배열만 지원합니다.
					for (int i = 0; i < view.get_size(); i++)
					{
						rttr::variant wrappedVar = view.get_value(i).extract_wrapped_value();

						bool thatsok = wrappedVar.convert<uint64_t>();

						const type valueType = wrappedVar.get_type();

						// UUID를 뽑아낸다.
						uint64_t uuid = wrappedVar.get_value<uint64_t>();

						variant mappedPointer =	reinterpret_cast<uint64_t>(_uuidObjectFunc(uuid));

						// uint64_t 컨버팅합니다.
						bool isOk = mappedPointer.convert(valueType);

						type mappedType = mappedPointer.get_type();

						uint64_t yeah = mappedPointer.get_value<uint64_t>();

						bool Isreok = view.set_value(i, mappedPointer);

						variant viewVal = view.get_value(i);

						uint64_t yeee = viewVal.get_value<uint64_t>();
					}

					prop.set_value(obj, propValue);
				}
				else
					UUIDMappingSequentialRecursively(propValue.create_sequential_view());
			}
			// 관계형 프로퍼티이다.
			else if (propType.is_associative_container())
			{
				UUIDMappingAssociativeRecursively(propValue.create_associative_view());
			}
			else
			{
				// UUID 로 시리얼라이즈 되었고 (== UUID 로 원본 객체를 참조한다는 뜻)
				if (prop.get_metadata(DUOLCommon::MetaDataType::SerializeByUUID) == true
				// FileUUID, 즉, 시리얼라이즈 과정에서 얻은 UUID 개체를 참조한다는 뜻
					&& prop.get_metadata(DUOLCommon::MetaDataType::MappingType) == DUOLCommon::MappingType::FileUUID
				// 그리고 포인터 타입이면 ..!
					&& propType.is_pointer())
				{
					// 해당 프로퍼티가 가르킬 객체의 UUID
					variant uuidVar = prop.get_value(obj);

					// UUID를 뽑아낸다.
					uint64_t uuid = uuidVar.get_value<uint64_t>();

					// 만약, 뽑아냈다면
					if (_uuidInstanceMap.contains(uuid))
					{
						// 해당 uuid를 가진 객체의 포인터 주소이자 uint64_t
						variant mappedPointer =	reinterpret_cast<uint64_t>(_uuidInstanceMap.at(uuid));

						// uint64_t 컨버팅합니다.
						bool isOk = mappedPointer.convert(propType);

						// 넣어줍니다.
						bool ok = prop.set_value(obj, mappedPointer);
					}
				}
				// UUID 로 시리얼라이즈 되었고 (== UUID 로 원본 객체를 참조한다는 뜻)
				else if (prop.get_metadata(DUOLCommon::MetaDataType::SerializeByUUID) == true
					// Resource, 즉, 외부의 리소스 개체를 참조한다는 뜻
					&& prop.get_metadata(DUOLCommon::MetaDataType::MappingType) == DUOLCommon::MappingType::Resource
					// 그리고 포인터 타입이면 ..!
					&& propType.is_pointer())
				{
					// 해당 프로퍼티가 가르킬 객체의 UUID
					variant uuidVar = prop.get_value(obj);

					// UUID를 뽑아낸다.
					uint64_t uuid = uuidVar.get_value<uint64_t>();

					void* resourcePointer = _uuidObjectFunc(uuid);

					if (resourcePointer != nullptr)
					{
						// 해당 uuid를 가진 객체의 포인터 주소이자 uint64_t
						variant mappedPointer = reinterpret_cast<uint64_t>(resourcePointer);

						// uint64_t 컨버팅합니다.
						bool isOk = mappedPointer.convert(propType);

						// 넣어줍니다.
						bool ok = prop.set_value(obj, mappedPointer);
					}
					// 널 포인터 .. 없으니까 제거해주자 !
					else
					{
						// 해당 uuid를 가진 객체의 포인터 주소이자 uint64_t
						variant mappedPointer = reinterpret_cast<uint64_t>(nullptr);

						// uint64_t 컨버팅합니다.
						bool isOk = mappedPointer.convert(propType);

						// 넣어줍니다.
						bool ok = prop.set_value(obj, mappedPointer);
					}
				}
				// 리소스 등 포인터 값으로 UUID (널 포인터) 를 물고 있는 경우가 있다. 이런 경우에는 타지 않도록 주의하자. => 해결 완료.
				else
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
					// UUID 매핑이 필요 없으니 .. 넘어가고
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

				type varType = tempVar.get_type();

				variant wrappedVar = tempVar.extract_wrapped_value();

				type wrappedType = wrappedVar.get_type();

				// 널 포인터인데 만약 UUID 시리얼라이즈가 아니면 .. 당연히 객체 만들어주고 시작한다.
				if ((tempVar == nullptr) && (!isSerializedByUUID))
				{
					if (jsonIndexValue.FindMember("#Type") != jsonIndexValue.MemberEnd())
					{
						auto cons = type::get_by_name(jsonIndexValue.FindMember("#Type")->value.GetString()).get_constructor();

						tempVar = cons.invoke();

						type newType = tempVar.get_type();

						bool isValid = tempVar.is_valid();

						FromJsonRecursively(tempVar, jsonIndexValue);

						// 만약 래퍼 클래스라면 (shared_ptr, ... 포장을 벗겨줍니다.)
						if (tempVar.get_type().is_wrapper())
							tempVar.convert(newType.get_wrapped_type());

						// 해당 객체를 담을 Container 타입으로 변경합니다.
						if (view.get_value_type().is_wrapper())
							tempVar.convert(view.get_value_type().get_wrapped_type());

						type camType = tempVar.get_type();

						bool camVal = tempVar.is_valid();

						// 컨트롤 블록이 하나 더 만들어짐 그냥 raw pointer 만들고
						// raw pointer 어차피 상속되어 있을거니까 위로 올리고 wrapper로 만들어줄까 ?

						// Wrapper들의 sequence일 수 있으니 다시 감쌀 수 있으면 감싸준다.
						if (view.get_value_type().is_wrapper())
						{
							if (tempVar.can_convert(view.get_value_type()))
								tempVar.convert(view.get_value_type());
						}

						bool isOK = view.set_value(i, tempVar);

						int temp = 5;
					}
					else
					{
						auto valueType = tempVar.get_type();

						// 재귀적으로 최종 raw_ptr 까지 타고 올라간다. (shared 까지 )
						auto wrappedType = valueType.is_wrapper() ? valueType.get_wrapped_type() : valueType;

						wrappedType = wrappedType.is_wrapper() ? wrappedType.get_wrapped_type() : wrappedType;

						auto cons = wrappedType.get_raw_type().get_constructor();

						tempVar = cons.invoke();

						FromJsonRecursively(tempVar, jsonIndexValue);

						view.set_value(i, tempVar);
					}
				}
				// 포인터 변수 아니다. 스택 변수이다 !
				else
				{
					FromJsonRecursively(wrappedVar, jsonIndexValue);

					view.set_value(i, wrappedVar);
				}
			}
			else
			{
				// const type arrayType = view.get_rank_type(i);
				const type arrayType = view.get_value_type();

				variant extractedValue = ExtractBasicTypes(jsonIndexValue);

				type extType =	extractedValue.get_type();

				if (extractedValue.convert(arrayType))
					view.set_value(i, extractedValue);
			}
		}
	}

	void JsonSerializer::WriteSequentialTString(variant_sequential_view& view, Value& jsonValue)
	{
		view.set_size(jsonValue.Size());

		for (SizeType i = 0; i < jsonValue.Size(); i++)
		{
			auto& jsonIndexValue = jsonValue[i];

			variant extracted_value = ExtractBasicTypes(jsonIndexValue);

			DUOLCommon::tstring tstr = extracted_value.get_value<DUOLCommon::tstring>();

			variant target = _stringObjectFunc(tstr);

			// else
			{
				// const type arrayType = view.get_rank_type(i);
				const type arrayType = view.get_value_type();

				type extType = target.get_type();

				if (target.convert(arrayType))
					view.set_value(i, target);
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
					return static_cast<uint64_t>(jsonValue.GetInt());
				else if (jsonValue.IsUint())
					return static_cast<uint64_t>(jsonValue.GetUint());
				else if (jsonValue.IsInt64())
					return static_cast<uint64_t>(jsonValue.GetInt64());
				else if (jsonValue.IsUint64())
					return jsonValue.GetUint64();
				else if (jsonValue.IsDouble())
					return jsonValue.GetDouble();

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
		type targetOriginType = target.get_type();

		instance object = target.get_type().get_raw_type().is_wrapper() ? target.get_wrapped_instance() : target;

		// 해당 오브젝트의 프로퍼티들을 작성할 준비를 합니다.
		writer.StartObject();

		// TODO : 오브젝트 베이스로부터 상속된 녀석만 위에 이름 추가해보자 ..
		if (object.get_derived_type().is_valid() && object.get_derived_type().is_derived_from(type::get_by_name("ObjectBase")))
		{
			// 시리얼라이즈 되는 해당 오브젝트의 진짜 타입을 적습니다.
			writer.String("#Type");

			writer.String(object.get_derived_type().get_name().to_string());
		}

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
					const rttr::variant uuid = _getUUID.invoke(propValue);

					uuid.is_valid() ? writer.Uint64(uuid.get_value<uint64_t>()) : writer.Uint64(static_cast<uint64_t>(0));
				}
			}
			// TODO : String 으로 JSON 에 남깁니다.
			else if (prop.get_metadata(DUOLCommon::MetaDataType::SerializeByString))
			{
				// Sequential UUID
				if (propValue.is_sequential_container())
					WriteStringSequentialContainer(propValue, writer);
				// Associative UUID
				else if (propValue.is_associative_container())
					WriteStringAssociativeContainer(propValue, writer);
				else
				{
					const rttr::variant tstring = _getString.invoke(propValue);

					tstring.is_valid() ? writer.String(DUOLCommon::StringHelper::ToString(tstring.get_value<DUOLCommon::tstring>())) : writer.String("");
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

	void JsonSerializer::WriteStringSequentialContainer(const rttr::variant& var, PrettyWriter<StringBuffer>& writer)
	{
		auto view = var.create_sequential_view();

		writer.StartArray();

		for (const auto& item : view)
		{
			rttr::variant wrappedVar = item.extract_wrapped_value();

			const rttr::variant tstring = _getString.invoke(wrappedVar);

			tstring.is_valid() ? writer.String(DUOLCommon::StringHelper::ToString(tstring.get_value<DUOLCommon::tstring>())) : writer.String("");
		}

		writer.EndArray();
	}

	void JsonSerializer::WriteStringAssociativeContainer(const rttr::variant& var, PrettyWriter<StringBuffer>& writer)
	{
		static const string_view key_name("key");
		static const string_view value_name("value");

		auto view = var.create_associative_view();

		writer.StartArray();

		if (view.is_key_only_type())
		{
			for (auto& item : view)
			{
				const rttr::variant tstring = _getString.invoke(item.first);

				tstring.is_valid() ? writer.String(DUOLCommon::StringHelper::ToString(tstring.get_value<DUOLCommon::tstring>())) : writer.String("");
			}
		}
		else
		{
			for (auto& item : view)
			{
				writer.StartObject();

				// Key 값이 tstring 인 경우에 대해서 처리합니다.
				writer.String(key_name.data(), static_cast<rapidjson::SizeType>(key_name.length()), false);

				const rttr::variant tstring = _getString.invoke(item.first);

				tstring.is_valid() ? writer.String(DUOLCommon::StringHelper::ToString(tstring.get_value<DUOLCommon::tstring>())) : writer.String("");

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

		ifs.close();

		FromJsonRecursively(object, doc);

		// TODO : 일단 모든 것들의 UUID가 씌여지는 버그를 막기 위해서 ..
		// UUID Instance Map의 사이즈가 1 이상일 때 => 연결 지어야할 때
		if (_uuidInstanceMap.size() > 1)
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

	void JsonSerializer::SetUUIDObjectFunc(std::function<DUOLGameEngine::ObjectBase*(DUOLCommon::UUID)> uuidObjectFunc)
	{
		_uuidObjectFunc = uuidObjectFunc;
	}

	void JsonSerializer::SetStringObjectFunc(std::function<DUOLGameEngine::ObjectBase*(DUOLCommon::tstring&)> stringObjectFunc)
	{
		_stringObjectFunc = stringObjectFunc;
	}
}