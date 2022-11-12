#pragma once

#include "Core/CoreMinimal.hpp"

#include "Renderer/RHI/RHITypes.hpp"
#include "Renderer/Materials/Material.hpp"
#include "Renderer/RendererStatistics.hpp"

#include "SkeletalMeshHelpers.hpp"

#include <variant>
#include <map>
#include <array>

namespace Engine
{
	namespace RHI
	{
		class RHIVertexArray;
	}
	using namespace RHI;

	class AStaticMesh;
	class ASkeletalMesh;

	class Mesh
	{
	protected:
		/** RHI Data */
		TSharedPtr<RHIVertexArray>	m_VAO;

		/** Material Info */
		TWeakPtr<Material>	m_HardMaterialReference;
		bool				m_bUseHardMaterialReference{ false };
		
		uint8_t				m_MaterialIndex = Index::None;

		std::variant<TWeakPtr<AStaticMesh>, TWeakPtr<ASkeletalMesh>>	m_OwnerActor;

		/** Mesh Info */
		MeshStatistics m_MeshStatistics;

		/** Model Evaluation */
		bool m_bIsMeshLazyEvaluated;
		bool m_bEvaluated{ false };
		std::vector<RHIVertex>	m_LazyVertices;
		std::vector<uint32_t>	m_LazyIndices;

	public:
		Mesh() = default;
		Mesh(std::vector<RHIVertex> const& Vertices, std::vector<uint32_t> const& Indices, bool LazyEvaluateMesh = false);
		Mesh(std::vector<RHIVertex>&& Vertices, std::vector<uint32_t>&& Indices, bool LazyEvaluateMesh = false);
		virtual ~Mesh();

		/** Drawing */
		void Draw(glm::mat4 const& ModelMat) const;
		
		/** Stats */
		MeshStatistics& GetMeshStatistics(void);
		MeshStatistics const& GetMeshStatistics(void) const;

		/** Materials */

		/* Hard material reference - use when want to set custom material / mesh is not owned by StaticMesh*/
		void SetHardMaterialReference(TWeakPtr<Material> Material);
		void SetUseHardMaterialReference(bool Use);
		
		/* Material can also be parsed from StaticMesh when its reference is valid, 
		then proper material with assigned index is being applied */
		void SetStaticMeshOwner(TSharedPtr<AStaticMesh> const& Owner);
		void SetStaticMeshOwner(TSharedPtr<ASkeletalMesh> const& Owner);
		uint8_t GetMaterialIndex(void) const;
		void SetMaterialIndex(uint8_t Index);
	
	public:
		bool IsMeshLazyEvaluated(void)
		{
			return m_bIsMeshLazyEvaluated;
		}
		
		bool IsManualEvaluationPerformed(void)
		{
			return m_bEvaluated;
		}

		virtual void EvaluateMesh(void);

	protected:
		void SetupMesh(std::vector<RHIVertex> const& Vertices, std::vector<uint32_t> const& Indicesbs);

		Material* GetMaterialFromStaticMeshSlot(uint8_t Index) const;
	};

	FORCEINLINE void Mesh::SetStaticMeshOwner(TSharedPtr<AStaticMesh> const& Owner)
	{
		m_OwnerActor = Owner;
	}

	FORCEINLINE void Mesh::SetStaticMeshOwner(TSharedPtr<ASkeletalMesh> const& Owner)
	{
		m_OwnerActor = Owner;
	}

	FORCEINLINE uint8_t Mesh::GetMaterialIndex(void) const
	{
		return m_MaterialIndex;
	}

	FORCEINLINE void Mesh::SetMaterialIndex(uint8_t Index)
	{
		m_MaterialIndex = Index;
	}

	FORCEINLINE void Mesh::SetUseHardMaterialReference(bool Use)
	{
		m_bUseHardMaterialReference = Use;
	}

	FORCEINLINE MeshStatistics& Mesh::GetMeshStatistics(void)
	{
		return m_MeshStatistics;
	}

	FORCEINLINE MeshStatistics const& Mesh::GetMeshStatistics(void) const
	{
		return m_MeshStatistics;
	}

	class SkinnedMesh : public Mesh
	{
	private:
		std::vector<VertexBoneInfluenceData> m_LazyBoneInfluenceData;

	public:
		SkinnedMesh(std::vector<RHIVertex>&& Vertices, std::vector<uint32_t>&& Indices, std::vector<VertexBoneInfluenceData>&& BoneData);

	private:
		void SetupMesh(std::vector<RHIVertex> const& Vertices, std::vector<uint32_t> const& Indices, std::vector<VertexBoneInfluenceData> const& BoneData);
	
	public:
		virtual void EvaluateMesh(void) override;
	};
}