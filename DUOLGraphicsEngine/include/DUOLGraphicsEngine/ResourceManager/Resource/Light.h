#pragma once
#include "DUOLMath/DUOLMath.h"
#include "DUOLGraphicsEngine/Export.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"

namespace DUOLGraphicsEngine
{
	struct RenderObject;
	class LightManager;

	enum class LightType
	{
		Direction = 1u,
		Point = 2u,
		Spot = 3u,
		Area = 4u,
		Unknown = 0,
	};

	enum class LightState
	{
		Static = 1u, //static only
		Mixed = 2u, //mix + static
		Dynamic = 3u, //all dynamic
		Unknown = 0,
	};
	struct DUOLGRAPHICSENGINE_EXPORT LightData
	{
		LightData() :
			_lightType(LightType::Direction)
			, _lightState(LightState::Dynamic)
			, _direction(0.f, -1.f, 0.f)
			, _up(1.f, 0.f, 0.f)
			, _position(0.f, 10.f, 0.f)
			, _color(0.5f, 0.5f, 0.5f)
			, _intensity(10.f)
			, _angle(0.785398f)
			, _innerAngle(0.01f)
			, _attenuationRadius(10.f)
			, _fallOffExponential(0.1f)
			, _width(1.f)
			, _height(1.f)
			, _shadowStaticMapIdx(-1)
			, _shadowDynamicMapIdx(-1)
		{
		}

		LightType _lightType;
		DUOLMath::Vector3 _direction;

		DUOLMath::Vector3  _position;
		float _attenuationRadius;

		DUOLMath::Vector3  _color;
		float _intensity;

		float _angle;
		float _innerAngle;
		float _fallOffExponential;
		float _width;

		DUOLMath::Vector3 _up;
		float _height;

		int _shadowStaticMapIdx;
		int _shadowDynamicMapIdx;
		LightState _lightState;
		float _pad;

		DUOLMath::Matrix _shadowMatrix;
		//todo:: ���ϴ� ����Ʈ����Ʈ ť����� ���� ����Ѵ�...  ��� �����ؾ��ϴ°�?...
		DUOLMath::Matrix _shadowMatrix1;
		DUOLMath::Matrix _shadowMatrix2;
		DUOLMath::Matrix _shadowMatrix3;
		DUOLMath::Matrix _shadowMatrix4;
		DUOLMath::Matrix _shadowMatrix5;

		// todo:: ���߿� vertex fetchó�� ����Ʈ�����͸� �ؽ��Ŀ� ����־ read�ص� ������ �� ����.
	};


	class DUOLGRAPHICSENGINE_EXPORT Light
	{
	public:
		Light(LightManager* lightManager);

		~Light();

	public:
		LightType GetLightType();

		LightState GetLightState();

		float GetAttenuationRadius();

		float GetIntensity();

		float GetAngle();

		float GetInnerAngle();

		float GetFallOffExponential();

		float GetWidth();

		float GetHeight();

		const DUOLMath::Vector3& GetPosition();

		void SetLightType(LightType type);

		void SetLightState(LightState state);

		void SetDirection(const DUOLMath::Vector3& direction);

		void SetUp(const DUOLMath::Vector3& up);

		void SetPosition(const DUOLMath::Vector3& position);

		void SetAttenuationRadius(float attenuationRadius);

		void SetColor(const DUOLMath::Vector3& color);

		void SetIntensity(float intencity);

		void SetAngle(float angle);

		void SetInnerAngle(float innerAngle);

		void SetFallOffExponential(float FallOffExponential);

		void SetWidth(float width);

		void SetHeight(float height);

		void SetLightWorldMatrix();

		void SetStaticShadowMapIndex(int shadowMapIndex);

		void SetDynamicShadowMapIndex(int shadowMapIndex);

		int GetStaticShadowMapIndex();

		int GetDynamicShadowMapIndex();

		void CalculateShadowMatrix();

		void ResetLightBakeFlag();

		bool GetNeedBakeStaticShadowMap() const;

		void SetNeedBakeStaticShadowMap(bool needBakeStaticShadowMap);

		bool GetNeedBakeDynamicShadowMap() const;

		void SetNeedBakeDynamicShadowMap(bool needBakeDynamicShadowMap);

		LightData& GetLightData();

		void ClearRenderTarget(DUOLGraphicsLibrary::Renderer* renderer);

		void TryGetShadowMapSpace();

		//����� ������Ʈ�� ���ؼ��� ����Ʈ��  
		void AddDrawObject(RenderObject* renderObject);

		static void CalculateSpotProjection(LightData& lightData);

	private:
		LightData _lightData;

		bool _needBakeStaticShadowMap;

		bool _needBakeDynamicShadowMap;

		DUOLMath::Matrix _lightViewMatrix;

		LightManager* _lightManager;

		//��Ʈ������ �ø��� ������Ʈ�鸸�� �׸��ϴ�.
		std::vector<DUOLGraphicsEngine::RenderObject*> _drawRenderObjectInfo;

	};
}
