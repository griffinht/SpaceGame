#include "VertexBuffer.h"

void VertexBuffer::Bind(Graphics& graphics)
{
	const UINT offset = 0u;
	GetContext(graphics)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
}