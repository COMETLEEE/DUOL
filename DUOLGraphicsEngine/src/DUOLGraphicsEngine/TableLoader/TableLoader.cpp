#include "TableLoader.h"
#include <DUOLJson/JsonReader.h>

#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGraphicsEngine/RenderManager/RenderingPipeline/RenderingPipeline.h"
#include "DUOLGraphicsEngine/ResourceManager/ResourceManager.h"
#include "DUOLGraphicsEngine/Util/Hash/Hash.h"
#include "DUOLGraphicsLibrary/Renderer/Sampler.h"
#include "DUOLGraphicsLibrary/Renderer/Texture.h"

JSON_SERIALIZE_ENUM(DUOLGraphicsLibrary::TextureType,
	{
	{DUOLGraphicsLibrary::TextureType::TEXTURE1D, _T("Texture1D")},
	{DUOLGraphicsLibrary::TextureType::TEXTURE2D, _T("Texture2D")},
	{DUOLGraphicsLibrary::TextureType::TEXTURECUBE, _T("TextureCube")},
	{DUOLGraphicsLibrary::TextureType::TEXTURE2DMS, _T("Texture2DMS")}
	});

JSON_SERIALIZE_ENUM(DUOLGraphicsLibrary::ResourceFormat,
	{
		{DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32G32B32A32_FLOAT,	_T("FORMAT_R32G32B32A32_FLOAT")},
		{DUOLGraphicsLibrary::ResourceFormat::FORMAT_R8G8B8A8_UNORM,	_T("FORMAT_R8G8B8A8_UNORM")},
		{DUOLGraphicsLibrary::ResourceFormat::FORMAT_R24G8_TYPELESS,	_T("FORMAT_R24G8_TYPELESS")},
		{DUOLGraphicsLibrary::ResourceFormat::FORMAT_B8G8R8A8_UNORM,	_T("FORMAT_B8G8R8A8_UNORM")}
		,{DUOLGraphicsLibrary::ResourceFormat::FORMAT_R24_UNORM_X8_TYPELESS,	_T("FORMAT_R24_UNORM_X8_TYPELESS")}
		,{DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32_FLOAT,	_T("FORMAT_R32_FLOAT")}
		,{DUOLGraphicsLibrary::ResourceFormat::FORMAT_R16G16B16A16_FLOAT,	_T("FORMAT_R16G16B16A16_FLOAT")}
	});

JSON_SERIALIZE_ENUM(DUOLGraphicsLibrary::ResourceUsage,
	{
		{DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT,		_T("USAGE_DEFAULT")},
		{DUOLGraphicsLibrary::ResourceUsage::USAGE_DYNAMIC,		_T("USAGE_DYNAMIC")},
		{DUOLGraphicsLibrary::ResourceUsage::USAGE_IMMUTABLE,	_T("USAGE_IMMUTABLE")},
		{DUOLGraphicsLibrary::ResourceUsage::USAGE_STAGING,		_T("USAGE_STAGING")}
	});

JSON_SERIALIZE_ENUM(DUOLGraphicsLibrary::RenderTargetType,
	{
		{DUOLGraphicsLibrary::RenderTargetType::Color,			_T("Color")},
		{DUOLGraphicsLibrary::RenderTargetType::DepthStencil,	_T("Depth")},
	});

bool DUOLGraphicsEngine::TableLoader::LoadRenderTargetTable(ResourceManager* resourceManager, const DUOLMath::Vector2& screenSize)
{
	auto jsonLoader = DUOLJson::JsonReader::GetInstance();

	const DUOLCommon::tstring textureTablePath(_T("Asset/DataTable/TextureTable.json"));
	const DUOLCommon::tstring renderTargetTablePath(_T("Asset/DataTable/RenderTargetTable.json"));

	//미리 렌더타겟 Table을 로드합니다.
	//텍스쳐 생성시 렌더타겟도 같이 만들어줌.
	auto renderTargets = jsonLoader->LoadJson(renderTargetTablePath);
	auto textures = jsonLoader->LoadJson(textureTablePath);

	const TCHAR* id = _T("ID");
	const TCHAR* textureid = _T("TextureID");
	const TCHAR* textureType = _T("TextureType");
	const TCHAR* mipLevel = _T("MipLevel");
	const TCHAR* arrayLayer = _T("ArrayLayer");
	const TCHAR* arraySize = _T("ArraySize");


	const TCHAR* extent = _T("Extent");
	const TCHAR* resolutionProportion = _T("ResolutionProportion");
	const TCHAR* percent = _T("Percent");
	const TCHAR* sample = _T("Sample");
	const TCHAR* resourceFormat = _T("ResourceFormat");
	const TCHAR* resourceUsage = _T("ResourceUsage");
	const TCHAR* cpuAccessFlag = _T("CPUAccessFlag");
	const TCHAR* bindFlag = _T("BindFlag");
	const TCHAR* renderTargetType = _T("RenderTargetType");

	for (auto& texture : textures->GetArray())
	{
		DUOLGraphicsLibrary::TextureDesc textureDesc;

		if (texture.HasMember(textureType))
		{
			if (!StringToEnum(texture[textureType].GetString(), textureDesc._type))
			{
				assert("Error");
			}
		}
		if (texture.HasMember(sample))
		{
			textureDesc._sample = texture[sample].GetInt();
		}
		if (texture.HasMember(resourceFormat))
		{
			if (!StringToEnum(texture[resourceFormat].GetString(), textureDesc._format))
			{
				assert("Error");
			}
		}
		if (texture.HasMember(resourceUsage))
		{
			if (!StringToEnum(texture[resourceUsage].GetString(), textureDesc._usage))
			{
				assert("Error");
			}
		}
		if (texture.HasMember(bindFlag))
		{
			for (rapidjson::SizeType i = 0; i < texture[bindFlag].Size(); i++)
			{
				int offset = texture[bindFlag][i].GetInt() - 1;
				if (offset >= 0)
					textureDesc._bindFlags |= (1 << offset);
			}
		}
		if (texture.HasMember(cpuAccessFlag))
		{
			for (rapidjson::SizeType i = 0; i < texture[cpuAccessFlag].Size(); i++)
			{
				int offset = texture[cpuAccessFlag][i].GetInt() - 1;
				if (offset >= 0)
					textureDesc._cpuAccessFlags |= (1 << offset);
			}
		}
		if (texture.HasMember(mipLevel))
		{
			textureDesc._mipLevels = texture[mipLevel].GetInt();
			if (textureDesc._mipLevels == 0)
				textureDesc._miscFlags |= static_cast<long>(DUOLGraphicsLibrary::MiscFlags::RESOURCE_MISC_GENERATE_MIPS);
		}
		if (texture.HasMember(arraySize))
		{
			textureDesc._arraySize = texture[arraySize].GetInt();
		}
		if (texture.HasMember(extent))
		{
			if (texture[extent].Size() > 3)
			{
				assert("Error");
			}
			for (rapidjson::SizeType i = 0; i < texture[extent].Size(); i++)
			{
				if (i == 0)
					textureDesc._textureExtent.x = texture[extent][i].GetInt();
				if (i == 1)
					textureDesc._textureExtent.y = texture[extent][i].GetInt();
				if (i == 2)
					textureDesc._textureExtent.z = texture[extent][i].GetInt();
			}
		}

		bool isproportionBool = false;
		float percentValue = 0.f;
		DUOLCommon::tstring idResource;

		if (texture.HasMember(percent))
		{
			percentValue = texture[percent].GetFloat();
		}

		if (texture.HasMember(resolutionProportion))
		{
			isproportionBool = texture[resolutionProportion].GetBool();
			if (isproportionBool)
			{
				textureDesc._textureExtent.x = screenSize.x * percentValue;
				textureDesc._textureExtent.y = screenSize.y * percentValue;
			}
		}

		if (texture.HasMember(id))
		{
			idResource = texture[id].GetString();
		}

		auto rettexture = resourceManager->CreateTexture(idResource, textureDesc, isproportionBool, percentValue);
	}

	for (auto& renderTarget : renderTargets->GetArray())
	{
		DUOLGraphicsLibrary::RenderTargetDesc renderTargetDesc;

		DUOLCommon::tstring idResource;
		DUOLCommon::tstring textureidResource;

		if (renderTarget.HasMember(id))
		{
			idResource = renderTarget[id].GetString();
		}

		if (renderTarget.HasMember(textureid))
		{
			textureidResource = renderTarget[textureid].GetString();
			renderTargetDesc._texture = resourceManager->GetTexture(textureidResource);
		}

		if (renderTarget.HasMember(mipLevel))
		{
			renderTargetDesc._mipLevel = renderTarget[mipLevel].GetInt();
		}

		if (renderTarget.HasMember(arrayLayer))
		{
			renderTargetDesc._arrayLayer = renderTarget[arrayLayer].GetInt();
		}

		if (renderTarget.HasMember(arraySize))
		{
			renderTargetDesc._arraySize = renderTarget[arraySize].GetInt();
		}

		if (renderTarget.HasMember(renderTargetType))
		{
			if (!StringToEnum(renderTarget[renderTargetType].GetString(), renderTargetDesc._type))
			{
				assert("Error");
			}
		}

		resourceManager->CreateRenderTarget(idResource, renderTargetDesc);
	}

	jsonLoader->UnloadJson(renderTargetTablePath);
	jsonLoader->UnloadJson(textureTablePath);

	return true;
}

