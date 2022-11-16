#include "EnginePCH.hpp"

#include "SkeletalMesh.hpp"
#include "Log/Log.hpp"
#include "../AssetImporter.hpp"
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

			int id = 0;

			for (auto& mesh : m_SubMeshes)
			{
				//if(id == 4)
				mesh->Draw(modelMat);
				
				id++;
			}
		}
	}
	
	void ASkeletalMesh::OnTick(double DeltaTime)
	{
		bool IsModelImported = m_ModelImporter->IsModelAlreadyLoaded();

		if (IsModelImported && !m_bIsModelReadyToDraw)
		{
			EvaluateMeshes();
			m_bIsModelReadyToDraw = true;
		}
	}

	void ASkeletalMesh::OnConstruct(void)
	{
		ImportModel();
	}

	glm::mat4 ASkeletalMesh::GetModelMat(void) const
	{
		glm::mat4 modelMat = glm::mat4(1.0f);

		modelMat = glm::translate(modelMat, m_Location);

		modelMat = glm::rotate(modelMat, m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		modelMat = glm::rotate(modelMat, m_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMat = glm::rotate(modelMat, m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

		modelMat = glm::scale(modelMat, m_Scale);
		//modelMat = glm::scale(modelMat, m_ModelImporter->GetRootScale());

		return modelMat;
	}

	void ASkeletalMesh::ImportModel()
	{
		m_ModelImporter = MakeShared<SkeletalModelImporter>(shared_from_this());

		m_MaterialsContainer.SetModelImporter(m_ModelImporter);

		m_ModelImporter->AsyncImportModel(m_ModelFilePath);
	}

	void ASkeletalMesh::InitializeMaterialSlots(uint8_t NumMaterials)
	{
		m_MaterialsContainer.InitializeMaterialSlots(NumMaterials);
	}

	void ASkeletalMesh::EvaluateMeshes(void)
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
					const aiScene* Scene = m_ModelImporter->GetScene();

					m_MaterialsContainer.LoadMaterialFromModelForIndex(	Scene->mMaterials[m_SubMeshes[idx]->GetMaterialIndex()],
																		m_SubMeshes[idx]->GetMaterialIndex());
				}
			}
		}
		m_OnAsyncLoadingFinishedDelegate.ExecuteIfBound(this);
		SceneDelegates::Get()->OnSkeletalMeshLoaded.Broadcast(this);
	}

	Material* ASkeletalMesh::GetMaterialInSlot(uint8_t SlotIndex)
	{
		return m_MaterialsContainer.GetMaterialInSlot(SlotIndex);
	}

	size_t ASkeletalMesh::GetNumMaterials(void) const
	{
		return m_MaterialsContainer.GetNumMaterials();
	}

	void ASkeletalMesh::SetMaterialForSlot(uint8_t SlotIndex, TSharedPtr<Material> const& Material)
	{
		m_MaterialsContainer.SetMaterialForSlot(SlotIndex, Material);
	}

	std::string const& ASkeletalMesh::GetFilePath(void) const
	{
		return m_ModelFilePath;
	}
	
	TSharedPtr<AssetImporter> const& ASkeletalMesh::GetImporter(void) const
	{
		return m_ModelImporter;
	}
}