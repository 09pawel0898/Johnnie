#include "EnginePCH.hpp"
#include "MaterialHelpers.hpp"

#include "Renderer/Materials/Material.hpp"
#include "Renderer/RHI/Resources/RHITexture.hpp"
#include "Log/Log.hpp"

#include <assimp/scene.h>

#include "AssetImporter.hpp"

namespace Engine
{
	void MaterialsContainer::InitializeMaterialSlots(uint8_t NumMaterials)
	{
		m_Materials.clear();
		m_MaterialProcessed.clear();

		Emplace_N_MaterialSlots(NumMaterials);
	}

	void MaterialsContainer::SetModelImporter(TSharedPtr<AssetImporter> const& ModelImporter)
	{
		m_ModelImporter = ModelImporter;
	}

	void MaterialsContainer::LoadMaterialFromModelForIndex(aiMaterial* Mat, uint32_t MaterialIdx)
	{
		TSharedPtr<AssetImporter> ModelImporter = m_ModelImporter.lock();
		if (!ModelImporter)
		{
			CheckMsg(false, "Failed to parse material data from model importer, importer was nullptr. Forgot to call SetModelImporter?");
			return;
		}

		if (!m_MaterialProcessed[MaterialIdx])
		{
			std::vector<TSharedPtr<RHITexture2D>> MaterialTextures;

			auto LoadTexturesByType = [this, Mat, &MaterialTextures, &ModelImporter](RHI::RHIMapTextureType Type)
			{
				std::vector<TSharedPtr<RHITexture2D>> Textures = LoadMaterialTextures(ModelImporter, Mat, Type);
				MaterialTextures.insert(MaterialTextures.end(), 
										std::make_move_iterator(Textures.begin()), 
										std::make_move_iterator(Textures.end()));
			};

			LoadTexturesByType(RHIMapTextureType::DiffuseMap);
			LoadTexturesByType(RHIMapTextureType::SpecularMap);
			LoadTexturesByType(RHIMapTextureType::NormalMap);

			if (MaterialTextures.size() > 0)
			{
				for (auto& Texture : MaterialTextures)
				{
					m_Materials[MaterialIdx]->SetTextureByType(Texture->GetType(), Texture);
				}
				m_MaterialProcessed[MaterialIdx] = true;
			}
		}
	}

	void MaterialsContainer::Emplace_N_MaterialSlots(uint8_t N)
	{
		m_Materials.reserve(N);

		for (uint8_t idx = 0; idx < N; idx++)
		{
			m_Materials.emplace_back(MakeShared<Material>());
		}

		m_MaterialProcessed.resize(N);
		m_NumMaterials = N;
	}

	std::vector<TSharedPtr<RHITexture2D>> MaterialsContainer::LoadMaterialTextures(TSharedPtr<AssetImporter> const& ModelImporter, aiMaterial* Material, RHIMapTextureType Type)
	{
		std::vector<TSharedPtr<RHITexture2D>> TexturesOfRequestedType;

		for (uint8_t i = 0; i < Material->GetTextureCount(RHITextureTypeToAssimpTextureType(Type)); i++)
		{
			aiString TextureFileName;
			Material->GetTexture(RHITextureTypeToAssimpTextureType(Type), i, &TextureFileName);

			std::string TexturePath = ModelImporter->GetRootDirectory() + "/";
			TexturePath.append(TextureFileName.C_Str());

			if (ModelImporter->HasEmbededTextures())
			{
				const aiScene* Scene = ModelImporter->GetScene();

				const aiTexture* AiTexture = Scene->GetEmbeddedTexture(TextureFileName.C_Str());

				TUniquePtr<RHITexture2D> Texture = RHITexture2D::Create(AiTexture->pcData,
													AiTexture->mWidth,
													AiTexture->mHeight,
													Type);

				TexturesOfRequestedType.emplace_back(MoveTemp(Texture));
			}
			else
			{
				TUniquePtr<RHITexture2D> Texture = RHITexture2D::Create(TexturePath, Type);
				TexturesOfRequestedType.emplace_back(MoveTemp(Texture));
			}
		}
		return TexturesOfRequestedType;
	}

	Material* MaterialsContainer::GetMaterialInSlot(uint8_t SlotIndex)
	{
		if (SlotIndex > m_Materials.size() - 1)
		{
			LOG(Materials, Warning, "Bad index. Tried to get material from slot of index {0}. Available slots for this static mesh : [0-{1}]",
				SlotIndex, m_Materials.size() - 1);
			return nullptr;
		}
		else
		{
			return m_Materials[SlotIndex].get();
		}
	}

	void MaterialsContainer::SetMaterialForSlot(uint8_t SlotIndex, TSharedPtr<Material> const& Material)
	{
		if (SlotIndex > m_Materials.size() - 1 || m_Materials.size() == 0)
		{
			LOG(Materials, Warning, "Bad index. Tried to set new material in slot of index {0}. Available slots for this static mesh : [0-{1}]",
				SlotIndex, m_Materials.size() - 1);
			return;
		}

		m_Materials[SlotIndex] = Material;
	}

	size_t MaterialsContainer::GetNumMaterials(void) const
	{
		return m_Materials.size();
	}
}