#pragma once

#include "Core/CoreMinimal.hpp"

#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct aiNode;
struct aiNodeAnim;

namespace Engine
{
	class VertexBoneInfluenceData
	{
	private:
		static constexpr inline int8_t s_MaxBonesPerVertex = 4;

	public:
		uint16_t Index = 0;

		/** Contains IDs of bones that invluence this vertex */
		int32_t	BoneIDs[s_MaxBonesPerVertex] = { -1 };

		/** Contains influence weights for associated bone IDs in BoneIDs array */
		float	Weights[s_MaxBonesPerVertex] = { 0.f };

	public:
		VertexBoneInfluenceData() = default;

		void AddBoneData(uint32_t GlobalVertexID, uint32_t BoneID, float Weight);
	};

	struct BoneData
	{
		glm::mat4	OffsetMatrix;

		BoneData() = default;

		explicit BoneData(glm::mat4&& OffsetMatrix) noexcept;
	};

	struct NodeData
	{
		const aiNode*	Node = nullptr;
		bool			IsRequired = false;

		NodeData() = default;
		NodeData(const aiNode* Node)
			:	Node(Node)
		{}
	};

	struct SkinnedMeshData
	{
		/** Contains information about which bones influence each vertex. Size is equal to vertices count in loaded model */
		std::vector<VertexBoneInfluenceData>	VertexBoneInfluenceData;

		/** Contains global index of first vertex for each mesh in model */
		std::vector<uint32_t>					MeshBaseVertex;
	};

	struct SkeletonData
	{
		/** Maps imported by assimp bone names to indexes */
		std::map<std::string, uint32_t>			BoneNameIndexMap;

		std::vector<BoneData>					BonesData;

		std::map<std::string, NodeData>			RequiredNodes;
	};
}