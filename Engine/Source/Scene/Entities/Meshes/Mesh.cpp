#include "EnginePCH.hpp"

#include "Mesh.hpp"
#include "Renderer/RHI/Resources/RHIBuffers.hpp"
#include "Renderer/RHI/Resources/RHIVertexArray.hpp"
#include "Renderer/RHI/Resources/RHITexture.hpp"
#include "Renderer/RHI/Resources/RHIShader.hpp"
#include "Renderer/Renderer.hpp"

namespace Engine
{
	Mesh::Mesh(std::vector<RHIVertex> const& Vertices, std::vector<uint32_t> const& Indices)
	{
		m_Vertices = Vertices;
		m_Indices = Indices;

		SetupMesh();
	}

	Mesh::Mesh(std::vector<RHIVertex>&& Vertices, std::vector<uint32_t>&& Indices, std::vector<std::shared_ptr<RHITexture2D>>&& Textures)
		:	m_Vertices(std::move(Vertices)),
			m_Indices(std::move(Indices)),
			m_Textures(std::move(Textures))
	{
		SetupMesh();
	}

	void Mesh::Draw(std::shared_ptr<RHIShader>& Shader, glm::mat4 const& ModelMat) const
	{  
		uint8_t diffuseNr = 1, specularNr = 1;
		Shader->Bind();

		if (m_Textures.size() == 0)
		{
			Shader->SetInt("uUseTextures", 0);
			Shader->SetFloat3("uBaseColor", glm::vec3(0.123f, 0.307f, 0.184f));
		}
		else
		{
			Shader->SetInt("uUseTextures", 1);
			for (uint8_t idx = 0; idx < m_Textures.size(); idx++)
			{
				m_Textures[idx]->Bind(idx);

				std::string texNum;

				std::string texName = GetUniformNameByTextureType(m_Textures[idx]->GetType());
				if (texName == "texture_diffuse")
				{
					texName = std::to_string(diffuseNr++);
				}
				else if (texName == "texture_specular")
				{
					texName = std::to_string(specularNr++);
				}
				Shader->SetInt((texName + texNum).c_str(), idx);
			}
		}
		Renderer::Get()->Draw(Shader, m_VAO, ModelMat);
	}
	
	void Mesh::SetupMesh(void)
	{
		m_VAO = RHIVertexArray::Create();

		RHIVertexBufferElement aPosition = RHIVertexBufferElement(RHIElementType::Float3, "aPosition");
		RHIVertexBufferElement aNormal = RHIVertexBufferElement(RHIElementType::Float3, "aNormal");
		RHIVertexBufferElement aTexUV = RHIVertexBufferElement(RHIElementType::Float2, "aTexUV");

		auto vbo = RHIVertexBuffer::Create(m_Vertices);

		RHIVertexBufferLayout layout = { aPosition, aNormal, aTexUV };
		vbo->SetLayout(std::make_unique< RHIVertexBufferLayout>(std::move(layout)));

		std::unique_ptr<RHIIndexBuffer> ibo = RHIIndexBuffer::Create(m_Indices);

		m_VAO->SetIndexBuffer(std::move(ibo));
		m_VAO->AddVertexBuffer(std::move(vbo));
	}
}