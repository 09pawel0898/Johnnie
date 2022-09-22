#pragma once

#include "Core/CoreMinimal.hpp"
#include "Renderer/RHI/RHITypes.hpp"
#include "Renderer/Materials/Material.hpp"

namespace Engine
{
	struct TextureData 
	{
		uint32_t			ID;
		std::string_view	Type;
	};

	namespace RHI
	{
		class RHIVertexArray;
		class RHITexture2D;
		class RHIShader;
	}
	using namespace RHI;

	class Mesh
	{
	private:
		std::vector<RHIVertex>		m_Vertices;
		std::vector<uint32_t>		m_Indices;

	private:
		std::shared_ptr<RHIVertexArray>	m_VAO;

		std::shared_ptr<Material> m_Material{ nullptr };

		void SetupMesh(void);
		void SetupMaterial(std::vector<std::shared_ptr<RHITexture2D>>&& Textures);

	public:
		Mesh(std::vector<RHIVertex> const& Vertices, std::vector<uint32_t> const& Indices);
		Mesh(std::vector<RHIVertex>&& Vertices, std::vector<uint32_t>&& Indices, std::vector<std::shared_ptr<RHITexture2D>>&& Textures);
	
	public:
		void Draw(std::shared_ptr<RHIShader>& Shader, glm::mat4 const& ModelMat) const;

		std::shared_ptr<Material>& GetMaterial(void)
		{
			return m_Material;
		}
	};
}