#pragma once

#include "Core/CoreMinimal.hpp"
#include "Renderer/RHI/RHITypes.hpp"

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

	class OMesh
	{
	private:
		std::vector<RHIVertex>		m_Vertices;
		std::vector<uint32_t>		m_Indices;

	private:
		std::shared_ptr<RHIVertexArray>				m_VAO;
		std::vector<std::shared_ptr<RHITexture2D>>	m_Textures;
		void SetupMesh(void);

	public:
		OMesh(std::vector<RHIVertex> const& Vertices, std::vector<uint32_t> const& Indices);
		OMesh(std::vector<RHIVertex>&& Vertices, std::vector<uint32_t>&& Indices, std::vector<std::shared_ptr<RHITexture2D>>&& Textures);
	
	public:
		void Draw(std::shared_ptr<RHIShader>& Shader, glm::mat4 const& ModelMat);
	};
}