JSON_SERIALIZE_ENUM(DUOLGraphicsLibrary::ShaderType,
	{
		{DUOLGraphicsLibrary::ShaderType::VERTEX,			_T("VERTEX")},
		{DUOLGraphicsLibrary::ShaderType::HULL,				_T("HULL")},
		{DUOLGraphicsLibrary::ShaderType::DOMAINS,			_T("DOMAIN")},
		{DUOLGraphicsLibrary::ShaderType::GEOMETRY,			_T("GEOMETRY")},
		{DUOLGraphicsLibrary::ShaderType::PIXEL,			_T("PIXEL")},
		{DUOLGraphicsLibrary::ShaderType::COMPUTE,			_T("COMPUTE")},
	});

bool DUOLGraphicsEngine::TableLoader::LoadShaderTable(ResourceManager* resourceManager)
{
	auto jsonLoader = DUOLJson::JsonReader::GetInstance();
	g_threadPool.init();

	const DUOLCommon::tstring shaderTablePath(_T("Asset/DataTable/ShaderTable.json"));

	auto shaderTables = jsonLoader->LoadJson(shaderTablePath);


	struct ShaderInfo
	{
		std::string strsource;
		std::string strprofile;
		std::string strentrypoint;
		std::vector<std::string> macros;
	};

	std::vector<ShaderInfo> shaderStrinfos;

	for (auto& shaderTable : shaderTables->GetArray())
	{
		DUOLGraphicsLibrary::ShaderDesc shaderDesc;

		const TCHAR* id = _T("ID");
		const TCHAR* shaderType = _T("ShaderType");
		const TCHAR* source = _T("Source");
		const TCHAR* profile = _T("Profile");
		const TCHAR* entryPoint = _T("EntryPoint");
		const TCHAR* shaderMacro = _T("ShaderMacro");
		const TCHAR* flag = _T("Flag");

		if (shaderTable.HasMember(shaderType))
		{
			if (!StringToEnum(shaderTable[shaderType].GetString(), shaderDesc._type))
			{
				assert("Error");
			}

			if (shaderDesc._type == DUOLGraphicsLibrary::ShaderType::GEOMETRY)
			{
				if (shaderTable.HasMember(flag))
				{
					int rflag = shaderTable[flag].GetInt();

					if (rflag == 1)
					{
						shaderDesc._useStreamOut = true;
					}
				}
			}
		}

		//std::string strsource;
		//std::string strprofile;
		//std::string strentrypoint;
		//std::vector<std::string> macros;

		//if (shaderTable.HasMember(source))
		//{
		//	strsource = DUOLCommon::StringHelper::WStringToString(shaderTable[source].GetString());

		//	shaderDesc._source = strsource.c_str();
		//}

		//if (shaderTable.HasMember(profile))
		//{
		//	strprofile = DUOLCommon::StringHelper::WStringToString(shaderTable[profile].GetString());

		//	shaderDesc._profile = strprofile.c_str();
		//}

		//if (shaderTable.HasMember(entryPoint))
		//{
		//	strentrypoint = DUOLCommon::StringHelper::WStringToString(shaderTable[entryPoint].GetString());

		//	shaderDesc._entryPoint = strentrypoint.c_str();
		//}

		//if (shaderTable.HasMember(shaderMacro))
		//{
		//	shaderDesc._shaderMacro.reserve(shaderTable[shaderMacro].Size());
		//	macros.reserve(shaderTable[shaderMacro].Size());

		//	for (auto& macro : shaderTable[shaderMacro].GetArray())
		//	{
		//		DUOLGraphicsLibrary::ShaderMacroDesc shaderMacro;

		//		macros.emplace_back(DUOLCommon::StringHelper::WStringToString(macro.GetString()));
		//		shaderMacro.Name = macros.back().c_str();

		//		shaderDesc._shaderMacro.emplace_back(shaderMacro);
		//	}
		//}
		shaderStrinfos.emplace_back();
		ShaderInfo& info = shaderStrinfos.back();

		if (shaderTable.HasMember(source))
		{
			info.strsource = DUOLCommon::StringHelper::WStringToString(shaderTable[source].GetString());

			memcpy(shaderDesc._source, info.strsource.data(), info.strsource.length());
		}

		if (shaderTable.HasMember(profile))
		{
			info.strprofile = DUOLCommon::StringHelper::WStringToString(shaderTable[profile].GetString());

			memcpy(shaderDesc._profile, info.strprofile.data(), info.strprofile.length());
		}

		if (shaderTable.HasMember(entryPoint))
		{
			info.strentrypoint = DUOLCommon::StringHelper::WStringToString(shaderTable[entryPoint].GetString());

			memcpy(shaderDesc._entryPoint, info.strentrypoint.data(), info.strentrypoint.length());
		}

		if (shaderTable.HasMember(shaderMacro))
		{
			shaderDesc._shaderMacro.reserve(shaderTable[shaderMacro].Size());
			info.macros.reserve(shaderTable[shaderMacro].Size());

			for (auto& macro : shaderTable[shaderMacro].GetArray())
			{
				DUOLGraphicsLibrary::ShaderMacroDesc shaderMacro;

				info.macros.emplace_back(DUOLCommon::StringHelper::WStringToString(macro.GetString()));
				shaderMacro.Name = info.macros.back().c_str();

				shaderDesc._shaderMacro.emplace_back(shaderMacro);
			}
		}


		if (shaderTable.HasMember(id))
		{
			//non-thread
			//resourceManager->CreateShader(Hash::Hash64(shaderTable[id].GetString()), shaderDesc);

			auto task = [](ResourceManager* resourceManager, const UINT64 id, const DUOLGraphicsLibrary::ShaderDesc shaderDesc)
			{
				//std::string message = shaderDesc._source;
				//DUOL_ENGINE_INFO(DUOL_CONSOLE, message)
				resourceManager->CreateShader(id, shaderDesc);
				//message += " End";
				//DUOL_ENGINE_INFO(DUOL_CONSOLE, message)
			};

			g_threadPool.submit(task, resourceManager, Hash::Hash64(shaderTable[id].GetString()), shaderDesc);
		}
	}


	while (g_threadPool.GetJobCount())
	{

	}
	g_threadPool.shutdown();
	jsonLoader->UnloadJson(shaderTablePath);

	return false;
}

