#include "pch.h"
#include "Display.h"

Display::Display()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}


Display::Display(const Display& other)
{
}


Display::~Display()
{
	ReleaseCOM(m_InputLayout);
	ReleaseCOM(m_vertexBuffer);
	ReleaseCOM(m_indexBuffer);
}


bool Display::Initialize(int screenWidth, int screenHeight, int bitmapWidth, int bitmapHeight)
{
	bool result;
	ID3D11Device* device = DXEngine::GetInstance()->GetD3dDevice();
	HRESULT hr;
	ReleaseCOM(m_InputLayout);
	ReleaseCOM(m_vertexBuffer);
	ReleaseCOM(m_indexBuffer);
	// 화면 사이즈 넘겨주기
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// 어느 사이즈로 출력할지.
	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	// 음수로 초기화
	m_previousPosX = -1;
	m_previousPosY = -1;



	// 텍스쳐를 위한 인풋 레이아웃을 만들자~
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV",    0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	// Create the input layout
	D3DX11_PASS_DESC passDesc;
	Effects::TextureRenderFX->m_TextureRenderTech->GetPassByIndex(0)->GetDesc(&passDesc);

	hr = DXEngine::GetInstance()->GetD3dDevice()->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_InputLayout);


	// 버텍스와 인덱스 버퍼 초기화
	result = InitializeBuffers(device);
	if (!result || !hr)
	{
		return false;
	}




	return true;
}


void Display::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}


bool Display::Render(int positionX, int positionY)
{

	ID3D11DeviceContext* deviceContext = DXEngine::GetInstance()->Getd3dImmediateContext();
	bool result;


	// Re-build the dynamic vertex buffer for rendering to possibly a different location on the screen.
	result = UpdateBuffers(deviceContext, positionX, positionY);
	if (!result)
	{
		return false;
	}

	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return true;
}


int Display::GetIndexCount()
{
	return m_indexCount;
}


bool Display::InitializeBuffers(ID3D11Device* device)
{
	Vertex* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// 버텍스 카운트
	m_vertexCount = 6;

	// 인덱스 카운트
	m_indexCount = m_vertexCount;

	// 버텍스 배열 만들기
	vertices = new Vertex[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// 인덱스 배열 만들기
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(Vertex) * m_vertexCount));

	// Load the index array with data.
	for (i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void Display::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


bool Display::UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	float left, right, top, bottom;
	Vertex* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Vertex* verticesPtr;
	HRESULT result;


	// 안바뀌면 업데이트 x
	// currently has the correct parameters.
	if ((positionX == m_previousPosX) && (positionY == m_previousPosY))
	{
		return true;
	}

	// 변경시 업데이트
	m_previousPosX = positionX;
	m_previousPosY = positionY;

	left = -1.0f + (float)positionX*2 / m_screenWidth;
	right = left + (float)m_bitmapWidth * 2 / m_screenWidth;
	top = 1.0f - (float)positionY * 2 / m_screenHeight;
	bottom = top - (float)m_bitmapHeight * 2 / m_screenHeight;


	// Create the vertex array.
	vertices = new Vertex[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Load the vertex array with data.
	// First triangle.
	vertices[0].Pos = XMFLOAT3(left, top, 0.0f);  // Top left.
	//vertices[0].Pos = XMFLOAT3(0,0, 0.0f);  // Top left.
	vertices[0].Texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].Pos = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].Texture = XMFLOAT2(1.0f, 1.0f);

	vertices[2].Pos = XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].Texture = XMFLOAT2(0.0f, 1.0f);

	// Second triangle.
	vertices[3].Pos = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[3].Texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].Pos = XMFLOAT3(right, top, 0.0f);  // Top right.
	vertices[4].Texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].Pos = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].Texture = XMFLOAT2(1.0f, 1.0f);

	// Lock the vertex buffer so it can be written to.
	// todo
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (Vertex*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(Vertex) * m_vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(m_vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	return true;
}


void Display::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(Vertex);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}
