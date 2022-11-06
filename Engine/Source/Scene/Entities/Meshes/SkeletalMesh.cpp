#include "EnginePCH.hpp"
#include "SkeletalMesh.hpp"
#include "Log/Log.hpp"
#include "AssetImporter.hpp"

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
	}
	
	void ASkeletalMesh::OnTick(double DeltaTime)
	{
	}
	
	void ASkeletalMesh::OnConstruct(void)
	{
		ImportModel();
	}

	void ASkeletalMesh::ImportModel()
	{
		m_ModelImporter = MakeUnique<SkeletalModelImporter>();

		m_ModelImporter->OnMaterialsInforAcquiredDelegate.BindLambda(
			[this](uint8_t NumMaterials)
			{
				InitializeMaterialSlots(NumMaterials);
			});

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

	void ASkeletalMesh::InitializeMaterialSlots(uint8_t NumMaterials)
	{
		m_Materials.clear();
		m_MaterialProcessed.clear();

		Emplace_N_MaterialSlots(NumMaterials);
	}
}