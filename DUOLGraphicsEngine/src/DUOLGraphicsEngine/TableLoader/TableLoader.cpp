#include "TableLoader.h"
#include <DUOLJson/JsonReader.h>

#include "DUOLGraphicsEngine/RenderManager/RenderingPipeline.h"
#include "DUOLGraphicsEngine/ResourceManager/ResourceManager.h"
#include "DUOLGraphicsEngine/Util/Hash/Hash.h"
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
		{DUOLGraphicsLibrary::ResourceFormat::FORMAT_R24G8_TYPELESS,	_T("FORMAT_R24G8_TYPELESS")}
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

	const DUOLCommon::tstring renderTargetTable(_T("Asset/DataTable/RenderTargetTable.json"));

	auto renderTargets = jsonLoader->LoadJson(renderTargetTable);

	const TCHAR* id = _T("ID");
	const TCHAR* textureType = _T("TextureType");
	const TCHAR* extent = _T("Extent");
	const TCHAR* resolutionProportion = _T("ResolutionProportion");
	const TCHAR* percent = _T("Percent");
	const TCHAR* sample = _T("Sample");
	const TCHAR* resourceFormat = _T("ResourceFormat");
	const TCHAR* resourceUsage = _T("ResourceUsage");
	const TCHAR* bindFlag = _T("BindFlag");
	const TCHAR* cpuAccessFlag = _T("CPUAccessFlag");
	const TCHAR* mipLevel = _T("MipLevel");
	const TCHAR* arrayLayer = _T("ArrayLayer");
	const TCHAR* renderTargetType = _T("RenderTargetType");

	for (auto& renderTarget : renderTargets->GetArray())
	{
		DUOLGraphicsLibrary::TextureDesc textureDesc;

		if (renderTarget.HasMember(textureType))
		{
			if (!StringToEnum(renderTarget[textureType].GetString(), textureDesc._type))
			{
				assert("Error");
			}
		}
		if (renderTarget.HasMember(sample))
		{
			textureDesc._sample = renderTarget[sample].GetInt();
		}
		if (renderTarget.HasMember(resourceFormat))
		{
			if (!StringToEnum(renderTarget[resourceFormat].GetString(), textureDesc._format))
			{
				assert("Error");
			}
		}
		if (renderTarget.HasMember(resourceUsage))
		{
			if (!StringToEnum(renderTarget[resourceUsage].GetString(), textureDesc._usage))
			{
				assert("Error");
			}
		}
		if (renderTarget.HasMember(bindFlag))
		{
			for (int i = 0; i < renderTarget[bindFlag].Size(); i++)
			{
				int offset = renderTarget[bindFlag][i].GetInt() - 1;
				if (offset >= 0)
					textureDesc._bindFlags |= (1 << offset);
			}
		}
		if (renderTarget.HasMember(cpuAccessFlag))
		{
			for (int i = 0; i < renderTarget[cpuAccessFlag].Size(); i++)
			{
				int offset = renderTarget[cpuAccessFlag][i].GetInt() - 1;
				if (offset >= 0)
					textureDesc._cpuAccessFlags |= (1 << offset);
			}
		}
		if (renderTarget.HasMember(mipLevel))
		{
			textureDesc._sample = renderTarget[mipLevel].GetInt();
		}
		if (renderTarget.HasMember(arrayLayer))
		{
			textureDesc._sample = renderTarget[arrayLayer].GetInt();
		}
		if (renderTarget.HasMember(extent))
		{
			if (renderTarget[extent].Size() > 3)
			{
				assert("Error");
			}
			for (int i = 0; i < renderTarget[extent].Size(); i++)
			{
				if (i == 0)
					textureDesc._textureExtent.x = renderTarget[extent][i].GetInt();
				if (i == 1)
					textureDesc._textureExtent.y = renderTarget[extent][i].GetInt();
				if (i == 2)
					textureDesc._textureExtent.z = renderTarget[extent][i].GetInt();
			}
		}

		bool isproportionBool = false;
		float percentValue = 0.f;
		DUOLCommon::tstring idResource;

		if (renderTarget.HasMember(percent))
		{
			percentValue = renderTarget[percent].GetFloat();
		}
		if (renderTarget.HasMember(resolutionProportion))
		{
			isproportionBool = renderTarget[resolutionProportion].GetBool();
			if (isproportionBool)
			{
				textureDesc._textureExtent.x = screenSize.x * percentValue;
				textureDesc._textureExtent.y = screenSize.y * percentValue;
			}
		}
		if (renderTarget.HasMember(id))
		{
			idResource = renderTarget[id].GetString();
		}

		DUOLGraphicsLibrary::RenderTargetDesc renderTargetDesc;
		renderTargetDesc._texture = resourceManager->CreateTexture(idResource, textureDesc);

		if (renderTarget.HasMember(renderTargetType))
		{
			if (!StringToEnum(renderTarget[renderTargetType].GetString(), renderTargetDesc._type))
			{
				assert("Error");
			}
		}

		resourceManager->CreateRenderTarget(renderTargetDesc, isproportionBool, percentValue);
	}

	jsonLoader->UnloadJson(renderTargetTable);

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

	const DUOLCommon::tstring shaderTablePath(_T("Asset/DataTable/ShaderTable.json"));

	auto shaderTables = jsonLoader->LoadJson(shaderTablePath);

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
		}

		std::string strsource;
		std::string strprofile;
		std::string strentrypoint;
		std::vector<std::string> macros;

		if (shaderTable.HasMember(source))
		{
			strsource = DUOLCommon::StringHelper::WStringToString(shaderTable[source].GetString());

			shaderDesc._source = strsource.c_str();
		}

		if (shaderTable.HasMember(profile))
		{
			strprofile = DUOLCommon::StringHelper::WStringToString(shaderTable[profile].GetString());

			shaderDesc._profile = strprofile.c_str();
		}

		if (shaderTable.HasMember(entryPoint))
		{
			strentrypoint = DUOLCommon::StringHelper::WStringToString(shaderTable[entryPoint].GetString());

			shaderDesc._entryPoint = strentrypoint.c_str();
		}

		if (shaderTable.HasMember(shaderMacro))
		{
			shaderDesc._shaderMacro.reserve(shaderTable[shaderMacro].Size());
			macros.reserve(shaderTable[shaderMacro].Size());

			for (auto& macro : shaderTable[shaderMacro].GetArray())
			{
				DUOLGraphicsLibrary::ShaderMacroDesc shaderMacro;

				macros.emplace_back(DUOLCommon::StringHelper::WStringToString(macro.GetString()));
				shaderMacro.Name = macros.back().c_str();

				shaderDesc._shaderMacro.emplace_back(shaderMacro);
			}
		}

		if (shaderTable.HasMember(id))
		{
			resourceManager->CreateShader(Hash::Hash64(shaderTable[id].GetString()), shaderDesc);
		}
	}

	jsonLoader->UnloadJson(shaderTablePath);

	return false;
}

