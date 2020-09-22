#pragma once

#include <memory>
#include <string>

namespace poc {

	class Window {
	public:

		struct Size {
			uint32_t width;
			uint32_t height;
		};

		virtual Size getDrawableSurfaceSize() const = 0;
		virtual bool isClosing() const = 0;
		virtual void update() = 0;
		virtual void waitWhileMinimized() const = 0;
		virtual ~Window() {};

		static std::unique_ptr<Window> openWindow(int, int, const std::string&);

		template<class C, class H>
		static void toGraphicApi(const Window& window, const C& context, H* handler);

	};

}