#include "TransformConstantBuffer.h"

TransformConstantBuffer::TransformConstantBuffer(Graphics& graphics, const Drawable& parent)
	:
	vertexConstantBuffer(graphics),
	parent(parent)
{}

void TransformConstantBuffer::Bind(Graphics& graphics)
{
	vertexConstantBuffer.Update(graphics,
		DirectX::XMMatrixTranspose(
			parent.GetTransformXM() * graphics.GetProjection()
		)
	);
	vertexConstantBuffer.Bind(graphics);
}