JSON_SERIALIZE_ENUM(DUOLGraphicsLibrary::PrimitiveTopology,
	{
		{DUOLGraphicsLibrary::PrimitiveTopology::PRIMITIVE_TOPOLOGY_POINTLIST,			_T("POINTLIST")},
		{DUOLGraphicsLibrary::PrimitiveTopology::PRIMITIVE_TOPOLOGY_LINELIST,			_T("LINELIST")},
		{DUOLGraphicsLibrary::PrimitiveTopology::PRIMITIVE_TOPOLOGY_LINESTRIP,			_T("LINESTRIP")},
		{DUOLGraphicsLibrary::PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST,		_T("TRIANGLELIST")},
		{DUOLGraphicsLibrary::PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,		_T("TRIANGLESTRIP")},
	});

bool DUOLGraphicsEngine::TableLoader::LoadPipelineStateTable(ResourceManager* resourceManager)
{
	auto jsonLoader = DUOLJson::JsonReader::GetInstance();

	const DUOLCommon::tstring pipelineStateTable(_T("Asset/DataTable/PipelineStateTable.json"));
	auto pipelineStates = jsonLoader->LoadJson(pipelineStateTable);
	DUOLGraphicsLibrary::PipelineStateDesc pipelineStateDesc;

	const TCHAR* id = _T("ID");
	const TCHAR* vertexShader = _T("VertexShader");
	const TCHAR* pixelShader = _T("PixelShader");
	const TCHAR* hullShader = _T("HullShader");
	const TCHAR* domainShader = _T("DomainShader");
	const TCHAR* geometryShader = _T("GeometryShader");
	const TCHAR* renderTargetTexture = _T("RenderTargetTexture");
	const TCHAR* depthStencilView = _T("DepthStencilView");
	const TCHAR* shaderResourceView = _T("ShaderResourceView");
	const TCHAR* primitveTopology = _T("PrimitveTopology");
	const TCHAR* blendState = _T("BlendStateDesc");
	const TCHAR* rasterizerState = _T("RasterizerStateDesc");
	const TCHAR* depthStencilState = _T("DepthStencilStateDesc");

	for (auto& pipelineState : pipelineStates->GetArray())
	{

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

		//TODO:: 블렌드스테이트와 래스터라이저는 어떻게 해야할까

		if (pipelineState.HasMember(id))
		{
			resourceManager->CreatePipelineState(Hash::Hash64(pipelineState[id].GetString()), pipelineStateDesc);
		}
	}

	jsonLoader->UnloadJson(pipelineStateTable);

	return false;
}

JSON_SERIALIZE_ENUM(DUOLGraphicsEngine::PipelineType,
	{
		{DUOLGraphicsEngine::PipelineType::Render,					_T("Render")},
		{DUOLGraphicsEngine::PipelineType::PostProcessing,			_T("PostProcessing")}
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

	for (auto& renderingPipeline : renderingPipelines->GetArray())
	{
		DUOLGraphicsLibrary::RenderPass renderPass;
		DUOLGraphicsLibrary::ResourceViewLayout resourceViewLayout;
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
			resourceViewLayout._resourceViews.reserve(renderingPipeline[shaderResourceView].Size());

			for (int resourceViewIndex = 0; resourceViewIndex < renderingPipeline[shaderResourceView].Size(); resourceViewIndex++)
			{
				auto srvName = renderingPipeline[shaderResourceView][resourceViewIndex].GetString();
				auto srv = resourceManager->GetTexture(Hash::Hash64(srvName));

				if (srv == nullptr)
				{
				}
				else
				{
					resourceViewLayout._resourceViews.emplace_back(srv, resourceViewIndex, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));
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
			auto pipeline = resourceManager->CreateRenderingPipeline(renderingPipeline[id].GetString(), pipelineType, renderPass, resourceViewLayout);

			if (pipelineType == PipelineType::PostProcessing)
			{
				if (renderingPipeline.HasMember(postProcessingpPipeline))
				{
					pipeline->SetPipelineState(resourceManager->GetPipelineState(Hash::Hash64(renderingPipeline[postProcessingpPipeline].GetString())));
				}
				else
				{
					assert("error");
				}
			}
		}


	}

	return false;
}
