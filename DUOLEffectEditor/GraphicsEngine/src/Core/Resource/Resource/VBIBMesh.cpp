#include "Core/Resource/Resource/VBIBMesh.h"
#include "util/TypeDefine.h"

namespace MuscleGrapics
{
	VBIBMesh::VBIBMesh() :
		_VB(nullptr), _IB(nullptr), _indexSize()
	{
	}
	VBIBMesh::~VBIBMesh()
	{
		ReleaseCOM(_VB);

		ReleaseCOM(_IB);
	}
	ID3D11Buffer** VBIBMesh::GetVB()
	{
		return &_VB;
	}
	ID3D11Buffer** VBIBMesh::GetIB()
	{
		return &_IB;
	}
	int VBIBMesh::GetIndexSize()
	{
		return _indexSize;
	}
}
