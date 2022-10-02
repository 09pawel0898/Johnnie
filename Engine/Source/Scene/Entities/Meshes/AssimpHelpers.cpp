#include "EnginePCH.hpp"

#include "AssimpHelpers.hpp"

namespace Engine
{
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
}
