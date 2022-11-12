#pragma once

#include "Core/CoreMinimal.hpp"
#include "Renderer/RHI/RHITypes.hpp"

struct aiMaterial;
struct aiScene;

namespace Engine
{
	class Material;
	class AssetImporter;

	namespace RHI
	{
		class RHITexture2D;
	}
	
	using namespace RHI;

	class MaterialsContainer final
	{
	private:
		uint8_t m_NumMaterials = 0;

		std::vector<TSharedPtr<Material>>	m_Materials;
		std::vector<bool>					m_MaterialProcessed;

		TWeakPtr<AssetImporter>				m_ModelImporter;

	public:
		MaterialsContainer() = default;

	public:
		void SetModelImporter(TSharedPtr<AssetImporter> const& ModelImporter);

		void InitializeMaterialSlots(uint8_t NumMaterials);

		/*
		*	Parses material from imported aiMaterial, this is long and synchronous operation and must be performed from OpenGL rendering thread. 
		*/
		void LoadMaterialFromModelForIndex(aiMaterial* Mat, uint32_t MaterialIdx);

		Material* GetMaterialInSlot(uint8_t SlotIndex);

		size_t GetNumMaterials(void) const;

		void SetMaterialForSlot(uint8_t SlotIndex, TSharedPtr<Material> const& Material);

	private:
		void Emplace_N_MaterialSlots(uint8_t N);

		std::vector<TSharedPtr<RHITexture2D>> LoadMaterialTextures(TSharedPtr<AssetImporter> const& ModelImporter, aiMaterial* Material, RHIMapTextureType Type);
	};
}