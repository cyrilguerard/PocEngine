#pragma once

#include <memory>
#include <string>

namespace poc {

	class Window {
	public:

		virtual bool isClosing() = 0;
		virtual void update() = 0;
		virtual ~Window() {};

		static std::unique_ptr<Window> openWindow(int, int, const std::string&);

		template<class C, class H>
		static void toGraphicApi(const Window& window, const C& context, H* handler);

	};

}