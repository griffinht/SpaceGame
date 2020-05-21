#pragma once

#include "Graphics.h"
#include <DirectXMath.h>

class Bindable;

class Drawable
{
	template<class T>
	friend class DrawableBase;
public:
	Drawable() = default;
	virtual DirectX::XMMATRIX GetTransformXM() const = 0;
	void Draw(Graphics& graphics) const;
	virtual void Update(float dt) = 0;
	void AddBind(std::unique_ptr<Bindable> bind);
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> indexBuffer);
	virtual ~Drawable() = default;
private:
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const = 0;
private:
	const class IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
};