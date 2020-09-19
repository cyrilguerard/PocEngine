#pragma once

#include <vector>

#include "../core/logger.hpp"
#include "vertex.hpp"

namespace poc {

	class Mesh {
	public:

		Mesh() = default;
		Mesh(std::vector<Vertex>&& v) : vertices(std::move(v)) {}

		const std::vector<Vertex>& getVertices() const {
			return vertices;
		}

	private:
		std::vector<Vertex> vertices;
	};

}