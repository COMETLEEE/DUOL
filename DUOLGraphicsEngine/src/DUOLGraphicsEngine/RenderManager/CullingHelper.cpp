#include "DUOLGraphicsEngine/RenderManager/CullingHelper.h"

#include "DUOLGraphicsEngine/GraphicsEngine/GraphicsEngine.h"
#include "DUOLGraphicsEngine/ResourceManager/ResourceManager.h"
#include "DUOLGraphicsEngine/Util/Hash/Hash.h"

bool DUOLGraphicsEngine::CullingHelper::ViewFrustumCulling(DUOLMath::Matrix& worldTM, DUOLMath::Vector3& extents,
                                                           const Frustum& frustum, DUOLMath::Vector3& outWorldTranslatedExtent, DUOLMath::Vector3& outWorldTranslatedCenterPos)
{
	//���� �࿡ ������ �ٿ�� �ڽ��� ���Ѵ�.
	DUOLMath::Vector4 localx{ extents.x, 0.f, 0.f, 0.f };
	DUOLMath::Vector4 localy{ 0.f, extents.y, 0.f, 0.f };
	DUOLMath::Vector4 localz{ 0.f, 0.f, extents.z, 0.f };

	DUOLMath::Vector4 scaledxV4 = DUOLMath::Vector4::Transform(localx, worldTM);
	DUOLMath::Vector4 scaledyV4 = DUOLMath::Vector4::Transform(localy, worldTM);
	DUOLMath::Vector4 scaledzV4 = DUOLMath::Vector4::Transform(localz, worldTM);

	DUOLMath::Vector3 scaledx{ scaledxV4 };
	DUOLMath::Vector3 scaledy{ scaledyV4 };
	DUOLMath::Vector3 scaledz{ scaledzV4 };

	float newx = fabs(scaledx.Dot(DUOLMath::Vector3::Right)) + fabs(scaledy.Dot(DUOLMath::Vector3::Right)) + fabs(scaledz.Dot(DUOLMath::Vector3::Right));
	float newy = fabs(scaledx.Dot(DUOLMath::Vector3::Up)) + fabs(scaledy.Dot(DUOLMath::Vector3::Up)) + fabs(scaledz.Dot(DUOLMath::Vector3::Up));
	float newz = fabs(scaledx.Dot(DUOLMath::Vector3::Forward)) + fabs(scaledy.Dot(DUOLMath::Vector3::Forward)) + fabs(scaledz.Dot(DUOLMath::Vector3::Forward));

	DUOLMath::Vector3 min = { -newx, -newy, -newz };
	DUOLMath::Vector3 max = { newx, newy, newz };

	DUOLMath::Vector3 centerPos{ worldTM.m[3][0], worldTM.m[3][1], worldTM.m[3][2] };

	outWorldTranslatedExtent = max;
	outWorldTranslatedCenterPos = centerPos;

	bool rb = IsForwardPlane(centerPos, max, frustum._rightFace);
	bool lb = IsForwardPlane(centerPos, max, frustum._leftFace);
	bool tb = IsForwardPlane(centerPos, max, frustum._topFace);
	bool bb = IsForwardPlane(centerPos, max, frustum._bottomFace);
	bool nb = IsForwardPlane(centerPos, max, frustum._nearFace);
	bool fb = IsForwardPlane(centerPos, max, frustum._farFace);

	return (rb && lb && tb && bb && nb && fb);

}

