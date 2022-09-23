#pragma once

#include "Core/CoreMinimal.hpp"
#include "Renderer/RHI/RHITypes.hpp"
#include "Renderer/Materials/Material.hpp"

namespace Engine
{
	struct TextureData 
	{
		uint32_t			ID;
		std::string_view	Type;
	};

	namespace RHI
	{
		class RHIVertexArray;
		class RHITexture2D;
		class RHIShader;
	}
	using namespace RHI;

	class AStaticMesh;

	class Mesh
	{
	private:
		std::vector<RHIVertex>		m_Vertices;
		std::vector<uint32_t>		m_Indices;
		
		std::shared_ptr<RHIVertexArray>	m_VAO;

	private:
		uint8_t						m_MaterialIndex = Index::None;
		std::weak_ptr<AStaticMesh>	m_Owner;

		void SetupMesh(void);
	public:
		Mesh(std::vector<RHIVertex> const& Vertices, std::vector<uint32_t> const& Indices);
		Mesh(std::vector<RHIVertex>&& Vertices, std::vector<uint32_t>&& Indices);
	
	public:
		void Draw(glm::mat4 const& ModelMat) const;
		
		void SetOwner(std::shared_ptr<AStaticMesh> const& Owner);

		uint8_t GetMaterialIndex(void) const;
		void SetMaterialIndex(uint8_t Index);
	
	private:
		std::shared_ptr<Material> GetMaterialFromStaticMeshSlot(uint8_t Index) const;

		std::weak_ptr<Material> m_HardMaterialReference;
		bool m_bUseHardMaterialReference{ false };

	public:
		void SetHardMaterialReference(std::weak_ptr<Material> Material);
	};

	FORCEINLINE void Mesh::SetOwner(std::shared_ptr<AStaticMesh> const& Owner)
	{
		m_Owner = Owner;
	}

	FORCEINLINE uint8_t Mesh::GetMaterialIndex(void) const
	{
		return m_MaterialIndex;
	}

	FORCEINLINE void Mesh::SetMaterialIndex(uint8_t Index)
	{
		m_MaterialIndex = Index;
	}
}