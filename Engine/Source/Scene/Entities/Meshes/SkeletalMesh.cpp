#include "EnginePCH.hpp"
#include "SkeletalMesh.hpp"
#include "Log/Log.hpp"
#include "AssetImporter.hpp"
#include "Core/Debug/ProfileMacros.hpp"
#include "Scene/SceneDeleates.hpp"

namespace Engine
{
	ASkeletalMesh::ASkeletalMesh(std::string const& FilePath, OnSkeletalMeshAsyncLoadingFinishedDelegate OnLoadingFinished, glm::vec3 const& WorldLocation)
		:	Actor(WorldLocation),
			m_ModelFilePath(FilePath)
	{
		m_OnAsyncLoadingFinishedDelegate = MoveTemp(OnLoadingFinished);
	}

	ASkeletalMesh::~ASkeletalMesh()
	{}
	
	void ASkeletalMesh::Draw(void) const
	{
		if (m_bIsModelReadyToDraw)
		{
			const glm::mat4 modelMat = GetModelMat();

			for (auto& mesh : m_SubMeshes)
			{
				mesh->Draw(modelMat);
			}
		}
	}
	
	void ASkeletalMesh::OnTick(double DeltaTime)
	{
		bool IsModelImported = m_ModelImporter->IsModelAlreadyLoaded();

		if (IsModelImported && !m_bWasModelLoadedOnPrevFrame)
		{
			for (int8_t idx = 0; idx < m_SubMeshes.size(); idx++)
			{
				if (m_SubMeshes[idx]->IsMeshLazyEvaluated() && !m_SubMeshes[idx]->IsManualEvaluationPerformed())
				{
					{
						PROFILE_SCOPE("EvaluateMesh");
						m_SubMeshes[idx]->EvaluateMesh();
					}
					{
						PROFILE_SCOPE("ProcessMaterials");
						const aiScene* scene = m_ModelImporter->GetScene();
						ProcessMaterial(scene->mMaterials[m_SubMeshes[idx]->GetMaterialIndex()], m_SubMeshes[idx]->GetMaterialIndex());
					}
				}
			}
			m_OnAsyncLoadingFinishedDelegate.ExecuteIfBound(this);
			SceneDelegates::Get()->OnSkeletalMeshLoaded.Broadcast(this);

			m_bIsModelReadyToDraw = true;
		}

		m_bWasModelLoadedOnPrevFrame = IsModelImported;
	}
	

	void ASkeletalMesh::ProcessMaterial(aiMaterial* Material_, uint32_t MaterialIdx)
	{
		// If this material hasn't been processed yet //
		if (!m_MaterialProcessed[MaterialIdx])
		{
			std::vector<TSharedPtr<RHITexture2D>> materialTextures;

			std::vector<TSharedPtr<RHITexture2D>> diffuseMaps = LoadMaterialTextures(Material_, RHITextureType::Diffuse);
			std::vector<TSharedPtr<RHITexture2D>> specularMaps = LoadMaterialTextures(Material_, RHITextureType::Specular);
			std::vector<TSharedPtr<RHITexture2D>> normalMaps = LoadMaterialTextures(Material_, RHITextureType::Normal);

			materialTextures.insert(materialTextures.end(), std::make_move_iterator(diffuseMaps.begin()), std::make_move_iterator(diffuseMaps.end()));
			materialTextures.insert(materialTextures.end(), std::make_move_iterator(specularMaps.begin()), std::make_move_iterator(specularMaps.end()));
			materialTextures.insert(materialTextures.end(), std::make_move_iterator(normalMaps.begin()), std::make_move_iterator(normalMaps.end()));

			if (materialTextures.size() > 0)
			{
				for (auto& texture : materialTextures)
				{
					m_Materials[MaterialIdx]->SetTextureByType(texture->GetType(), texture);
				}
				m_MaterialProcessed[MaterialIdx] = true;
			}
		}
	}

	std::vector<TSharedPtr<RHITexture2D>> ASkeletalMesh::LoadMaterialTextures(aiMaterial* Material, RHITextureType Type)
	{
		std::vector<TSharedPtr<RHITexture2D>> textures;

		for (uint8_t i = 0; i < Material->GetTextureCount(RHITextureTypeToAssimpTextureType(Type)); i++)
		{
			aiString texFileName;
			Material->GetTexture(RHITextureTypeToAssimpTextureType(Type), i, &texFileName);

			std::string texturePath = m_Directory + "/";
			texturePath.append(texFileName.C_Str());

			if (m_ModelImporter->HasEmbededTextures())
			{
				const aiScene* Scene = m_ModelImporter->GetScene();

				const aiTexture* AiTexture = Scene->GetEmbeddedTexture(texFileName.C_Str());

				auto Texture = RHITexture2D::Create(AiTexture->pcData,
					AiTexture->mWidth,
					AiTexture->mHeight,
					Type);

				textures.emplace_back(MoveTemp(Texture));
			}
			else
			{
				auto Texture = RHITexture2D::Create(texturePath, Type);
				textures.emplace_back(MoveTemp(Texture));
			}
		}
		return textures;
	}

	void ASkeletalMesh::OnConstruct(void)
	{
		ImportModel();
	}

	void ASkeletalMesh::ImportModel()
	{
		m_ModelImporter = MakeUnique<SkeletalModelImporter>(shared_from_this());

		m_ModelImporter->AsyncImportModel(m_ModelFilePath);
	}

	void ASkeletalMesh::Emplace_N_MaterialSlots(uint8_t N)
	{
		m_Materials.reserve(N);
		m_MaterialProcessed.reserve(N);

		for (uint8_t idx = 0; idx < N; idx++)
		{
			m_Materials.emplace_back(MakeShared<Material>());
			m_MaterialProcessed.emplace_back(false);
		}
		m_NumMaterials = N;
	}
	Material* ASkeletalMesh::GetMaterialInSlot(uint8_t SlotIndex)
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

	void ASkeletalMesh::SetMaterialForSlot(uint8_t SlotIndex, TSharedPtr<Material> Material)
	{
		if (SlotIndex > m_Materials.size() - 1 || m_Materials.size() == 0)
		{
			LOG(Materials, Warning, "Bad index. Tried to set new material in slot of index {0}. Available slots for this static mesh : [0-{1}]",
				SlotIndex, m_Materials.size() - 1);
			return;
		}

		m_Materials[SlotIndex] = Material;
	}

	std::string const& ASkeletalMesh::GetFilePath(void) const
	{
		return m_ModelFilePath;
	}

	size_t ASkeletalMesh::GetNumMaterials(void) const
	{
		return m_Materials.size();
	}

	void ASkeletalMesh::InitializeMaterialSlots(uint8_t NumMaterials)
	{
		m_Materials.clear();
		m_MaterialProcessed.clear();

		Emplace_N_MaterialSlots(NumMaterials);
	}
}