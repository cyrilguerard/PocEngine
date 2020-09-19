#pragma once

#include <memory>
#include <vector>

#include "core/scene.hpp"

namespace poc {

	class PocEngine {
	public:

		virtual void loadScene(const Scene scene) = 0;
		virtual void run() = 0;
		virtual ~PocEngine() {};

		static std::unique_ptr<PocEngine> make();

	};

}
