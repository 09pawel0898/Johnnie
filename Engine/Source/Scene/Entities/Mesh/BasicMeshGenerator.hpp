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
		static std::shared_ptr<OStaticMesh> CreateBox(glm::vec3 Size);

		[[nodiscard]]
		static std::shared_ptr<OStaticMesh> CreatePlane(float Width, float Depth);

		[[nodiscard]]
		static std::shared_ptr<OStaticMesh> CreatSphere(float Radius, uint32_t SliceCount, uint32_t StackCount);
	};
}