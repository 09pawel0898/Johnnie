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
		static TSharedPtr<AStaticMesh> CreateBox(glm::vec3 Size);
		
		[[nodiscard]]
		static TUniquePtr<Mesh> CreateBoxMesh(glm::vec3 Size);

		[[nodiscard]]
		static TSharedPtr<AStaticMesh> CreatePlane(float Width, float Depth);

		[[nodiscard]]
		static TSharedPtr<AStaticMesh> CreateSphere(float Radius, uint32_t SliceCount, uint32_t StackCount);

		[[nodiscard]]
		static TUniquePtr<Mesh> CreateSphereMesh(float Radius, uint32_t SliceCount, uint32_t StackCount);
	};
}