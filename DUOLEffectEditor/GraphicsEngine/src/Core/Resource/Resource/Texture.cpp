#include "Core/Resource/Resource/Texture.h"
#include "util/TypeDefine.h"

MuscleGrapics::ShaderResourceView::ShaderResourceView(ID3D11ShaderResourceView* srv)
	: _srv(srv)
{
}

MuscleGrapics::ShaderResourceView::~ShaderResourceView()
{
	ReleaseCOM(_srv);
}

ID3D11ShaderResourceView* MuscleGrapics::ShaderResourceView::Get()
{
	return _srv;
}
