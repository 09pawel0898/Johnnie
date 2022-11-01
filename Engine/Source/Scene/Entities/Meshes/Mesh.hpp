#pragma once

#include "Core/CoreMinimal.hpp"
#include "Renderer/RHI/RHITypes.hpp"
#include "Renderer/Materials/Material.hpp"
#include "Renderer/RendererStatistics.hpp"

namespace Engine
{
	namespace RHI
	{
		class RHIVertexArray;
	}
	using namespace RHI;

	class AStaticMesh;

	class Mesh
	{
	private:
		/** RHI Data */
		TSharedPtr<RHIVertexArray>	m_VAO;

		/** Material Info */
		TWeakPtr<Material> m_HardMaterialReference;
		bool m_bUseHardMaterialReference{ false };

		uint8_t						m_MaterialIndex = Index::None;
		TWeakPtr<AStaticMesh>	m_StaticMeshOwner;

		/** Mesh Info */
		MeshStatistics m_MeshStatistics;


		/** Model Evaluation */
		bool m_bIsMeshLazyEvaluated;
		bool m_bEvaluated{ false };
		std::vector<RHIVertex>	m_LazyVertices;
		std::vector<uint32_t>	m_LazyIndices;

	public:
		Mesh(std::vector<RHIVertex> const& Vertices, std::vector<uint32_t> const& Indices, bool LazyEvaluateMesh = false);
		Mesh(std::vector<RHIVertex>&& Vertices, std::vector<uint32_t>&& Indices, bool LazyEvaluateMesh = false);
		~Mesh();

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

		void EvaluateMesh(void);

	private:
		void SetupMesh(std::vector<RHIVertex> const& Vertices, std::vector<uint32_t> const& Indicesbs);

		Material* GetMaterialFromStaticMeshSlot(uint8_t Index) const;
	};

	FORCEINLINE void Mesh::SetStaticMeshOwner(TSharedPtr<AStaticMesh> const& Owner)
	{
		m_StaticMeshOwner = Owner;
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
}