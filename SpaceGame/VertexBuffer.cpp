#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(Graphics& graphics, const Vertex* vertices[])
{
	/*
	Vertex vertices[] =
	{
		{ -1.0f,-1.0f,-1.0f },
		{ 1.0f,-1.0f,-1.0f },
		{ -1.0f,1.0f,-1.0f },
		{ 1.0f,1.0f,-1.0f },
		{ -1.0f,-1.0f,1.0f },
		{ 1.0f,-1.0f,1.0f },
		{ -1.0f,1.0f,1.0f },
		{ 1.0f,1.0f,1.0f },
	};
	*/

	
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	
	HRESULT hr;
	GRAPHICS_THROW_INFO(graphics.pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));
}

void VertexBuffer::Bind(Graphics& graphics)
{
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	graphics.pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
}