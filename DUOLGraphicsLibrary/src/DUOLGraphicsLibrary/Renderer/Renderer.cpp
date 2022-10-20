#include "DUOLGraphicsLibrary/Renderer/Renderer.h"
#include "DUOLGraphicsLibrary/Core/Module.h"
#include <string>

namespace DUOLGraphicsLibrary
{
	static std::map<std::unique_ptr<Renderer>, std::unique_ptr<Module>> g_renderers;

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

		std::unique_ptr<Renderer> uniqueRenderer(renderer);

		g_renderers.emplace(std::move(uniqueRenderer), std::move(module));

		return renderer;
	}

	void Renderer::DeleteRenderer(Renderer* renderer)
	{

	}

	void Renderer::DeleteAll()
	{
	}
}

