/**

	@file      OITParticlePass.h
	@brief     반투명을 표현하기 위해 Order Independent Transparency 를 적용 시킨 패스.
	@details   ~ 한 오브젝트를 여러번 렌더링 해야한다... 기존의 구조와는 다르지만, 이런 예외는 더 발생 하지 않을 거라 생각하기에 구조는 이대로 유지한다.
	@author    SinSeongHyeon
	@date      22.12.2022
	@copyright © SinSeongHyeon, 2022. All right reserved.

**/
#pragma once

#include "PassBase.h"
namespace MuscleGrapics
{

	class OITParticlePass : public PassBase<RenderingData_Particle>
	{
	public:
		OITParticlePass();

		virtual ~OITParticlePass() override = default;
	private:
		void CompileAllFlags(std::vector<D3D_SHADER_MACRO>& macros, int index = 0, int flag_sum = 0);

	protected:
		virtual void SetConstants(RenderingData_Particle& renderingData) override;

	public:
		virtual void Draw(RenderingData_Particle& renderingData) override;



	};

}

