#pragma once

#include "../rendering/mesh.hpp"

namespace poc {

	class Scene {
	public:

		explicit Scene() :
			vertexCount(0),
			meshs(0) {}

		void addMesh(Mesh&& mesh) {
			const std::vector<Vertex> vertices = mesh.getVertices();
			vertexCount += static_cast<uint32_t>(vertices.size());
			meshs.emplace_back(mesh);
		}

		uint32_t getVertexCount() const {
			return vertexCount;
		};

		bool isEmpty() const {
			return vertexCount == 0;
		}

		std::vector<Vertex> getVertexes() const {
			std::vector<Vertex> vertices;
			vertices.reserve(vertexCount);
			for (const auto mesh : meshs) {
				const auto v = mesh.getVertices();
				std::copy(v.cbegin(), v.cend(), std::back_inserter(vertices));
			}
			return vertices;
		};

	private:
		uint32_t vertexCount;
		std::vector<Mesh> meshs;

	};

}
