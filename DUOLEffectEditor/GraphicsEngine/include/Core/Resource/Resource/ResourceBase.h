/**

    @file      ResourceBase.h
    @brief     리소스를 일괄적으로 관리하기 위하여 만드는 부모 클래스
    @details   ~
    @author    SinSeongHyeon
    @date      9.02.2023
    @copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once
namespace MuscleGrapics
{
	class ResourceBase abstract
	{
	public:
		ResourceBase() = default;

		virtual ~ResourceBase() = default;
	};
}