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
		std::string Name;
		glm::mat4 Transformation;

		std::vector<NodeData> Children;
		uint8_t	ChildrenCount;

		NodeData* Parent{ nullptr };

		bool IsRequired = false;
	};

	struct SkinnedMeshData
	{
		/** Contains information about which bones influence each vertex. Size is equal to vertices count in loaded model */
		std::vector<VertexBoneInfluenceData>	VertexBoneInfluenceData;

		/** Contains global index of first vertex for each mesh in model */
		std::vector<uint32_t>					MeshBaseVertex;
	};

	class Skeleton
	{
	private:
		/** Maps imported by assimp bone names to indexes */
		std::map<std::string, uint32_t>	m_BoneNameIndexMap;
		std::vector<BoneData>			m_BonesData;
		NodeData						m_RootNode;
		glm::mat4						m_GlobalInverseTransform;

	public:
		Skeleton() = default;

	public:
		void SetGlobalInverseTransform(glm::mat4 const& GlobalInverseTransform);
		glm::mat4 const& GetGlobalInverseTransform(void) const;
		
		std::map<std::string, uint32_t> const& GetBoneNameIndexMap(void) const;
		std::map<std::string, uint32_t>& GetBoneNameIndexMap(void);

		std::vector<BoneData> const& GetBonesData(void) const;
		std::vector<BoneData>& GetBonesData(void);

		NodeData const& GetRootNode(void) const;
		NodeData& GetRootNode(void);

	public:
		NodeData* FindNodeByName(std::string const& Name);
		const NodeData* FindNodeByName(std::string const& Name) const;

		void GetRootBone(NodeData** OutResult) const;
		
	private:	
		void GetRootBone_Internal(const NodeData* Node, NodeData** OutResult) const;
		NodeData* FindNodeByName_Internal(std::string const& Name, NodeData* Node) const;
	};

	FORCEINLINE void Skeleton::SetGlobalInverseTransform(glm::mat4 const& GlobalInverseTransform)
	{
		m_GlobalInverseTransform = GlobalInverseTransform;
	}

	FORCEINLINE glm::mat4 const& Skeleton::GetGlobalInverseTransform(void) const
	{
		return m_GlobalInverseTransform;
	}
	
	FORCEINLINE std::map<std::string, uint32_t> const& Skeleton::GetBoneNameIndexMap(void) const
	{
		return m_BoneNameIndexMap;
	}

	FORCEINLINE std::map<std::string, uint32_t>& Skeleton::GetBoneNameIndexMap(void)
	{
		return m_BoneNameIndexMap;
	}
	
	FORCEINLINE std::vector<BoneData> const& Skeleton::GetBonesData(void) const
	{
		return m_BonesData;
	}

	FORCEINLINE std::vector<BoneData>& Skeleton::GetBonesData(void)
	{
		return m_BonesData;
	}

	FORCEINLINE NodeData const& Skeleton::GetRootNode(void) const
	{
		return m_RootNode;
	}

	FORCEINLINE NodeData& Skeleton::GetRootNode(void)
	{
		return m_RootNode;
	}
}