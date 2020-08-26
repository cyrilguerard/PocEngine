#pragma once

#include <string>

#include "window.h"

namespace poc::layers {

	class Graphic {

	public:

		enum class Api {
			//OPENGL,
			//DIRECTX,
			VULKAN
		};

		Api getApi() const;
		virtual ~Graphic();

		static std::unique_ptr<Graphic> createGraphicApi(const Window& window, Api api);
		static std::string toString(Api api);

	protected:

		Graphic(Api api);

	private:

		const Api api;
		static bool created;

	};

}
