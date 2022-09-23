#include "EnginePCH.hpp"

#include "BasicMeshGenerator.hpp"
#include "../CoreActor.hpp"

namespace Engine
{
	std::shared_ptr<AStaticMesh> BasicMeshGenerator::CreateBox(glm::vec3 Size)
	{
		static std::vector<std::shared_ptr<Mesh>> meshes =
		{ 
			CreateBoxMesh(Size)
		};
		return NewActor<AStaticMesh>(std::move(meshes));
	}

	std::unique_ptr<Mesh> BasicMeshGenerator::CreateBoxMesh(glm::vec3 Size)
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
			{glm::vec3(w, h, -d),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f, 1.0f)},
			{glm::vec3(w, h, d),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f, 0.0f)},
			{glm::vec3(+w, h, d),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(1.0f, 0.0f)},
			{glm::vec3(+w, h, -d),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(1.0f, 1.0f)},

			/** Bottom Face */
			{glm::vec3(-w, -h, -d),	glm::vec3(0.0f, -1.0f, 0.0f),	glm::vec2(1.0f, 1.0f)},
			{glm::vec3(w, -h, -d),	glm::vec3(0.0f, -1.0f, 0.0f),	glm::vec2(0.0f, 1.0f)},
			{glm::vec3(w, -h, +d),	glm::vec3(0.0f, -1.0f, 0.0f),	glm::vec2(0.0f, 0.0f)},
			{glm::vec3(-w, -h, +d),	glm::vec3(0.0f, -1.0f, 0.0f),	glm::vec2(1.0f, 0.0f)},

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

			/* Back Face */
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

		return std::make_unique<Mesh>(boxVertices, boxIndices);
	}

	std::shared_ptr<AStaticMesh> BasicMeshGenerator::CreatePlane(float Width, float Depth)
	{
		return std::unique_ptr<AStaticMesh>();
	}

	std::shared_ptr<AStaticMesh> BasicMeshGenerator::CreateSphere(float Radius, uint32_t SliceCount, uint32_t StackCount)
	{
		std::vector<std::shared_ptr<Mesh>> meshes =
		{
			CreateSphereMesh(Radius, SliceCount, StackCount)
		};
		return NewActor<AStaticMesh>(std::move(meshes));
	}

	std::unique_ptr<Mesh> BasicMeshGenerator::CreateSphereMesh(float Radius, uint32_t SliceCount, uint32_t StackCount)
	{
		std::vector<RHIVertex> sphereVertices;
		std::vector<uint32_t> sphereIndices;

		RHIVertex topVertex = { { 0.0f, +Radius, 0.0f }, { 0.0f, +1.0f, 0.0f } };
		sphereVertices.push_back(topVertex);


		double phiStep = std::numbers::pi / StackCount;
		double thetaStep = 2.0f * std::numbers::pi / SliceCount;

		for (uint32_t i = 1; i <= StackCount - 1; ++i)
		{
			double phi = i * phiStep;
			for (uint32_t j = 0; j <= SliceCount; ++j)
			{
				double theta = j * thetaStep;

				RHIVertex v;

				// spherical to cartesian
				v.Position.x = (float)(Radius * sin(phi) * cos(theta));
				v.Position.y = (float)(Radius * cos(phi));
				v.Position.z = (float)(Radius * sin(phi) * sin(theta));

				v.Normal = glm::normalize(v.Position);

				v.TexUV.x = (float)(theta / std::numbers::pi);
				v.TexUV.y = (float)(phi / std::numbers::pi);

				sphereVertices.push_back(v);
			}
		}
		RHIVertex bottomVertex = { { 0.0f, -Radius, 0.0f,  }, { 0.0f, -1.0f, 0.0f } };

		sphereVertices.push_back(bottomVertex);

		for (uint32_t i = 1; i <= SliceCount; ++i)
		{
			sphereIndices.push_back(0);
			sphereIndices.push_back(i + 1);
			sphereIndices.push_back(i);
		}

		uint32_t baseIndex = 1;
		uint32_t ringVertexCount = SliceCount + 1;
		for (uint32_t i = 0; i < StackCount - 2; ++i)
		{
			for (uint32_t j = 0; j < SliceCount; ++j)
			{
				sphereIndices.push_back(baseIndex + i * ringVertexCount + j);
				sphereIndices.push_back(baseIndex + i * ringVertexCount + j + 1);
				sphereIndices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

				sphereIndices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				sphereIndices.push_back(baseIndex + i * ringVertexCount + j + 1);
				sphereIndices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
			}
		}

		uint32_t southPoleIndex = (uint32_t)sphereVertices.size() - 1;
		baseIndex = southPoleIndex - ringVertexCount;

		for (uint32_t i = 0; i < SliceCount; ++i)
		{
			sphereIndices.push_back(southPoleIndex);
			sphereIndices.push_back(baseIndex + i);
			sphereIndices.push_back(baseIndex + i + 1);
		}

		return std::make_unique<Mesh>(sphereVertices, sphereIndices);
	}
}