#include "EnginePCH.hpp"

#include "AssimpHelpers.hpp"

namespace Engine
{
	TUniquePtr<AssetImporter> AssetImporter::Create(void)
	{
		return MakeUnique<AssetImporter>();
	}
	
	Assimp::Importer& Engine::AssetImporter::GetImporter(void)
	{
		return m_Importer;
	}

	Assimp::Importer const& Engine::AssetImporter::GetImporter(void) const
	{
		return m_Importer;
	}

	const aiScene* Engine::AssetImporter::GetScene(void)
	{
		return m_Importer.GetScene();
	}

	aiTextureType RHITextureTypeToAssimpTextureType(RHI::RHITextureType RHITextureType)
	{
		static std::unordered_map<RHI::RHITextureType, aiTextureType> map =
		{
			{RHI::RHITextureType::Diffuse,	aiTextureType_DIFFUSE	},
			{RHI::RHITextureType::Specular,	aiTextureType_SPECULAR	},
			{RHI::RHITextureType::Normal,	aiTextureType_NORMALS	},
		};
		return map[RHITextureType];
	}
}
