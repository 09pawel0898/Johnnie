#include "EnginePCH.hpp"
#include "SkeletalMesh.hpp"

#include "AssimpHelpers.hpp"

namespace Engine
{
	ASkeletalMesh::ASkeletalMesh(std::string const& FilePath, OnSkeletalMeshAsyncLoadingFinishedDelegate OnLoadingFinished, glm::vec3 const& WorldLocation)
	{
	}

	ASkeletalMesh::~ASkeletalMesh()
	{
	}
	
	void ASkeletalMesh::Draw(void) const
	{
	}
	
	void ASkeletalMesh::OnTick(double DeltaTime)
	{
	}
	
	void ASkeletalMesh::OnConstruct(void)
	{
	}
}