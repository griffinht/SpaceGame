#include "TransformConstantBuffer.h"

TransformConstantBuffer::TransformConstantBuffer(Graphics& graphics, const Drawable& parent)
	:
	parent(parent)
{
	if (!pVertexConstantBuffer)
	{
		pVertexConstantBuffer = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(graphics);
	}
}

void TransformConstantBuffer::Bind(Graphics& graphics)
{
	pVertexConstantBuffer->Update(graphics,
		DirectX::XMMatrixTranspose(
			parent.GetTransformXM() * graphics.GetProjection()
		)
	);
	pVertexConstantBuffer->Bind(graphics);
}

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformConstantBuffer::pVertexConstantBuffer;