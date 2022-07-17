#pragma once

#include <vector>
#include <memory>

using Gadget = int;

class Widget
{
	public:
		Widget();
		~Widget();
		Widget(Widget&& rhs);
		Widget& operator=(Widget&& rhs);
		Widget(const Widget& rhs);
		Widget& operator=(const Widget& rhs);

	private:
		struct Impl;
		std::unique_ptr<Impl> pImpl;
};