JSON_SERIALIZE_ENUM(DUOLGraphicsLibrary::ComparisonFunc,
	{
	{DUOLGraphicsLibrary::ComparisonFunc::COMPARISON_NEVER, _T("NEVER")},
	{DUOLGraphicsLibrary::ComparisonFunc::COMPARISON_LESS, _T("LESS")},
	{DUOLGraphicsLibrary::ComparisonFunc::COMPARISON_EQUAL, _T("EQUAL")},
	{DUOLGraphicsLibrary::ComparisonFunc::COMPARISON_LESS_EQUAL, _T("LESS_EQUAL")},
	{DUOLGraphicsLibrary::ComparisonFunc::COMPARISON_GREATER, _T("GREATER")},
	{DUOLGraphicsLibrary::ComparisonFunc::COMPARISON_NOT_EQUAL, _T("NOT_EQUAL")},
	{DUOLGraphicsLibrary::ComparisonFunc::COMPARISON_GREATER_EQUAL, _T("GREATER_EQUAL")},
	{DUOLGraphicsLibrary::ComparisonFunc::COMPARISON_ALWAYS, _T("ALWAYS")},
	});

JSON_SERIALIZE_ENUM(DUOLGraphicsLibrary::SamplerFilter,
	{
	 {DUOLGraphicsLibrary::SamplerFilter::FILTER_MIN_MAG_MIP_POINT							,_T("FILTER_MIN_MAG_MIP_POINT")}
	,{DUOLGraphicsLibrary::SamplerFilter::FILTER_MIN_MAG_POINT_MIP_LINEAR					,_T("FILTER_MIN_MAG_POINT_MIP_LINEAR")}
	,{DUOLGraphicsLibrary::SamplerFilter::FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT				,_T("FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT")}
	,{DUOLGraphicsLibrary::SamplerFilter::FILTER_MIN_POINT_MAG_MIP_LINEAR					,_T("FILTER_MIN_POINT_MAG_MIP_LINEAR")}
	,{DUOLGraphicsLibrary::SamplerFilter::FILTER_MIN_LINEAR_MAG_MIP_POINT					,_T("FILTER_MIN_LINEAR_MAG_MIP_POINT")}
	,{DUOLGraphicsLibrary::SamplerFilter::FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR			,_T("FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR")}
	,{DUOLGraphicsLibrary::SamplerFilter::FILTER_MIN_MAG_LINEAR_MIP_POINT					,_T("FILTER_MIN_MAG_LINEAR_MIP_POINT")}
	,{DUOLGraphicsLibrary::SamplerFilter::FILTER_ANISOTROPIC								,_T("FILTER_ANISOTROPIC")}
	,{DUOLGraphicsLibrary::SamplerFilter::FILTER_COMPARISON_MIN_MAG_MIP_POINT			 	,_T("FILTER_COMPARISON_MIN_MAG_MIP_POINT")}
	,{DUOLGraphicsLibrary::SamplerFilter::FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR		,_T("FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR")}
	,{DUOLGraphicsLibrary::SamplerFilter::FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT	,_T("FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT")}
	,{DUOLGraphicsLibrary::SamplerFilter::FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR		,_T("FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR")}
	,{DUOLGraphicsLibrary::SamplerFilter::FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT		,_T("FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT")}
	,{DUOLGraphicsLibrary::SamplerFilter::FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR	,_T("FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR")}
	,{DUOLGraphicsLibrary::SamplerFilter::FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT		,_T("FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT")}
	,{DUOLGraphicsLibrary::SamplerFilter::FILTER_COMPARISON_MIN_MAG_MIP_LINEAR				,_T("FILTER_COMPARISON_MIN_MAG_MIP_LINEAR")}
	,{DUOLGraphicsLibrary::SamplerFilter::FILTER_COMPARISON_ANISOTROPIC						,_T("FILTER_COMPARISON_ANISOTROPIC")}
	});

