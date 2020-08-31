#pragma once

#include <memory>
#include <string>

namespace poc {

	class PocEngine {
	public:

		virtual void run() = 0;
		virtual ~PocEngine() {};

		static std::unique_ptr<PocEngine> make();

	};

}
