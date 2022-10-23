#pragma once

#include "Core/CoreMinimal.hpp"

#include "RHITexture.hpp"
#include "RHIShader.hpp"
#include "../../Materials/Material.hpp"

namespace Engine::RHI
{
	template <typename ResourceType, typename ResourceID>
		requires std::is_base_of_v<RHIResource, ResourceType>
	class RHIResourceManager
	{
	private:
		std::unordered_map<ResourceID, TSharedPtr<ResourceType>> m_Resources;

	public:
		template <typename... Args> 
		bool LoadResource(ResourceID const& ResourceID, Args&&... _Args);

		TSharedPtr<ResourceType>& GetResource(ResourceID ResourceID);
		const TSharedPtr<ResourceType>& GetResource(ResourceID ResourceID) const;

	private:
		template <typename Resource>
		bool InsertResource(ResourceID ResourceID, Resource&& _Resource);
	};

	using FilePath		= std::string;
	using ShaderName	= std::string;
	using MaterialName = std::string;

	using RHITexture2DManager	= RHIResourceManager<RHITexture2D, FilePath>;
	using RHIShaderManager		= RHIResourceManager<RHIShader, ShaderName>;
	using MaterialManager		= RHIResourceManager<Material, MaterialName>;
}

#include "RHIResourceManager.inl"