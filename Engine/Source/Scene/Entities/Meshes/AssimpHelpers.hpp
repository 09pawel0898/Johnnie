#pragma once

#include "Core/CoreMinimal.hpp"
#include "Renderer/RHI/RHITypes.hpp"
#include "Utilities/Singleton.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Engine
{
	class AssetImporter : public Singleton<AssetImporter>
	{
	private:
		Assimp::Importer m_Importer;

	public:
		AssetImporter() = default;
		~AssetImporter() noexcept{}

		Assimp::Importer& GetImporter(void);
		Assimp::Importer const& GetImporter(void) const;

		const aiScene* GetScene(void);
	};

	FORCEINLINE aiTextureType RHITextureTypeToAssimpTextureType(RHI::RHITextureType RHITextureType)
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

