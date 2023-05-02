#pragma once
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderConstantBuffer.h"
#include "DUOLGraphicsLibrary/Renderer/RenderTarget.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLGraphicsLibrary
{
	class Sampler;
	class Buffer;
	class Texture;
	class PipelineState;
}

namespace DUOLGraphicsEngine
{
	class ResourceManager;
	class GraphicsEngine;

	struct Plane
	{
		Plane() :
			_normal(0.f, 1.f, 0.f)
			, _position(0.f)
		{

		}

		Plane(DUOLMath::Vector3 normal, DUOLMath::Vector3 pos) :
			_normal(normal)
			, _position(pos)
		{

		}

		DUOLMath::Vector3 _normal;

		DUOLMath::Vector3 _position;
	};

	struct Frustum
	{
		Frustum() :
			_topFace()
			, _bottomFace()
			, _rightFace()
			, _leftFace()
			, _farFace()
			, _nearFace()
		{
		}

		Plane _topFace;

		Plane _bottomFace;

		Plane _rightFace;

		Plane _leftFace;

		Plane _farFace;

		Plane _nearFace;

		DUOLMath::Vector3 _camLook;

		DUOLMath::Vector3 _camUp;

		DUOLMath::Vector3 _camRight;
	};


	class CullingHelper
	{
	public:
		static bool ViewFrustumCullingBoundingBox(DUOLMath::Matrix& worldTM, DUOLMath::Vector3& extents, DUOLMath::Vector3& center, const Frustum& frustum, DUOLMath::Vector3&
		                               outWorldTranslatedExtent, DUOLMath::Vector3& outWorldTranslatedCenterPos);

		static bool ViewFrustumCullingBoundingVolume(const DUOLMath::Vector3& worldPos, float radius, const Frustum& camera);

		static void CreateFrustumFromCamera(const Camera& camera, Frustum& outFrustum);

	private:
		static bool IsForwardPlane(DUOLMath::Vector3& centerPos, DUOLMath::Vector3& boundingBox, const Plane& plane);
		static bool IsForwardPlaneBoundingVolume(const DUOLMath::Vector3& centerPos, float radius, const Plane& plane);

	};


	/**

		@class   OcclusionCulling
		@brief   ��Ŭ���� �ø��� ���� ���ҽ��� �����ϴ� ���Դϴ�.
		@details ~
		@author  KyungMin Oh

	**/
	class OcclusionCulling
	{
	public:
		OcclusionCulling(DUOLGraphicsEngine::GraphicsEngine* const graphicsEngine);

		void OnResize(DUOLGraphicsEngine::GraphicsEngine* const graphicsEngine);

		void UnloadRenderTargets(DUOLGraphicsEngine::GraphicsEngine* const graphicsEngine);

		void ClearRenderTarget();

		DUOLGraphicsLibrary::Texture* GetMipmapDepth() const;

		DUOLGraphicsLibrary::Texture* GetRenderDepth() const;

		std::vector<DUOLGraphicsLibrary::RenderTarget*>& GetMipmapRenderTargets();

		DUOLGraphicsLibrary::PipelineState* GetDownSampling() const;

		DUOLGraphicsLibrary::PipelineState* GetOcclusionCullingPipeline() const;

		DUOLGraphicsLibrary::Buffer* GetCpuBuffer() const;

		DUOLGraphicsLibrary::Buffer* GetResultBuffer() const;

		DUOLGraphicsLibrary::Buffer* GetExtentBuffer() const;

		DUOLGraphicsLibrary::Sampler* GetTriPointerSampler() const;

		void SetMipmapDepth(DUOLGraphicsLibrary::Texture* mipmap_depth);

		void SetRenderDepth(DUOLGraphicsLibrary::Texture* render_depth);

		void SetDownSampling(DUOLGraphicsLibrary::PipelineState* down_sampling);

		DUOLGraphicsLibrary::PipelineState* GetCopyTexture() const;

		void CopyTexture(DUOLGraphicsLibrary::PipelineState* copyTexture);

	private:
		void CreateBuffers(DUOLGraphicsEngine::ResourceManager* resourceManager);

	private:
		DUOLGraphicsLibrary::Texture* _mipmapDepth;

		DUOLGraphicsLibrary::Texture* _renderDepth;

		DUOLGraphicsLibrary::Buffer* _extentsBuffer;

		DUOLGraphicsLibrary::Buffer* _cpuBuffer;

		DUOLGraphicsLibrary::Buffer* _resultBuffer;

		DUOLGraphicsLibrary::Sampler* _TriPointSampler;

		//����� ���� �Ӹ��� �޶����Ƿ� ����Ÿ�� ���� ���� �ٲپ��־���Ѵ�.
		//resize�ÿ��� ��� ����Ÿ���� �����ٰ� �ٽ� �����ϴ� ������ �Ѵ�. ��, ����������� ����� ũ��.
		std::vector<DUOLGraphicsLibrary::RenderTarget*> _mipmapRenderTargets;

		DUOLGraphicsLibrary::PipelineState* _downSampling;

		DUOLGraphicsLibrary::PipelineState* _culling;

		DUOLGraphicsLibrary::PipelineState* _copyTexture;
	};

}
