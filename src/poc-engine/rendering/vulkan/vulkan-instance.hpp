#pragma once

#include "../../core/pimpl_ptr.hpp"
#include "../../plateform/platform.hpp"

namespace poc {

	class VulkanInstance {
	public:

		VulkanInstance();
		const vk::Instance& getInstance() const;

	private:
		class Impl;
		pimpl_ptr<Impl> pimpl;
	};


}