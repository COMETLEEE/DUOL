#include "DUOLGraphicsLibrary/Renderer/Renderer.h"
#include "DUOLGraphicsLibrary/Core/Module.h"
#include <string>

namespace DUOLGraphicsLibrary
{
	static std::map<Renderer*, std::unique_ptr<Module>> g_renderers;

	Renderer* Renderer::CreateRenderer(const RendererDesc& renderDesc)
	{
		std::unique_ptr<Module> module = nullptr;
		std::string moduleName;

		moduleName += "DUOLGraphicsLibrary_";

		switch (renderDesc._moduleType)
		{
		case ModuleType::DIRECTX11:
		{
			moduleName += "Direct3D11.dll";
			module = Module::Load(moduleName.c_str());

			break;
		}
		default:;
		}

		typedef DUOLGraphicsLibrary::Renderer* (*RendererAlloc)(const DUOLGraphicsLibrary::RendererDesc& renderDesc);
		auto AllocFunc = reinterpret_cast<RendererAlloc>(module->LoadProcedure("DUOLGraphics_Renderer_Alloc"));

		DUOLGraphicsLibrary::Renderer* renderer = AllocFunc(renderDesc);

		g_renderers.emplace(renderer, std::move(module));

		return renderer;
	}

	void Renderer::DeleteRenderer(Renderer* renderer)
	{
		auto foundModule = g_renderers.find(renderer);

		typedef void (*RenderFree)(DUOLGraphicsLibrary::Renderer* renderer);
		auto FreeFunc = reinterpret_cast<RenderFree>(foundModule->second->LoadProcedure("DUOLGraphics_Renderer_Free"));

		FreeFunc(renderer);

		g_renderers.erase(foundModule);
	}

}

