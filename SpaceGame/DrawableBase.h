#pragma once

#include "Drawable.h"
#include "IndexBuffer.h"

template<class T>
class DrawableBase : public Drawable
{
public:
	bool IsStaticInitialized() const
	{
		return !staticBinds.empty();
	}
	void AddStaticBind(std::unique_ptr<Bindable> bind)
	{
		staticBinds.push_back(std::move(bind));
	}
	void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> ibuf)
	{
		assert(pIndexBuffer == nullptr);
		pIndexBuffer = ibuf.get();
		staticBinds.push_back(std::move(ibuf));
	}
private:
	const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const override
	{
		return staticBinds;
	}
private:
	static std::vector<std::unique_ptr<Bindable>> staticBinds;
};

template<class T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticBinds;