
#include "Log/Log.hpp"

namespace Engine::RHI
{
	template <typename ResourceType, typename ResourceID>
		requires std::is_base_of_v<RHIResource, ResourceType>
	template <typename... Args>
	bool RHIResourceManager<ResourceType, ResourceID>::LoadResource(ResourceID const& ResourceID, Args&&... _Args)
	{
		if (m_Resources.find(ResourceID) != m_Resources.end())
		{
			//LOG(RHI, Warning, "Attempting to add resource that is already loaded.");
			return false;
		}

		TSharedPtr<ResourceType> newResource =
			MakeShared<ResourceType>(ResourceID, Forward<Args>(_Args)...);

		return InsertResource(ResourceID, MoveTemp(newResource));
	}

	template <>
	template <typename... Args>
	bool RHIResourceManager<RHITexture2D, FilePath>::LoadResource(FilePath const& FilePath, Args&&... _Args)
	{
		if (m_Resources.find(FilePath) != m_Resources.end())
		{
			//LOG(RHI, Warning, "Attempting to add resource that is already loaded.");
			return false;
		}
		
		TSharedPtr<RHITexture2D> newResource = RHITexture2D::Create(FilePath, Forward<Args>(_Args)...);

		return InsertResource(FilePath, MoveTemp(newResource));
	}

	template <>
	template <typename... Args>
	bool RHIResourceManager<RHIShader, ShaderName>::LoadResource(ShaderName const& ShaderName, Args&&... _Args)
	{
		if (m_Resources.find(ShaderName) != m_Resources.end())
		{
			LOG(RHI, Warning, "Attempting to load shader that is already loaded.");
			return false;
		}
		TSharedPtr<RHIShader> newResource = RHIShader::Create(ShaderName, Forward<Args>(_Args)...);

		return InsertResource(ShaderName, MoveTemp(newResource));
	}

	template <typename ResourceType, typename ResourceID>
		requires std::is_base_of_v<RHIResource, ResourceType>
	template <typename Resource>
	bool RHIResourceManager<ResourceType, ResourceID>::InsertResource(ResourceID ResourceID, Resource&& _Resource)
	{
		auto newResource = m_Resources.insert(std::make_pair(	ResourceID, 
																Forward<Resource>(_Resource)));
		Check(newResource.second);
		return true;
	}

	template <typename ResourceType, typename ResourceID>
		requires std::is_base_of_v<RHIResource, ResourceType>
	TSharedPtr<ResourceType>& RHIResourceManager<ResourceType, ResourceID>::GetResource(ResourceID ResourceID)
	{
		auto foundResource = m_Resources.find(ResourceID);
		Check(foundResource != m_Resources.end());

		return foundResource->second;
	}

	template <typename ResourceType, typename ResourceID>
		requires std::is_base_of_v<RHIResource, ResourceType>
	const TSharedPtr<ResourceType>& RHIResourceManager<ResourceType, ResourceID>::GetResource(ResourceID ResourceID) const
	{
		auto foundResource = m_Resources.find(ResourceID);
		Check(foundResource != m_Resources.end());

		return foundResource->second;
	}
}
