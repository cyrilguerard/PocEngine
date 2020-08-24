#pragma once

#include <string>
#include "graphic.h"

namespace poc::layers {

	class Window {

	public:

		virtual bool isClosing() = 0;
		virtual void update() = 0;
		virtual ~Window() = 0;

		static Window* createWindow(int width, int height, std::string title, Graphic::Api api);

	protected:

		Window();

	private:
		
		static bool created;

	};

}