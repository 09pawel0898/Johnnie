#pragma once

#include "Core/CoreMinimal.hpp"

#include "RHITexture.hpp"
#include "RHIShader.hpp"

namespace Engine::RHI
{
	template <typename ResourceType, typename ResourceID>
		requires std::is_base_of_v<RHIResource, ResourceType>
	class RHIResourceManager
	{
	private:
		std::unordered_map<ResourceID, std::shared_ptr<ResourceType>> m_Resources;

	public:
		template <typename... Args> 
		bool LoadResource(ResourceID const& ResourceID, Args&&... _Args);

		std::shared_ptr<ResourceType>& GetResource(ResourceID ResourceID);
		const std::shared_ptr<ResourceType>& GetResource(ResourceID ResourceID) const;

	private:
		template <typename Resource>
		bool InsertResource(ResourceID ResourceID, Resource&& _Resource);
	};

	using FilePath = std::string;
	using ShaderName = std::string;

	using RHITexture2DManager	= RHIResourceManager<RHITexture2D, FilePath>;
	using RHIShaderManager		= RHIResourceManager<RHIShader, ShaderName>;


}

#include "RHIResourceManager.inl"