/**

	@file      ShaderFlagsManager.h
	@brief     셰이더 코드 최적화를 위한 클래스.
	@details   셰이더 코드에서는 if문의 사용을 최소한으로 줄여야하기 때문에 플래그로 분기를 가른다.
	@author    SinSeongHyeon
	@date      04.01.2023
	@copyright © SinSeongHyeon, 2022. All right reserved.

**/

#pragma once
#include <string>
#include <unordered_map>


namespace BasicParticle
{
	// 가능한 3~4개를 초과하지 않도록 하자. 컴파일 시간이 엄청나게 늘어난다.
	enum class Flags
	{
		None = 0,
		ParticleSystemCommonInfo = 1 << 0, // ParticleSystemCommonInfo Module
		Emission = 1 << 1, // Emission Module
		Shape = 1 << 2, // Shape Module
		Velocity_Over_Lifetime = 1 << 3, // Velocity_Over_Lifetime Module
		Force_over_Lifetime = 1 << 4, // Force_over_Lifetime Module
		Color_over_Lifetime = 1 << 5, // Color_over_Lifetime Module
		Size_over_Lifetime = 1 << 6, // Size_over_Lifetime Module
		Rotation_over_Lifetime = 1 << 7, // Rotation_over_Lifetime Module
		Noise = 1 << 8, // Noise Module
		Collision = 1 << 9, // Collision Module
		Texture_Sheet_Animation = 1 << 10, // Texture_Sheet_Animation Module
		Trails = 1 << 11, // Trails Module  
		Renderer = 1 << 12, // Renderer Module

	};
	static const char* Flags_str[] =
	{
	"ParticleSystemCommonInfo\n",
	/*"Emission\n",
	"Shape\n",
	"Velocity_Over_Lifetime\n",
	"Force_over_Lifetime\n",
	"Color_over_Lifetime\n",
	"Size_over_Lifetime\n",
	"Rotation_over_Lifetime\n",
	"Noise\n",
	"Collision\n",
	"Texture_Sheet_Animation\n",
	"Trails\n",
	"Renderer\n"*/
	};
}

class ShaderFlagsManager
{
protected:
	ShaderFlagsManager();

	~ShaderFlagsManager() = default;
public:
	static ShaderFlagsManager& Get();
public:
	BasicParticle::Flags GetFlag(std::string flag_str);
private:
	std::unordered_map<std::string, BasicParticle::Flags> _basicParticeFlags; // map을 사용한 이유는 Key값을 통해 비트 연산을 하여 셰이더를 선택하기 위함.

};
