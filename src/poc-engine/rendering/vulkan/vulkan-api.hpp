#pragma once

#include "../../core/pimpl_ptr.hpp"
#include "../../plateform/window.hpp"
#include "../graphic-api.hpp"

namespace poc {

	class VulkanApi : public GraphicApi {
	public:

		VulkanApi(const Window& window);
		virtual void render() override;

	private:
		class Impl;
		pimpl_ptr<Impl> pimpl;
	};


}