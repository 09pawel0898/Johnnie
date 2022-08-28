#include "EnginePCH.hpp"

#include "BasicMeshGenerator.hpp"
#include "../CoreObject.hpp"

namespace Engine
{
	std::shared_ptr<OStaticMesh> BasicMeshGenerator::CreateBox(glm::vec3 Size)
	{
        float w = 0.5f * Size.x;
        float h = 0.5f * Size.y;
        float d = 0.5f * Size.z;

		static std::vector<RHIVertex> boxVertices =
		{
			/** Front Face */
			{glm::vec3(-w, -h, -d),	glm::vec3(0.0f, 0.0f, -1.0f),	glm::vec2(0.0f, 1.0f)},
			{glm::vec3(-w, +h, -d),	glm::vec3(0.0f, 0.0f, -1.0f),	glm::vec2(0.0f, 0.0f)},
			{glm::vec3(+w, +h, -d),	glm::vec3(0.0f, 0.0f, -1.0f),	glm::vec2(1.0f, 0.0f)},
			{glm::vec3(+w, -h, -d),	glm::vec3(0.0f, 0.0f, -1.0f),	glm::vec2(1.0f, 1.0f)},
			
			/** Back Face */
			{glm::vec3(-w, -h, +d),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(1.0f, 1.0f)},
			{glm::vec3(+w, -h, +d),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(0.0f, 1.0f)},
			{glm::vec3(+w, +h, +d),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(0.0f, 0.0f)},
			{glm::vec3(-w, +h, +d),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(1.0f, 0.0f)},
			
			/** Top Face */
			{glm::vec3(w, h, d),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f, 1.0f)},
			{glm::vec3(w, h, d),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f, 0.0f)},
			{glm::vec3(w, h, d),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(1.0f, 0.0f)},
			{glm::vec3(w, h, d),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(1.0f, 1.0f)},
			
			/** Bottom Face */
			{glm::vec3(w, h, -d),	glm::vec3(0.0f, -1.0f, 0.0f),	glm::vec2(1.0f, 1.0f)},
			{glm::vec3(w, h, -d),	glm::vec3(0.0f, -1.0f, 0.0f),	glm::vec2(0.0f, 1.0f)},
			{glm::vec3(w, h, +d),	glm::vec3(0.0f, -1.0f, 0.0f),	glm::vec2(0.0f, 0.0f)},
			{glm::vec3(w, h, +d),	glm::vec3(0.0f, -1.0f, 0.0f),	glm::vec2(1.0f, 0.0f)},
			
			/** Left Face */
			{glm::vec3(-w, -h, +d),	glm::vec3(-1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 1.0f)},
			{glm::vec3(-w, +h, +d),	glm::vec3(-1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 0.0f)},
			{glm::vec3(-w, +h, -d),	glm::vec3(-1.0f, 0.0f, 0.0f),	glm::vec2(1.0f, 0.0f)},
			{glm::vec3(-w, -h, -d),	glm::vec3(-1.0f, 0.0f, 0.0f),	glm::vec2(1.0f, 1.0f)},
			
			/** Right Face */
			{glm::vec3(+w, -h, -d),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 1.0f)},
			{glm::vec3(+w, +h, -d),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 0.0f)},
			{glm::vec3(+w, +h, +d),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(1.0f, 0.0f)},
			{glm::vec3(+w, -h, +d),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(1.0f, 1.0f)}
		};

		static std::vector<uint32_t> boxIndices =
		{
			/** Front Face */
			0, 1, 2,
			0, 2, 3,

			/** Back Face */
			4, 5, 6,
			4, 6, 7,

			/** Top Face */
			8, 9, 10,
			8, 10, 11,

			/** Bottom Face */
			12, 13, 14,
			12, 14, 15,

			/** Left Face */
			16, 17, 18,
			16, 18, 19,

			/** Right Face */
			20, 21, 22,
			20, 22, 23,
		};

		std::vector<OMesh> meshes = 
		{ 
			OMesh(boxVertices, boxIndices) 
		};
		return NewObject<OStaticMesh>(std::move(meshes));
	}

	std::shared_ptr<OStaticMesh> BasicMeshGenerator::CreatePlane(float Width, float Depth)
	{
		return std::unique_ptr<OStaticMesh>();
	}

	std::shared_ptr<OStaticMesh> BasicMeshGenerator::CreatSphere(float Radius, uint32_t SliceCount, uint32_t StackCount)
	{
		return std::unique_ptr<OStaticMesh>();
	}
}