#pragma once

#include "Core/CoreMinimal.hpp"
#include "../Object.hpp"
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
		class RHITexture;
		class RHIShader;
	}
	using namespace RHI;

	class OMesh : Object
	{
	private:
		std::vector<RHIVertex>		m_Vertices;
		std::vector<uint32_t>		m_Indices;

		OMesh(std::vector<RHIVertex> const& Vertices, std::vector<uint32_t> const& Indices, std::vector<std::shared_ptr<RHITexture>>&& Textures);

	private:
		std::shared_ptr<RHIVertexArray>				m_VAO;
		std::vector<std::shared_ptr<RHITexture>>	m_Textures;
		void SetupMesh();
	
	public:
		void Draw(std::shared_ptr<RHIShader> const& Shader);
	};
}