void DUOLGraphicsEngine::CullingHelper::CreateFrustumFromCamera(const Camera& camera, Frustum& outFrustum)
{
	auto camMat = camera._viewMatrix.Invert();

	outFrustum._camRight = DUOLMath::Vector3{ camMat.m[0][0], camMat.m[0][1], camMat.m[0][2] };
	outFrustum._camUp = DUOLMath::Vector3{ camMat.m[1][0], camMat.m[1][1], camMat.m[1][2] };
	outFrustum._camLook = DUOLMath::Vector3{ camMat.m[2][0], camMat.m[2][1], camMat.m[2][2] };

	outFrustum._camLook.Normalize();
	outFrustum._camRight.Normalize();
	outFrustum._camUp.Normalize();

	outFrustum._nearFace = { outFrustum._camLook , camera._cameraPosition + outFrustum._camLook * camera._cameraNear };
	outFrustum._farFace = { -outFrustum._camLook , camera._cameraPosition + outFrustum._camLook * camera._cameraFar };

	DUOLMath::Vector3 FarVec = outFrustum._camLook * camera._cameraFar;
	float halfVertical = camera._cameraFar * tanf(camera._cameraVerticalFOV * 0.5f);
	float halfHorizon = halfVertical * camera._aspectRatio;

	DUOLMath::Vector3 rightNormal = FarVec + outFrustum._camRight * halfHorizon;
	DUOLMath::Vector3 leftNormal = FarVec - outFrustum._camRight * halfHorizon;

	rightNormal.Normalize();
	leftNormal.Normalize();

	outFrustum._rightFace = { rightNormal.Cross(outFrustum._camUp) , camera._cameraPosition };
	outFrustum._leftFace = { outFrustum._camUp.Cross(leftNormal) , camera._cameraPosition };

	DUOLMath::Vector3 upNormal = FarVec + outFrustum._camUp * halfVertical;
	DUOLMath::Vector3 downNormal = FarVec - outFrustum._camUp * halfVertical;

	upNormal.Normalize();
	downNormal.Normalize();

	//outFrustum._topFace = {  upNormal.Cross(outFrustum._camRight) , camera._cameraPosition };
	//outFrustum._bottomFace = { outFrustum._camRight.Cross(downNormal) , camera._cameraPosition };

	outFrustum._topFace = { outFrustum._camRight.Cross(upNormal) , camera._cameraPosition };
	outFrustum._bottomFace = { downNormal.Cross(outFrustum._camRight) , camera._cameraPosition };

	outFrustum._nearFace._normal.Normalize();
	outFrustum._farFace._normal.Normalize();
	outFrustum._rightFace._normal.Normalize();
	outFrustum._leftFace._normal.Normalize();
	outFrustum._topFace._normal.Normalize();
	outFrustum._bottomFace._normal.Normalize();
}

bool DUOLGraphicsEngine::CullingHelper::IsForwardPlane(DUOLMath::Vector3& centerPos, DUOLMath::Vector3& boundingBox, const Plane& plane)
{
	//bounding box�� positive extent�� plane normal���� ����. 
	float r = boundingBox.x * fabs(plane._normal.x) + boundingBox.y * fabs(plane._normal.y) + boundingBox.z * fabs(plane._normal.z);

	//signed distance
	float distPlane = plane._normal.Dot(plane._position);
	float s = plane._normal.Dot(centerPos) - distPlane;

	return -r <= s;
}

DUOLGraphicsEngine::OcclusionCulling::OcclusionCulling(DUOLGraphicsEngine::GraphicsEngine* const graphicsEngine)
{
	auto resourceManger = graphicsEngine->GetResourceManager();

	CreateBuffers(resourceManger);

	//TEMP TODO:: �׽�Ʈ�� ���� ComputeShaderPipeline�����


	_culling = resourceManger->GetPipelineState(Hash::Hash64(_T("TestComputeShader")));
	_linearSampler = resourceManger->GetSampler(Hash::Hash64(_T("SamLinearClamp")));
}

void DUOLGraphicsEngine::OcclusionCulling::OnResize(DUOLGraphicsEngine::GraphicsEngine* const graphicsEngine)
{
	auto resourceManger = graphicsEngine->GetResourceManager();

	//���� depth�� mip level�� Ȯ���մϴ�.
	const auto mipLevel = _mipmapDepth->GetTextureDesc()._mipLevels;
	
	//mipSize�� 1x1�ɶ����� �����.
	for(unsigned int mipidx = 0; mipidx < mipLevel; ++mipidx)
	{
		std::wstring id = L"OcclusionCullingDepth";
		id += std::to_wstring(mipidx);

		DUOLGraphicsLibrary::RenderTargetDesc renderTargetDesc;

		renderTargetDesc._texture = _mipmapDepth;
		renderTargetDesc._mipLevel = mipidx;

		auto rendertarget = resourceManger->CreateRenderTarget(id, renderTargetDesc);

		_mipmapRenderTargets.emplace_back(rendertarget);
	}
}

void DUOLGraphicsEngine::OcclusionCulling::UnloadRenderTargets(DUOLGraphicsEngine::GraphicsEngine* const graphicsEngine)
{
	auto resourceManger = graphicsEngine->GetResourceManager();

	for (auto rtv : _mipmapRenderTargets)
	{
		resourceManger->DeleteRenderTarget(rtv->GetGUID());
	}
	_mipmapRenderTargets.clear();
}

