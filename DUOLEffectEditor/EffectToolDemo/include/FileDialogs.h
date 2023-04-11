#pragma once

#include <memory>
#include <vector>
#include "StringHelper.h"

namespace MuscleGrapics
{
	struct RenderingData_Particle;
}
namespace PrevVersion
{
	struct RenderingData_Particle;
}
class FileDialogs
{
public:
	static DUOLCommon::tstring OpenTextureFile();

	static std::vector<MuscleGrapics::RenderingData_Particle> LoadParticleFile();

	static PrevVersion::RenderingData_Particle LoadPreviousVersionParticleFile();

	static void SaveParticleFile(std::vector<MuscleGrapics::RenderingData_Particle>& data);

	static void SaveAsParticleFile(std::vector<MuscleGrapics::RenderingData_Particle>& data);

	static void UpdateAllFileToLatestVersionInFolder();
private:
	static DUOLCommon::tstring OpenFile(const char* filter);
	static DUOLCommon::tstring OpenFolder();
	static DUOLCommon::tstring SaveFile(const char* filter);
};

