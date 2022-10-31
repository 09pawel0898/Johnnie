#pragma once

#include "Core/CoreMinimal.hpp"
#include "Renderer/RHI/RHITypes.hpp"
#include "Utilities/Singleton.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Engine
{
	class AssetImporter
	{
	private:
		Assimp::Importer m_Importer;
		
	public:
		AssetImporter() = default;

		static TUniquePtr<AssetImporter> Create(void);

	public:
		Assimp::Importer& GetImporter(void);
		Assimp::Importer const& GetImporter(void) const;

		const aiScene* GetScene(void);
	};

	aiTextureType RHITextureTypeToAssimpTextureType(RHI::RHITextureType RHITextureType);
}