JSON_SERIALIZE_ENUM(DUOLGraphicsLibrary::SamplerAddressMode,
	{
		 {DUOLGraphicsLibrary::SamplerAddressMode::ADDRESS_WRAP			,_T("ADDRESS_WRAP")}
		,{DUOLGraphicsLibrary::SamplerAddressMode::ADDRESS_MIRROR		,_T("ADDRESS_MIRROR")}
		,{DUOLGraphicsLibrary::SamplerAddressMode::ADDRESS_BORDER		,_T("ADDRESS_BORDER")}
		,{DUOLGraphicsLibrary::SamplerAddressMode::ADDRESS_CLAMP		,_T("ADDRESS_CLAMP")}
		,{DUOLGraphicsLibrary::SamplerAddressMode::ADDRESS_MIRROR_ONCE	,_T("ADDRESS_MIRROR_ONCE")}
	});

bool DUOLGraphicsEngine::TableLoader::LoadSampler(ResourceManager* resourceManager)
{
	auto jsonLoader = DUOLJson::JsonReader::GetInstance();

	const TCHAR* id = _T("ID");
	const DUOLCommon::tstring SamplerDescPath(_T("Asset/DataTable/SamplerTable.json"));

	{
		const TCHAR* SamplerFilter = _T("SamplerFilter");
		const TCHAR* AddressModeU = _T("AddressModeU");
		const TCHAR* AddressModeV = _T("AddressModeV");
		const TCHAR* AddressModeW = _T("AddressModeW");
		const TCHAR* ComparisonFunc = _T("ComparisonFunc");
		const TCHAR* MipLODBias = _T("MipLODBias");
		const TCHAR* MaxAnisotrpy = _T("MaxAnisotrpy");
		const TCHAR* MinLOD = _T("MinLOD");
		const TCHAR* MaxLOD = _T("MaxLOD");
		const TCHAR* BorderColor = _T("BorderColor");

		auto samplerInfos = jsonLoader->LoadJson(SamplerDescPath);

		for (auto& samplerInfo : samplerInfos->GetArray())
		{
			DUOLGraphicsLibrary::SamplerDesc samplerDesc;

			if (samplerInfo.HasMember(SamplerFilter))
			{
				auto ret = samplerInfo[SamplerFilter].GetString();
				StringToEnum(ret, samplerDesc._samplerFilter);
			}

			if (samplerInfo.HasMember(AddressModeU))
			{
				auto ret = samplerInfo[AddressModeU].GetString();
				StringToEnum(ret, samplerDesc._addressU);
			}

			if (samplerInfo.HasMember(AddressModeV))
			{
				auto ret = samplerInfo[AddressModeV].GetString();
				StringToEnum(ret, samplerDesc._addressV);
			}

			if (samplerInfo.HasMember(AddressModeW))
			{
				auto ret = samplerInfo[AddressModeW].GetString();
				StringToEnum(ret, samplerDesc._addressW);
			}

			if (samplerInfo.HasMember(ComparisonFunc))
			{
				auto ret = samplerInfo[ComparisonFunc].GetString();
				StringToEnum(ret, samplerDesc._comparisonFunc);
			}

			if (samplerInfo.HasMember(MipLODBias))
			{
				samplerDesc._mipLODBias = samplerInfo[MipLODBias].GetFloat();
			}

			if (samplerInfo.HasMember(MinLOD))
			{
				samplerDesc._minLOD = samplerInfo[MinLOD].GetFloat();
			}

			if (samplerInfo.HasMember(MaxLOD))
			{
				samplerDesc._maxLOD = samplerInfo[MaxLOD].GetFloat();
			}

			if (samplerInfo.HasMember(MaxAnisotrpy))
			{
				samplerDesc._maxAnisotropy = samplerInfo[MaxAnisotrpy].GetInt();
			}

			if (samplerInfo.HasMember(BorderColor))
			{
				constexpr int maxidx = 4;
				int idx = 0;

				for (auto& borderColor : samplerInfo[BorderColor].GetArray())
				{
					samplerDesc._borderColor[idx] = borderColor.GetFloat();

					idx++;
					if (idx >= maxidx)
					{
						break;
					}
				}
			}

			if (samplerInfo.HasMember(id))
			{
				DUOLCommon::tstring str = samplerInfo[id].GetString();
				resourceManager->CreateSampler(Hash::Hash64(str), samplerDesc);
			}
		}

		jsonLoader->UnloadJson(SamplerDescPath);
	}
	return false;
}

JSON_SERIALIZE_ENUM(DUOLGraphicsLibrary::PrimitiveTopology,
	{
	{DUOLGraphicsLibrary::PrimitiveTopology::PRIMITIVE_TOPOLOGY_POINTLIST, _T("POINTLIST")},
	{DUOLGraphicsLibrary::PrimitiveTopology::PRIMITIVE_TOPOLOGY_LINELIST, _T("LINELIST")},
	{DUOLGraphicsLibrary::PrimitiveTopology::PRIMITIVE_TOPOLOGY_LINESTRIP, _T("LINESTRIP")},
	{DUOLGraphicsLibrary::PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST, _T("TRIANGLELIST")},
	{DUOLGraphicsLibrary::PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, _T("TRIANGLESTRIP")},
	});

//TODO... 전부다 안함^^.... 나중에하자
JSON_SERIALIZE_ENUM(DUOLGraphicsLibrary::BlendStateDesc::RenderTagetBlendFactor::Blend,
	{
		{DUOLGraphicsLibrary::BlendStateDesc::RenderTagetBlendFactor::Blend::BLEND_ONE,			_T("BLEND_ONE")},
		{DUOLGraphicsLibrary::BlendStateDesc::RenderTagetBlendFactor::Blend::BLEND_ZERO,		_T("BLEND_ZERO")},
		{DUOLGraphicsLibrary::BlendStateDesc::RenderTagetBlendFactor::Blend::BLEND_SRC_COLOR,	_T("BLEND_SRC_COLOR")},
		{DUOLGraphicsLibrary::BlendStateDesc::RenderTagetBlendFactor::Blend::BLEND_DEST_COLOR,	_T("BLEND_DEST_COLOR")},
		{DUOLGraphicsLibrary::BlendStateDesc::RenderTagetBlendFactor::Blend::BLEND_SRC_ALPHA,	_T("BLEND_SRC_ALPHA")},
		{DUOLGraphicsLibrary::BlendStateDesc::RenderTagetBlendFactor::Blend::BLEND_DEST_ALPHA,	_T("BLEND_DEST_ALPHA")},
		{DUOLGraphicsLibrary::BlendStateDesc::RenderTagetBlendFactor::Blend::BLEND_INV_SRC_ALPHA,	_T("BLEND_INV_SRC_ALPHA")},
	});

