#include "DUOLGameEngine/Manager/GraphicsSettings.h"

#include <rttr/registration>

#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::ToneMapping>("ToneMapping")
	.constructor<>()
	(
		 rttr::policy::ctor::as_std_shared_ptr
	)
	.property("Exposure", &DUOLGameEngine::ToneMapping::_exposure)
	(
		rttr::detail::metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	);

	rttr::registration::class_<DUOLGameEngine::ScreenSpaceReflection>("ScreenSpaceReflection")
	.constructor<>()
	(
		rttr::policy::ctor::as_std_shared_ptr
	)
	.property("Stride", &DUOLGameEngine::ScreenSpaceReflection::_stride)
	(
		rttr::detail::metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("MaxSteps", &DUOLGameEngine::ScreenSpaceReflection::_maxSteps)
	(
			rttr::detail::metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("MaxDistance", &DUOLGameEngine::ScreenSpaceReflection::_maxDistance)
	(
		rttr::detail::metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("StrideZCutOff", &DUOLGameEngine::ScreenSpaceReflection::_strideZCutoff)
	(
		rttr::detail::metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("NumMips", &DUOLGameEngine::ScreenSpaceReflection::_numMips)
	(
		rttr::detail::metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("FadeStart", &DUOLGameEngine::ScreenSpaceReflection::_fadeStart)
	(
		rttr::detail::metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("FadeEnd", &DUOLGameEngine::ScreenSpaceReflection::_fadeEnd)
	(
		rttr::detail::metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	).property("Thickness", &DUOLGameEngine::ScreenSpaceReflection::_zThickness)
	(
		rttr::detail::metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	);


	rttr::registration::class_<DUOLGameEngine::ExponentialHeightFog>("ExponentialHeightFog")
	.constructor<>()
	(
		rttr::policy::ctor::as_std_shared_ptr
	)
	.property("FogDensity", &DUOLGameEngine::ExponentialHeightFog::_fogDensity)
	(
		rttr::detail::metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		)
	.property("FogHeightFallOff", &DUOLGameEngine::ExponentialHeightFog::_fogHeightFalloff)
	(
		rttr::detail::metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		)
	.property("FogCutOffDistnace", &DUOLGameEngine::ExponentialHeightFog::_fogCutOffDistance)
	(
		rttr::detail::metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		)
	.property("FogStartDistance", &DUOLGameEngine::ExponentialHeightFog::_fogStartDistance)
	(
		rttr::detail::metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		)
	.property("FogMaxOpacity", &DUOLGameEngine::ExponentialHeightFog::_fogMaxOpacity)
	(
		rttr::detail::metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		)
	.property("FogHeight", &DUOLGameEngine::ExponentialHeightFog::_fogHeight)
	(
		rttr::detail::metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float3)
		)
	.property("FogScatteringColor", &DUOLGameEngine::ExponentialHeightFog::_fogScatteringColor)
	(
		rttr::detail::metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float3)
	);

	rttr::registration::class_<DUOLGameEngine::RimLight>("RimLight")
		.constructor<>()
		(
			rttr::policy::ctor::as_std_shared_ptr
			)
	.property("ClipPoint", &DUOLGameEngine::RimLight::_clipPoint)
		(
			rttr::detail::metadata(DUOLCommon::MetaDataType::Serializable, true)
			, metadata(DUOLCommon::MetaDataType::Inspectable, true)
			, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float))
	.property("RimDecrease", &DUOLGameEngine::RimLight::_rimDecrease)
		(
			rttr::detail::metadata(DUOLCommon::MetaDataType::Serializable, true)
			, metadata(DUOLCommon::MetaDataType::Inspectable, true)
			, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float));

	rttr::registration::class_<DUOLGameEngine::Bloom>("Bloom")
		.constructor<>()
		(
			rttr::policy::ctor::as_std_shared_ptr
			)
		.property("Intensity", &DUOLGameEngine::Bloom::_intensity)
		(
			rttr::detail::metadata(DUOLCommon::MetaDataType::Serializable, true)
			, metadata(DUOLCommon::MetaDataType::Inspectable, true)
			, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float))
		.property("Threshhold", &DUOLGameEngine::Bloom::_threshhold)
		(
			rttr::detail::metadata(DUOLCommon::MetaDataType::Serializable, true)
			, metadata(DUOLCommon::MetaDataType::Inspectable, true)
			, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float));

	rttr::registration::class_<DUOLGameEngine::GraphicsSetting>("GraphicsSetting")
	.constructor<>()
	(
	)
	.property("ScreenSpaceReflection", &DUOLGameEngine::GraphicsSetting::_screenSpaceReflection)
	(
		rttr::detail::metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Structure)
	)
	.property("ToneMapping", &DUOLGameEngine::GraphicsSetting::_toneMapping)
	(
		rttr::detail::metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Structure)
	)
	.property("ExponentialHeightFog", &DUOLGameEngine::GraphicsSetting::_exponentialHeightFog)
	(
		rttr::detail::metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Structure)
	)
		.property("RimLight", &DUOLGameEngine::GraphicsSetting::_rimLight)
		(
			rttr::detail::metadata(DUOLCommon::MetaDataType::Serializable, true)
			, metadata(DUOLCommon::MetaDataType::Inspectable, true)
			, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Structure)
			)
		.property("Bloom", &DUOLGameEngine::GraphicsSetting::_bloom)
		(
			rttr::detail::metadata(DUOLCommon::MetaDataType::Serializable, true)
			, metadata(DUOLCommon::MetaDataType::Inspectable, true)
			, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Structure)
			);
}

