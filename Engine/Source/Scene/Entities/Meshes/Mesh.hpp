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
		std::shared_ptr<RHIVertexArray>	m_VAO;

		/** Material Info */
		std::weak_ptr<Material> m_HardMaterialReference;
		bool m_bUseHardMaterialReference{ false };

		uint8_t						m_MaterialIndex = Index::None;
		std::weak_ptr<AStaticMesh>	m_StaticMeshOwner;

		/** Mesh Info */
		MeshStatistics m_MeshStatistics;

	public:
		Mesh(std::vector<RHIVertex> const& Vertices, std::vector<uint32_t> const& Indices);

		/** Drawing */
		void Draw(glm::mat4 const& ModelMat) const;
		
		/** Stats */
		MeshStatistics& GetMeshStatistics(void);
		MeshStatistics const& GetMeshStatistics(void) const;

		/** Materials */

		/* Hard material reference - use when want to set custom material / mesh is not owned by StaticMesh*/
		void SetHardMaterialReference(std::weak_ptr<Material> Material);
		void SetUseHardMaterialReference(bool Use);
		
		/* Material can also be parsed from StaticMesh when its reference is valid, 
		then proper material with assigned index is being applied */
		void SetStaticMeshOwner(std::shared_ptr<AStaticMesh> const& Owner);
		uint8_t GetMaterialIndex(void) const;
		void SetMaterialIndex(uint8_t Index);
	
	private:
		void SetupMesh(std::vector<RHIVertex> const& Vertices, std::vector<uint32_t> const& Indicesbs);

		std::shared_ptr<Material> GetMaterialFromStaticMeshSlot(uint8_t Index) const;
	};

	FORCEINLINE void Mesh::SetStaticMeshOwner(std::shared_ptr<AStaticMesh> const& Owner)
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