#pragma once

#include <iostream>

#include <string>

namespace poc::layers::window {

	class Window {

	public:

		bool isClosing();
		void update();

		static Window createWindow(int width, int height, std::string title);
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(Window&&) = delete;

	private:

		void* handler = nullptr;
		Window(int width, int height, std::string title);

		static bool created;

	};

}