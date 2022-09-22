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
		SetupMaterial({});
	}

	Mesh::Mesh(std::vector<RHIVertex>&& Vertices, std::vector<uint32_t>&& Indices, std::vector<std::shared_ptr<RHITexture2D>>&& Textures)
		:	m_Vertices(std::move(Vertices)),
			m_Indices(std::move(Indices))
	{
		SetupMesh();
		SetupMaterial(std::move(Textures));
	}

	void Mesh::Draw(std::shared_ptr<RHIShader>& Shader, glm::mat4 const& ModelMat) const
	{  
		if (m_Material != nullptr)
		{
			m_Material->Bind(Shader);
		}
		else
		{
			DefaultMaterials::BasicWhite->Bind(Shader);
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

	void Mesh::SetupMaterial(std::vector<std::shared_ptr<RHITexture2D>>&& Textures)
	{
		if (Textures.size() > 0)
		{
			m_Material = std::make_shared<Material>();

			for (auto& texture : Textures)
			{
				if (texture->GetType() == RHITextureType::Diffuse)
				{
					m_Material->SetDiffuseTexture(texture);
				}
				else if (texture->GetType() == RHITextureType::Specular)
				{
					
				}
			}
		}
	}
}