JSON_SERIALIZE_ENUM(DUOLGraphicsLibrary::BlendStateDesc::RenderTagetBlendFactor::BlendOp,
	{
		{DUOLGraphicsLibrary::BlendStateDesc::RenderTagetBlendFactor::BlendOp::BLEND_OP_ADD,			_T("BLEND_OP_ADD")},
		{DUOLGraphicsLibrary::BlendStateDesc::RenderTagetBlendFactor::BlendOp::BLEND_OP_SUBTRACT,		_T("BLEND_OP_SUBTRACT")},
		{DUOLGraphicsLibrary::BlendStateDesc::RenderTagetBlendFactor::BlendOp::BLEND_OP_MAX,			_T("BLEND_OP_MAX")},
		{DUOLGraphicsLibrary::BlendStateDesc::RenderTagetBlendFactor::BlendOp::BLEND_OP_MIN,			_T("BLEND_OP_MIN")},
		{DUOLGraphicsLibrary::BlendStateDesc::RenderTagetBlendFactor::BlendOp::BLEND_OP_REV_SUBTRACT,	_T("BLEND_OP_REV_SUBTRACT")},
	});

JSON_SERIALIZE_ENUM(DUOLGraphicsLibrary::StencilOp,
	{
		{DUOLGraphicsLibrary::StencilOp::STENCIL_OP_KEEP,		_T("KEEP")},
		{DUOLGraphicsLibrary::StencilOp::STENCIL_OP_ZERO,		_T("ZERO")},
		{DUOLGraphicsLibrary::StencilOp::STENCIL_OP_REPLACE,	_T("REPLACE")},
		{DUOLGraphicsLibrary::StencilOp::STENCIL_OP_INCR_SAT,	_T("INCR_SAT")},
		{DUOLGraphicsLibrary::StencilOp::STENCIL_OP_DECR_SAT,	_T("DECR_SAT")},
		{DUOLGraphicsLibrary::StencilOp::STENCIL_OP_INVERT,		_T("INVERT")},
		{DUOLGraphicsLibrary::StencilOp::STENCIL_OP_INCR,		_T("INCR")},
		{DUOLGraphicsLibrary::StencilOp::STENCIL_OP_DECR,		_T("DECR")},
	});

JSON_SERIALIZE_ENUM(DUOLGraphicsLibrary::RasterizerStateDesc::FillMode,
	{
		{DUOLGraphicsLibrary::RasterizerStateDesc::FillMode::SOLID,		_T("SOLID")},
		{DUOLGraphicsLibrary::RasterizerStateDesc::FillMode::WIRE,		_T("WIRE")},
	});

JSON_SERIALIZE_ENUM(DUOLGraphicsLibrary::RasterizerStateDesc::CullMode,
	{
		{DUOLGraphicsLibrary::RasterizerStateDesc::CullMode::CULL_NONE,		_T("CULL_NONE")},
		{DUOLGraphicsLibrary::RasterizerStateDesc::CullMode::CULL_FRONT,	_T("CULL_FRONT")},
		{DUOLGraphicsLibrary::RasterizerStateDesc::CullMode::CULL_BACK,		_T("CULL_BACK")},
	});

