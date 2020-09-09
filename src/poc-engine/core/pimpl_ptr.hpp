#pragma once

#include <memory>

/*
 * Utility to ease implemtation of the PIMPL idom based on:
 *
 * http://oliora.github.io/2015/12/29/pimpl-and-rule-of-zero.html
 *
 * Based on std::unique_ptr to guarantee that it will destroy itself automatically when it
 * goes out of scope. Limitation: only move semantics are permitted.

 */
namespace poc {

	namespace pimpl_ptr_deleter {

		template<class T>
		void deleter(T* p) noexcept {
			delete p;
		}

	}

	template<class T, class Deleter = void(*)(T*)>
	using pimpl_ptr = std::unique_ptr<T, Deleter>;

	template<class T, class... Args>
	inline pimpl_ptr<T> make_unique_pimpl(Args&&... args) {
		return pimpl_ptr<T>(new T(std::forward<Args>(args)...), &pimpl_ptr_deleter::deleter<T>);
	}

}