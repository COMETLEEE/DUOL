/**

	@file      Inspector.h
	@brief     일단 테스트 삼아서 만들어보는 클래스.. Imgui 처음 사용해본다..
	@details   일단 인스펙터 클래스에 한번에 다 구현해 놓고. 여러개의 클래스로 나누던지, 함수를 나누던지 하자..!
	@author    SinSeongHyeon
	@date      8.11.2022
	@copyright © SinSeongHyeon, 2022. All right reserved.

**/
#pragma once

#include "ImGuiRnedererBase.h"
#include "../../DUOLMath/include/DUOLMath/DUOLMath.h"
#include <memory>

class Inspector : public Muscle::ImGuiRnedererBase
{
public:
	Inspector(std::shared_ptr<Muscle::GameObject> _gameObject);

	virtual ~Inspector() override = default;

private:
	std::shared_ptr<Muscle::ParticleRenderer> _selectedParticle;

	std::shared_ptr<Muscle::GameObject> _selectedGameObject;

	bool _isTextureBoxOpen;

	bool _isTrailTextureBoxOpen;

	std::shared_ptr<Muscle::GameObject> _collsionPlane[8]; // Particle이 선택됐을 때 보여주는 오브젝트니, 인스펙터가 관리하는게 맞을지도..?
private:
#pragma region Particle
	/**
	 * @brief 파티클 Common 정보 관련 Gui 함수.
	*/
	void ParticleSystemCommonInfo();
	/**
	 * \brief StartSize 함수의 양이 많아져서 따로 분리.
	 */
	void ParticleSystemCommonInfo_StartSize();
	/**
	 * \brief StartSize 함수의 양이 많아져서 따로 분리.
	 */
	void ParticleSystemCommonInfo_StartSpeed();
	/**
	* \brief StartSize 함수의 양이 많아져서 따로 분리.
	*/
	void ParticleSystemCommonInfo_StartLifetime();
	/**
	* \brief StartRotation 함수의 양이 많아져서 따로 분리.
	*/
	void ParticleSystemCommonInfo_StartRotation();
	/**
	* \brief StartColor 함수의 양이 많아져서 따로 분리.
	*/
	void ParticleSystemCommonInfo_StartColor();
	/**
	* \brief GravityModifier 함수의 양이 많아져서 따로 분리.
	*/
	void ParticleSystemCommonInfo_GravityModifier();
	/**
	 * @brief 파티클 방출 정보 관련 Gui 함수.
	*/
	void Emission();
	/**
	 * @brief 파티클 Sahpe 정보 관련 Gui 함수. 구, 원기둥, 원뿔 등 방출 방식을 설정하는 곳.
	*/
	void Shape();
	/**
	 * @brief 파티클 라이브 타임에 따른 속력 정보 관련 Gui 함수.
	*/
	void Velocity_Over_Lifetime();
	/**
	 * @brief 파티클 Force_over_Lifetime 정보 관련 Gui 함수.
	*/
	void Force_over_Lifetime();
	/**
	 * @brief 파티클 Color_over_Lifetime 정보 관련 Gui 함수.
	*/
	void Color_over_Lifetime();
	/**
	 * @brief 파티클 Size_over_Lifetime 정보 관련 Gui 함수.
	*/
	void Size_over_Lifetime();
	/**
	 * @brief 파티클 Rotation_over_Lifetime 정보 관련 Gui 함수.
	*/
	void Rotation_over_Lifetime();
	/**
	 * @brief 파티클 Noise 정보 관련 Gui 함수.
	*/
	void Noise();
	/**
	 * @brief 파티클 Texture_Sheet_Animation 정보 관련 Gui 함수.
	*/
	void Collision();
	/**
	 * @brief 파티클 Collision 정보 관련 Gui 함수.
	*/
	void Texture_Sheet_Animation();
	/**
	 * @brief 파티클 Trails 정보 관련 Gui 함수.
	*/
	void Trails();
	/**
	 * @brief 파티클 Renderer 정보 관련 Gui 함수.
	*/
	void Renderer();
#pragma endregion
#pragma region GameObject
	void GameObjectInfo();
	void TranformInfo();
#pragma endregion

protected:
	/**
	* \brief 이 함수로 그래픽스에 넘겨줄 함수 객체를 설정하자.
	*/
	virtual void SetRenderingFunc() override;
public:
	virtual void Start() override;

	virtual void Update() override;
};

