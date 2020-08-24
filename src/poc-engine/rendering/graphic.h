#pragma once

#include <string>

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

		static Graphic* createGraphicApi(Api api);
		static std::string toString(Api api);

	protected:

		Graphic(Api api);

	private:

		const Api api;
		static bool created;

	};

}