bool DUOLGraphicsEngine::TableLoader::LoadPipelineStateTable(ResourceManager* resourceManager)
{
	auto jsonLoader = DUOLJson::JsonReader::GetInstance();

	std::unordered_map<DUOLCommon::tstring, DUOLGraphicsLibrary::BlendStateDesc> blendStates;
	std::unordered_map<DUOLCommon::tstring, DUOLGraphicsLibrary::DepthStencilStateDesc> depthStencilStates;
	std::unordered_map<DUOLCommon::tstring, DUOLGraphicsLibrary::RasterizerStateDesc> rasterizerStates;

	const TCHAR* id = _T("ID");
	const DUOLCommon::tstring blendStatePath(_T("Asset/DataTable/BlendState.json"));

	//blendState
	{
		const TCHAR* AlphaToCoverageEnable = _T("AlphaToCoverageEnable");
		const TCHAR* IndependentBlendEnable = _T("IndependentBlendEnable");
		const TCHAR* RenderTagetBlendFactor = _T("RenderTagetBlendFactor");
		const TCHAR* srcBlend = _T("srcBlend");
		const TCHAR* destBlend = _T("destBlend");
		const TCHAR* srcBlendAlpha = _T("srcBlendAlpha");
		const TCHAR* destBlendAlpha = _T("destBlendAlpha");
		const TCHAR* blendOP = _T("blendOP");
		const TCHAR* blendOpAlpha = _T("blendOpAlpha");

		auto blendStateInfos = jsonLoader->LoadJson(blendStatePath);

		for (auto& blendStateInfo : blendStateInfos->GetArray())
		{
			DUOLGraphicsLibrary::BlendStateDesc blendStateDesc;

			if (blendStateInfo.HasMember(AlphaToCoverageEnable))
			{
				auto ret = blendStateInfo[AlphaToCoverageEnable].GetBool();
				blendStateDesc._alphaToCoverageEnable = ret;
			}

			if (blendStateInfo.HasMember(IndependentBlendEnable))
			{
				auto ret = blendStateInfo[IndependentBlendEnable].GetBool();
				blendStateDesc._independentBlendEnable = ret;
			}

			if (blendStateInfo.HasMember(RenderTagetBlendFactor))
			{
				auto ret = blendStateInfo[RenderTagetBlendFactor].GetBool();
				blendStateDesc._renderTarget[0]._blendEnable = ret;
			}

			if (blendStateInfo.HasMember(srcBlend))
			{
				auto ret = blendStateInfo[srcBlend].GetString();
				StringToEnum(ret, blendStateDesc._renderTarget[0]._srcBlend);
			}

			if (blendStateInfo.HasMember(srcBlend))
			{
				auto ret = blendStateInfo[srcBlend].GetString();
				StringToEnum(ret, blendStateDesc._renderTarget[0]._srcBlend);
			}

			if (blendStateInfo.HasMember(destBlend))
			{
				auto ret = blendStateInfo[destBlend].GetString();
				StringToEnum(ret, blendStateDesc._renderTarget[0]._destBlend);
			}

			if (blendStateInfo.HasMember(srcBlendAlpha))
			{
				auto ret = blendStateInfo[srcBlendAlpha].GetString();
				StringToEnum(ret, blendStateDesc._renderTarget[0]._srcBlendAlpha);
			}

			if (blendStateInfo.HasMember(destBlendAlpha))
			{
				auto ret = blendStateInfo[destBlendAlpha].GetString();
				StringToEnum(ret, blendStateDesc._renderTarget[0]._destBlendAlpha);
			}

			if (blendStateInfo.HasMember(blendOP))
			{
				auto ret = blendStateInfo[blendOP].GetString();
				StringToEnum(ret, blendStateDesc._renderTarget[0]._blendOp);
			}

			if (blendStateInfo.HasMember(blendOpAlpha))
			{
				auto ret = blendStateInfo[blendOpAlpha].GetString();
				StringToEnum(ret, blendStateDesc._renderTarget[0]._blendOpAlpha);
			}

			if (blendStateInfo.HasMember(id))
			{
				DUOLCommon::tstring str = blendStateInfo[id].GetString();

				blendStates.emplace(str, blendStateDesc);
			}
		}

		jsonLoader->UnloadJson(blendStatePath);
	}

	const DUOLCommon::tstring depthStencilStatePath(_T("Asset/DataTable/DepthStencilState.json"));
	//depthStencilState
	{
		const TCHAR* depthEnable = _T("DepthEnable");
		const TCHAR* writeEnable = _T("WriteEnable");
		const TCHAR* depthCompareOP = _T("DepthCompareOP");
		const TCHAR* stencilEnable = _T("StencilEnable");
		const TCHAR* stencilReadMask = _T("StencilReadMask");
		const TCHAR* stencilWriteMask = _T("StencilWriteMask");

		const TCHAR* stencilFrontFaceFailOP = _T("StencilFrontFaceFailOP");
		const TCHAR* stencilFrontFaceDepthFailOP = _T("StencilFrontFaceDepthFailOP");
		const TCHAR* stencilFrontFacePassOP = _T("StencilFrontFacePassOP");
		const TCHAR* stencilFrontFaceComparisonOP = _T("StencilFrontFaceComparisonOP");

		const TCHAR* stencilBackFaceFailOP = _T("StencilBackFaceFailOP");
		const TCHAR* stencilBackFaceDepthFailOP = _T("StencilBackFaceDepthFailOP");
		const TCHAR* stencilBackFacePassOP = _T("StencilBackFacePassOP");
		const TCHAR* stencilBackFaceComparisonOP = _T("StencilBackFaceComparisonOP");

		auto depthStencilStateInfos = jsonLoader->LoadJson(depthStencilStatePath);

		for (auto& depthStencilInfo : depthStencilStateInfos->GetArray())
		{
			DUOLGraphicsLibrary::DepthStencilStateDesc depthStencilStateDesc;

			if (depthStencilInfo.HasMember(depthEnable))
			{
				auto ret = depthStencilInfo[depthEnable].GetBool();
				depthStencilStateDesc._depthEnable = ret;
			}

			if (depthStencilInfo.HasMember(writeEnable))
			{
				auto ret = depthStencilInfo[writeEnable].GetBool();
				depthStencilStateDesc._writeEnable = ret;
			}

			if (depthStencilInfo.HasMember(depthCompareOP))
			{
				auto ret = depthStencilInfo[depthCompareOP].GetString();
				StringToEnum(ret, depthStencilStateDesc._depthCompareOp);
			}

			if (depthStencilInfo.HasMember(stencilEnable))
			{
				auto ret = depthStencilInfo[stencilEnable].GetBool();
				depthStencilStateDesc._stencilEnable = ret;
			}

			if (depthStencilInfo.HasMember(stencilReadMask))
			{
				auto ret = depthStencilInfo[stencilReadMask].GetInt();
				depthStencilStateDesc._stencilReadMask = ret;
			}

			if (depthStencilInfo.HasMember(stencilWriteMask))
			{
				auto ret = depthStencilInfo[stencilWriteMask].GetInt();
				depthStencilStateDesc._stencilWriteMask = ret;
			}

			// front
			if (depthStencilInfo.HasMember(stencilFrontFaceFailOP))
			{
				auto ret = depthStencilInfo[stencilFrontFaceFailOP].GetString();
				StringToEnum(ret, depthStencilStateDesc._frontFace._stencilFailOp);
			}

			if (depthStencilInfo.HasMember(stencilFrontFaceDepthFailOP))
			{
				auto ret = depthStencilInfo[stencilFrontFaceDepthFailOP].GetString();
				StringToEnum(ret, depthStencilStateDesc._frontFace._stencilDepthFailOp);
			}

			if (depthStencilInfo.HasMember(stencilFrontFacePassOP))
			{
				auto ret = depthStencilInfo[stencilFrontFacePassOP].GetString();
				StringToEnum(ret, depthStencilStateDesc._frontFace._stencilPassOp);
			}

			if (depthStencilInfo.HasMember(stencilFrontFaceComparisonOP))
			{
				auto ret = depthStencilInfo[stencilFrontFaceComparisonOP].GetString();
				StringToEnum(ret, depthStencilStateDesc._frontFace._comparisonOp);
			}

			// back
			if (depthStencilInfo.HasMember(stencilBackFaceFailOP))
			{
				auto ret = depthStencilInfo[stencilBackFaceFailOP].GetString();
				StringToEnum(ret, depthStencilStateDesc._backFace._stencilFailOp);
			}

			if (depthStencilInfo.HasMember(stencilBackFaceDepthFailOP))
			{
				auto ret = depthStencilInfo[stencilBackFaceDepthFailOP].GetString();
				StringToEnum(ret, depthStencilStateDesc._backFace._stencilDepthFailOp);
			}

			if (depthStencilInfo.HasMember(stencilBackFacePassOP))
			{
				auto ret = depthStencilInfo[stencilBackFacePassOP].GetString();
				StringToEnum(ret, depthStencilStateDesc._backFace._stencilPassOp);
			}

			if (depthStencilInfo.HasMember(stencilBackFaceComparisonOP))
			{
				auto ret = depthStencilInfo[stencilBackFaceComparisonOP].GetString();
				StringToEnum(ret, depthStencilStateDesc._backFace._comparisonOp);
			}

			if (depthStencilInfo.HasMember(id))
			{
				DUOLCommon::tstring str = depthStencilInfo[id].GetString();

				depthStencilStates.emplace(str, depthStencilStateDesc);
			}
		}

		jsonLoader->UnloadJson(depthStencilStatePath);
	}

	const DUOLCommon::tstring rasterizerStatePath(_T("Asset/DataTable/RasterizerState.json"));
	//rasterizerState
	{
		const TCHAR* FillMode = _T("FillMode");
		const TCHAR* CullMode = _T("CullMode");
		const TCHAR* FrontCounterClockWise = _T("FrontCounterClockWise");
		const TCHAR* DepthBias = _T("DepthBias");
		const TCHAR* DepthBiasClamp = _T("DepthBiasClamp");
		const TCHAR* SlopeScaledDepthBias = _T("SlopeScaledDepthBias");
		const TCHAR* DepthClipEnable = _T("DepthClipEnable");
		const TCHAR* ScissorEnable = _T("ScissorEnable");
		const TCHAR* MultiSampleEnable = _T("MultiSampleEnable");
		const TCHAR* AntialiasedLineEnable = _T("AntialiasedLineEnable");

		auto rasterizerStateInfos = jsonLoader->LoadJson(rasterizerStatePath);

		for (auto& raterizerInfo : rasterizerStateInfos->GetArray())
		{
			DUOLGraphicsLibrary::RasterizerStateDesc rasterizerStateDesc;

			if (raterizerInfo.HasMember(FillMode))
			{
				auto ret = raterizerInfo[FillMode].GetString();
				StringToEnum(ret, rasterizerStateDesc._fillMode);
			}

			if (raterizerInfo.HasMember(CullMode))
			{
				auto ret = raterizerInfo[CullMode].GetString();
				StringToEnum(ret, rasterizerStateDesc._cullMode);
			}

			if (raterizerInfo.HasMember(FrontCounterClockWise))
			{
				rasterizerStateDesc._frontCounterClockWise = raterizerInfo[FrontCounterClockWise].GetBool();
			}

			if (raterizerInfo.HasMember(DepthBias))
			{
				rasterizerStateDesc._depthBias = raterizerInfo[DepthBias].GetInt();
			}

			if (raterizerInfo.HasMember(DepthBiasClamp))
			{
				rasterizerStateDesc._depthBiasClamp = raterizerInfo[DepthBiasClamp].GetFloat();
			}

			if (raterizerInfo.HasMember(SlopeScaledDepthBias))
			{
				rasterizerStateDesc._slopeScaledDepthBias = raterizerInfo[SlopeScaledDepthBias].GetFloat();
			}

			if (raterizerInfo.HasMember(DepthClipEnable))
			{
				rasterizerStateDesc._depthClipEnable = raterizerInfo[DepthClipEnable].GetBool();
			}

			if (raterizerInfo.HasMember(ScissorEnable))
			{
				rasterizerStateDesc._scissorEnable = raterizerInfo[ScissorEnable].GetBool();
			}

			if (raterizerInfo.HasMember(MultiSampleEnable))
			{
				rasterizerStateDesc._multiSampleEnable = raterizerInfo[MultiSampleEnable].GetBool();
			}

			if (raterizerInfo.HasMember(AntialiasedLineEnable))
			{
				rasterizerStateDesc._antialiasedLineEnable = raterizerInfo[AntialiasedLineEnable].GetBool();
			}

			if (raterizerInfo.HasMember(id))
			{
				DUOLCommon::tstring str = raterizerInfo[id].GetString();
				rasterizerStates.emplace(str, rasterizerStateDesc);
			}
		}

		jsonLoader->UnloadJson(rasterizerStatePath);
	}

	const DUOLCommon::tstring pipelineStateTable(_T("Asset/DataTable/PipelineStateTable.json"));
	auto pipelineStates = jsonLoader->LoadJson(pipelineStateTable);

	const TCHAR* vertexShader = _T("VertexShader");
	const TCHAR* pixelShader = _T("PixelShader");
	const TCHAR* hullShader = _T("HullShader");
	const TCHAR* domainShader = _T("DomainShader");
	const TCHAR* geometryShader = _T("GeometryShader");
	const TCHAR* renderTargetTexture = _T("RenderTargetTexture");
	const TCHAR* depthStencilView = _T("DepthStencilView");
	const TCHAR* shaderResourceView = _T("ShaderResourceView");
	const TCHAR* primitveTopology = _T("PrimitveTopology");
	const TCHAR* blendStateID = _T("BlendStateDesc");
	const TCHAR* rasterizerStateID = _T("RasterizerStateDesc");
	const TCHAR* depthStencilStateID = _T("DepthStencilStateDesc");
	const TCHAR* computeShaderID = _T("ComputeShader");

	for (auto& pipelineState : pipelineStates->GetArray())
	{
		if (pipelineState.HasMember(computeShaderID))
		{
			DUOLGraphicsLibrary::ComputePipelineStateDesc computePipelineStateDesc;

			auto ret = pipelineState[computeShaderID].GetString();
			computePipelineStateDesc._computeShader = resourceManager->GetShader(Hash::Hash64(ret));

			if (pipelineState.HasMember(id))
			{
				DUOLCommon::tstring str = pipelineState[id].GetString();
				auto pipeline = resourceManager->CreatePipelineState(Hash::Hash64(str), computePipelineStateDesc);
			}
		}
		else
		{
			DUOLGraphicsLibrary::PipelineStateDesc pipelineStateDesc;

			if (pipelineState.HasMember(vertexShader))
			{
				auto ret = pipelineState[vertexShader].GetString();
				pipelineStateDesc._vertexShader = resourceManager->GetShader(Hash::Hash64(ret));
			}

			if (pipelineState.HasMember(hullShader))
			{
				auto ret = pipelineState[hullShader].GetString();
				pipelineStateDesc._hullShader = resourceManager->GetShader(Hash::Hash64(ret));
			}

			if (pipelineState.HasMember(geometryShader))
			{
				auto ret = pipelineState[geometryShader].GetString();
				pipelineStateDesc._geometryShader = resourceManager->GetShader(Hash::Hash64(ret));
			}

			if (pipelineState.HasMember(domainShader))
			{
				auto ret = pipelineState[domainShader].GetString();
				pipelineStateDesc._domainShader = resourceManager->GetShader(Hash::Hash64(ret));
			}

			if (pipelineState.HasMember(pixelShader))
			{
				auto ret = pipelineState[pixelShader].GetString();
				pipelineStateDesc._pixelShader = resourceManager->GetShader(Hash::Hash64(ret));
			}

			if (pipelineState.HasMember(primitveTopology))
			{
				if (!StringToEnum(pipelineState[primitveTopology].GetString(), pipelineStateDesc._primitiveTopology))
				{
					assert("s");
				}
			}

			if (pipelineState.HasMember(blendStateID))
			{
				auto ret = pipelineState[blendStateID].GetString();
				auto foundinfo = blendStates.find(ret);

				if (foundinfo != blendStates.end())
				{
					pipelineStateDesc._blendStateDesc = foundinfo->second;
				}
			}

			if (pipelineState.HasMember(depthStencilStateID))
			{
				auto ret = pipelineState[depthStencilStateID].GetString();
				auto foundinfo = depthStencilStates.find(ret);

				if (foundinfo != depthStencilStates.end())
				{
					pipelineStateDesc._depthStencilStateDesc = foundinfo->second;
				}
			}

			if (pipelineState.HasMember(rasterizerStateID))
			{
				auto ret = pipelineState[rasterizerStateID].GetString();
				auto foundinfo = rasterizerStates.find(ret);

				if (foundinfo != rasterizerStates.end())
				{
					pipelineStateDesc._rasterizerStateDesc = foundinfo->second;
				}
			}

			if (pipelineState.HasMember(id))
			{
				DUOLCommon::tstring str = pipelineState[id].GetString();
				auto pipeline = resourceManager->CreatePipelineState(Hash::Hash64(str), pipelineStateDesc);
			}
		}

	}

	jsonLoader->UnloadJson(pipelineStateTable);

	return false;
}