float DUOLGameEngine::ToneMapping::GetExposure() const
{
	return _exposure;
}

void DUOLGameEngine::ToneMapping::SetExposure(float exposure)
{
	_exposure = exposure;
}

float DUOLGameEngine::ScreenSpaceReflection::GetStride() const
{
	return _stride;
}

void DUOLGameEngine::ScreenSpaceReflection::SetStride(float stride)
{
	_stride = stride;
}

float DUOLGameEngine::ScreenSpaceReflection::GetMaxSteps() const
{
	return _maxSteps;
}

void DUOLGameEngine::ScreenSpaceReflection::SetMaxSteps(float maxSteps)
{
	_maxSteps = maxSteps;
}

float DUOLGameEngine::ScreenSpaceReflection::GetMaxDistance() const
{
	return _maxDistance;
}

void DUOLGameEngine::ScreenSpaceReflection::SetMaxDistance(float maxDistance)
{
	_maxDistance = maxDistance;
}

float DUOLGameEngine::ScreenSpaceReflection::GetStrideZCutoff() const
{
	return _strideZCutoff;
}

void DUOLGameEngine::ScreenSpaceReflection::SetStrideZCutoff(float strideZCutoff)
{
	_strideZCutoff = strideZCutoff;
}

float DUOLGameEngine::ScreenSpaceReflection::GetNumMips() const
{
	return _numMips;
}

void DUOLGameEngine::ScreenSpaceReflection::SetNumMips(float numMips)
{
	_numMips = numMips;
}

float DUOLGameEngine::ScreenSpaceReflection::GetFadeStart() const
{
	return _fadeStart;
}

void DUOLGameEngine::ScreenSpaceReflection::SetFadeStart(float fadeStart)
{
	_fadeStart = fadeStart;
}

float DUOLGameEngine::ScreenSpaceReflection::GetFadeEnd() const
{
	return _fadeEnd;
}

void DUOLGameEngine::ScreenSpaceReflection::SetFadeEnd(float fadeEnd)
{
	_fadeEnd = fadeEnd;
}

float DUOLGameEngine::ScreenSpaceReflection::GetZThickness() const
{
	return _zThickness;
}

void DUOLGameEngine::ScreenSpaceReflection::SetZThickness(float zThickness)
{
	_zThickness = zThickness;
}

float DUOLGameEngine::ExponentialHeightFog::GetFogDensity() const
{
	return _fogDensity;
}

void DUOLGameEngine::ExponentialHeightFog::SetFogDensity(float fogDensity)
{
	_fogDensity = fogDensity;
}

float DUOLGameEngine::ExponentialHeightFog::GetFogMaxOpacity() const
{
	return _fogMaxOpacity;
}

void DUOLGameEngine::ExponentialHeightFog::SetFogMaxOpacity(float fogMaxOpacity)
{
	_fogMaxOpacity = fogMaxOpacity;
}

float DUOLGameEngine::ExponentialHeightFog::GetFogStartDistance() const
{
	return _fogStartDistance;
}

void DUOLGameEngine::ExponentialHeightFog::SetFogStartDistance(float fogStartDistance)
{
	_fogStartDistance = fogStartDistance;
}

float DUOLGameEngine::ExponentialHeightFog::GetFogCutOffDistance() const
{
	return _fogCutOffDistance;
}

void DUOLGameEngine::ExponentialHeightFog::SetFogCutOffDistance(float fogCutOffDistance)
{
	_fogCutOffDistance = fogCutOffDistance;
}

float DUOLGameEngine::ExponentialHeightFog::GetFogHeightFalloff() const
{
	return _fogHeightFalloff;
}

void DUOLGameEngine::ExponentialHeightFog::SetFogHeightFalloff(float fogHeightFalloff)
{
	_fogHeightFalloff = fogHeightFalloff;
}

DUOLMath::Vector3 DUOLGameEngine::ExponentialHeightFog::GetFogHeight() const
{
	return _fogHeight;
}

void DUOLGameEngine::ExponentialHeightFog::SetFogHeight(const DUOLMath::Vector3& fogHeight)
{
	_fogHeight = fogHeight;
}

DUOLMath::Vector3 DUOLGameEngine::ExponentialHeightFog::GetFogScatteringColor() const
{
	return _fogScatteringColor;
}

void DUOLGameEngine::ExponentialHeightFog::SetFogScatteringColor(const DUOLMath::Vector3& fogScatteringColor)
{
	_fogScatteringColor = fogScatteringColor;
}

float DUOLGameEngine::ExponentialHeightFog::GetLightScatterFlag() const
{
	return _lightScatterFlag;
}

void DUOLGameEngine::ExponentialHeightFog::SetLightScatterFlag(float lightScatterFlag)
{
	_lightScatterFlag = lightScatterFlag;
}

DUOLGameEngine::GraphicsSetting::GraphicsSetting() :
	_screenSpaceReflection()
	, _toneMapping()
	, _exponentialHeightFog()
	, _rimLight()
{

}

DUOLGameEngine::GraphicsSetting::~GraphicsSetting()
{
}

