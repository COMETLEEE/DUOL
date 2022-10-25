/**

    @file    PhysicsMaterial.h
    @brief   Physics 연산에 사용되는 객체의 Material
    @details -
    @author  JKim
    @date    25.10.2022

**/
#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include "PxPhysicsAPI.h"
#include "PhysicsDescriptions.h"

namespace DUOLPhysics
{
    using namespace physx;

    struct HidedPhysicsMaterialDesc
    {
        PhysicsMaterialDesc _default;

        PxPhysics* _physics;
    };

 /**

     @class   PhysicsMaterial
     @brief   Physics 연산에 사용되는 객체의 Material
     @details -

 **/
	class PhysicsMaterial
	{
    public:
        /**
            @brief   PhysicsMaterial 클래스 생성자
            @details -
        **/
        PhysicsMaterial();

        /**
            @brief   PhysicsMaterial 클래스 default 소멸자
            @details -
        **/
        ~PhysicsMaterial() = default;

    private:
        PxMaterial* _material;

    public:
        /**
            @brief   Material 생성
            @details -
            @param   materialDesc - Material 생성에 필요한 값
        **/
        void CreateMaterial(HidedPhysicsMaterialDesc& materialDesc);

        /**
            @brief   Material 할당 해제
            @details -
        **/
        void Release();
	};
}