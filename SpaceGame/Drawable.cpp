#include "Drawable.h"
#include "GraphicsThrowMacros.h"
#include "IndexBuffer.h"
#include <cassert>
#include <typeinfo>

void Drawable::Draw(Graphics& graphics) const
{
	for (auto& b : binds)
	{
		b->Bind(graphics);
	}

	graphics.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind)
{
	binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer)
{
	pIndexBuffer = indexBuffer.get();
	binds.push_back(std::move(indexBuffer));
}