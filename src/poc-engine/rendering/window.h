#pragma once

#include <functional>
#include <string>

namespace poc::layers {

	class Window {

	public:

		static std::unique_ptr<Window> openWindow(int width, int height, const std::string& title);

		virtual bool isClosing() = 0;
		virtual void update() = 0;
		virtual ~Window() = 0;

		template<class C, class H>
		void exposeToGraphicApi(const C& context, H* handler) const;

	protected:

		Window();

	private:
		
		static bool created;

	};

	

}