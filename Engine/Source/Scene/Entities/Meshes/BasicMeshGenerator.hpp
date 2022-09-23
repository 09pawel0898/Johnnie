#pragma once

#include "StaticMesh.hpp"

namespace Engine
{
	class BasicMeshGenerator final
	{
	private:
		BasicMeshGenerator() = delete;

	public:
		[[nodiscard]]
		static std::shared_ptr<AStaticMesh> CreateBox(glm::vec3 Size);
		
		[[nodiscard]]
		static std::unique_ptr<Mesh> CreateBoxMesh(glm::vec3 Size);

		[[nodiscard]]
		static std::shared_ptr<AStaticMesh> CreatePlane(float Width, float Depth);

		[[nodiscard]]
		static std::shared_ptr<AStaticMesh> CreateSphere(float Radius, uint32_t SliceCount, uint32_t StackCount);

		[[nodiscard]]
		static std::unique_ptr<Mesh> CreateSphereMesh(float Radius, uint32_t SliceCount, uint32_t StackCount);
	};
}