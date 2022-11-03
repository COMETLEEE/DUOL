#pragma once
#include <memory>
#include <string>

#include "FBXParserDLL/FBXParserDLL.h"

struct FBXModel;

namespace DUOLParser
{
	class DUOLParserBase
	{
	public:
		FBXIMPORTER_DLL DUOLParserBase();
		FBXIMPORTER_DLL virtual ~DUOLParserBase();

	public:
		static FBXIMPORTER_DLL std::shared_ptr<DUOLParserBase> Create();

		FBXIMPORTER_DLL virtual std::shared_ptr<FBXModel> LoadFBX(const std::string& path) abstract;
	};
}