JSON_SERIALIZE_ENUM(DUOLGraphicsEngine::PipelineType,
	{
		{DUOLGraphicsEngine::PipelineType::Render,					_T("Render")}
		,{DUOLGraphicsEngine::PipelineType::PostProcessing,			_T("PostProcessing")}
	//,{DUOLGraphicsEngine::PipelineType::OrderIndependentTransparency,			_T("OrderIndependentTransparency")}
	});

bool DUOLGraphicsEngine::TableLoader::LoadRenderingPipelineTable(ResourceManager* resourceManager)
{
	auto jsonLoader = DUOLJson::JsonReader::GetInstance();

	const DUOLCommon::tstring renderingPipelineTable(_T("Asset/DataTable/RenderingPipelineTable.json"));

	auto renderingPipelines = jsonLoader->LoadJson(renderingPipelineTable);

	const TCHAR* id = _T("ID");
	const TCHAR* renderTargetTexture = _T("RenderTargetTexture");
	const TCHAR* depthStencilView = _T("DepthStencilView");
	const TCHAR* shaderResourceView = _T("ShaderResourceView");
	const TCHAR* pipelineTypeToken = _T("PipelineType");
	const TCHAR* postProcessingpPipeline = _T("PostProcessingPipeline");
	const TCHAR* sampler = _T("Sampler");


	for (auto& renderingPipeline : renderingPipelines->GetArray())
	{
		DUOLGraphicsLibrary::RenderPass renderPass;
		renderPass._depthStencilViewRef = nullptr;

		DUOLGraphicsLibrary::ResourceViewLayout textureResourceViewLayout;
		DUOLGraphicsLibrary::ResourceViewLayout samplerResourceViewLayout;

		PipelineType pipelineType = PipelineType::Render;

		if (renderingPipeline.HasMember(renderTargetTexture))
		{
			renderPass._renderTargetViewRefs.reserve(renderingPipeline[renderTargetTexture].Size());

			for (auto& renderTargetTexture : renderingPipeline[renderTargetTexture].GetArray())
			{
				auto rtvName = renderTargetTexture.GetString();

				auto rtv = resourceManager->GetRenderTarget(Hash::Hash64(rtvName));
				if (rtv == nullptr)
				{
				}
				else
				{
					renderPass._renderTargetViewRefs.emplace_back(rtv);
				}
			}
		}

		if (renderingPipeline.HasMember(depthStencilView))
		{
			renderPass._depthStencilViewRef = resourceManager->GetRenderTarget(Hash::Hash64(renderingPipeline[depthStencilView].GetString()));
		}

		if (renderingPipeline.HasMember(shaderResourceView))
		{
			textureResourceViewLayout._resourceViews.reserve(renderingPipeline[shaderResourceView].Size());

			for (rapidjson::SizeType resourceViewIndex = 0; resourceViewIndex < renderingPipeline[shaderResourceView].Size(); resourceViewIndex++)
			{
				auto srvName = renderingPipeline[shaderResourceView][resourceViewIndex].GetString();
				auto srv = resourceManager->GetTexture(Hash::Hash64(srvName));

				if (srv == nullptr)
				{
				}
				else
				{
					textureResourceViewLayout._resourceViews.emplace_back(srv, resourceViewIndex, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE));
				}
			}
		}

		if (renderingPipeline.HasMember(sampler))
		{
			samplerResourceViewLayout._resourceViews.reserve(renderingPipeline[sampler].Size());

			for (rapidjson::SizeType resourceViewIndex = 0; resourceViewIndex < renderingPipeline[sampler].Size(); resourceViewIndex++)
			{
				auto samplerName = renderingPipeline[sampler][resourceViewIndex].GetString();
				auto sampler = resourceManager->GetSampler(Hash::Hash64(samplerName));

				if (sampler == nullptr)
				{
				}
				else
				{
					samplerResourceViewLayout._resourceViews.emplace_back(sampler, resourceViewIndex, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SAMPLER), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::COMPUTESTAGE));
				}
			}
		}

		if (renderingPipeline.HasMember(pipelineTypeToken))
		{
			if (!StringToEnum(renderingPipeline[pipelineTypeToken].GetString(), pipelineType))
			{
				assert("error");
			}
		}


		if (renderingPipeline.HasMember(id))
		{
			auto pipeline = resourceManager->CreateRenderingPipeline(renderingPipeline[id].GetString(), pipelineType, renderPass, textureResourceViewLayout, samplerResourceViewLayout);

			if (renderingPipeline.HasMember(postProcessingpPipeline))
			{
				pipeline->SetPipelineState(resourceManager->GetPipelineState(Hash::Hash64(renderingPipeline[postProcessingpPipeline].GetString())));
			}
		}

	}
	return false;
}