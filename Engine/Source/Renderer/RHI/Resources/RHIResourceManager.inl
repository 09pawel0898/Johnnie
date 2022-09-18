
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

		std::shared_ptr<ResourceType> newResource =
			std::make_shared<ResourceType>(ResourceID, std::forward<Args>(_Args)...);

		return InsertResource(ResourceID, std::move(newResource));
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
		
		std::shared_ptr<RHITexture2D> newResource = RHITexture2D::Create(FilePath, std::forward<Args>(_Args)...);

		return InsertResource(FilePath, std::move(newResource));
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
		std::shared_ptr<RHIShader> newResource = RHIShader::Create(ShaderName, std::forward<Args>(_Args)...);

		return InsertResource(ShaderName, std::move(newResource));
	}

	template <typename ResourceType, typename ResourceID>
		requires std::is_base_of_v<RHIResource, ResourceType>
	template <typename Resource>
	bool RHIResourceManager<ResourceType, ResourceID>::InsertResource(ResourceID ResourceID, Resource&& _Resource)
	{
		auto newResource = m_Resources.insert(std::make_pair(	ResourceID, 
																std::forward<Resource>(_Resource)));
		Check(newResource.second);
		return true;
	}

	template <typename ResourceType, typename ResourceID>
		requires std::is_base_of_v<RHIResource, ResourceType>
	std::shared_ptr<ResourceType>& RHIResourceManager<ResourceType, ResourceID>::GetResource(ResourceID ResourceID)
	{
		auto foundResource = m_Resources.find(ResourceID);
		assert(foundResource != m_Resources.end());

		return foundResource->second;
	}

	template <typename ResourceType, typename ResourceID>
		requires std::is_base_of_v<RHIResource, ResourceType>
	const std::shared_ptr<ResourceType>& RHIResourceManager<ResourceType, ResourceID>::GetResource(ResourceID ResourceID) const
	{
		auto foundResource = m_Resources.find(ResourceID);
		assert(foundResource != m_Resources.end());

		return foundResource->second;
	}
}
