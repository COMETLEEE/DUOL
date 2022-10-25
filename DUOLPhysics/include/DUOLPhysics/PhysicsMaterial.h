/**

    @file    PhysicsMaterial.h
    @brief   Physics ���꿡 ���Ǵ� ��ü�� Material
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
     @brief   Physics ���꿡 ���Ǵ� ��ü�� Material
     @details -

 **/
	class PhysicsMaterial
	{
    public:
        /**
            @brief   PhysicsMaterial Ŭ���� ������
            @details -
        **/
        PhysicsMaterial();

        /**
            @brief   PhysicsMaterial Ŭ���� default �Ҹ���
            @details -
        **/
        ~PhysicsMaterial() = default;

    private:
        PxMaterial* _material;

    public:
        /**
            @brief   Material ����
            @details -
            @param   materialDesc - Material ������ �ʿ��� ��
        **/
        void CreateMaterial(HidedPhysicsMaterialDesc& materialDesc);

        /**
            @brief   Material �Ҵ� ����
            @details -
        **/
        void Release();
	};
}