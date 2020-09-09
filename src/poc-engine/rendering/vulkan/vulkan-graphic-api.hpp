#pragma once

#include "../../core/pimpl_ptr.hpp"
#include "../../plateform/window.hpp"
#include "../graphic-api.hpp"

namespace poc {

	class VulkanGraphicApi : public GraphicApi {
	public:

		explicit VulkanGraphicApi(const Window& window);
		virtual void render() override;

	private:
		class Impl;
		pimpl_ptr<Impl> pimpl;
	};


}