DUOLGraphicsLibrary::Texture* DUOLGraphicsEngine::OcclusionCulling::GetMipmapDepth() const
{
	return _mipmapDepth;
}

void DUOLGraphicsEngine::OcclusionCulling::SetMipmapDepth(DUOLGraphicsLibrary::Texture* mipmap_depth)
{
	_mipmapDepth = mipmap_depth;
}

DUOLGraphicsLibrary::Texture* DUOLGraphicsEngine::OcclusionCulling::GetRenderDepth() const
{
	return _renderDepth;
}

void DUOLGraphicsEngine::OcclusionCulling::SetRenderDepth(DUOLGraphicsLibrary::Texture* render_depth)
{
	_renderDepth = render_depth;
}

std::vector<DUOLGraphicsLibrary::RenderTarget*>& DUOLGraphicsEngine::OcclusionCulling::GetMipmapRenderTargets()
{
	return _mipmapRenderTargets;
}

DUOLGraphicsLibrary::PipelineState* DUOLGraphicsEngine::OcclusionCulling::GetDownSampling() const
{
	return _downSampling;
}

DUOLGraphicsLibrary::PipelineState* DUOLGraphicsEngine::OcclusionCulling::GetOcclusionCullingPipeline() const
{
	return _culling;
}

void DUOLGraphicsEngine::OcclusionCulling::SetDownSampling(DUOLGraphicsLibrary::PipelineState* down_sampling)
{
	_downSampling = down_sampling;
}

void DUOLGraphicsEngine::OcclusionCulling::CreateBuffers(DUOLGraphicsEngine::ResourceManager* resourceManager)
{
	DUOLGraphicsLibrary::BufferDesc cpuBuffer;

	cpuBuffer._usage  = DUOLGraphicsLibrary::ResourceUsage::USAGE_STAGING;
	cpuBuffer._size   = 64 * 256;
	cpuBuffer._stride = 64;
	cpuBuffer._cpuAccessFlags = static_cast<long>(DUOLGraphicsLibrary::CPUAccessFlags::READ);
	cpuBuffer._miscFlags |= static_cast<long>(DUOLGraphicsLibrary::MiscFlags::RESOURCE_MISC_BUFFER_STRUCTURED);

	_cpuBuffer = resourceManager->CreateEmptyBuffer(_T("OcclusionCullingCpuReadBuffer"), cpuBuffer);

	DUOLGraphicsLibrary::BufferDesc resultBuffer;
	 
	resultBuffer._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
	resultBuffer._size = 64 * 256;
	resultBuffer._stride = 64;
	resultBuffer._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::UNORDEREDACCESS);
	resultBuffer._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_UNKNOWN;
	resultBuffer._miscFlags = static_cast<long>(DUOLGraphicsLibrary::MiscFlags::RESOURCE_MISC_COUNTER);
	resultBuffer._miscFlags |= static_cast<long>(DUOLGraphicsLibrary::MiscFlags::RESOURCE_MISC_BUFFER_STRUCTURED);

	_resultBuffer = resourceManager->CreateEmptyBuffer(_T("OcclusionCullingResultBuffer"), resultBuffer);

	resultBuffer._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
	resultBuffer._size = 32 * 256;
	resultBuffer._stride = 32;
	resultBuffer._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);
	resultBuffer._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_UNKNOWN;
	resultBuffer._miscFlags = static_cast<long>(DUOLGraphicsLibrary::MiscFlags::RESOURCE_MISC_BUFFER_STRUCTURED);

	_extentsBuffer = resourceManager->CreateEmptyBuffer(_T("OcclusionCullingExtentBuffer"), resultBuffer);
}

DUOLGraphicsLibrary::Sampler* DUOLGraphicsEngine::OcclusionCulling::GetLinearSampler() const
{
	return _linearSampler;
}

DUOLGraphicsLibrary::Buffer* DUOLGraphicsEngine::OcclusionCulling::GetCpuBuffer() const
{
	return _cpuBuffer;
}

DUOLGraphicsLibrary::Buffer* DUOLGraphicsEngine::OcclusionCulling::GetResultBuffer() const
{
	return _resultBuffer;
}

DUOLGraphicsLibrary::Buffer* DUOLGraphicsEngine::OcclusionCulling::GetExtentBuffer() const
{
	return _extentsBuffer;
}

