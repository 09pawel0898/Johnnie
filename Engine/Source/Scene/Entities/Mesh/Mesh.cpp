#include "EnginePCH.hpp"

#include "Mesh.hpp"
#include "Renderer/RHI/Resources/RHIBuffers.hpp"
#include "Renderer/RHI/Resources/RHIVertexArray.hpp"
#include "Renderer/RHI/Resources/RHITexture.hpp"
#include "Renderer/RHI/Resources/RHIShader.hpp"
#include "Renderer/Renderer.hpp"

namespace Engine
{
	OMesh::OMesh(std::vector<RHIVertex> const& Vertices, std::vector<uint32_t> const& Indices, std::vector<std::shared_ptr<RHITexture>>&& Textures)
	{
		m_Vertices	= Vertices;
		m_Indices	= Indices;
		m_Textures	= std::move(Textures);
		SetupMesh();
	}

	void OMesh::Draw(std::shared_ptr<RHIShader> const& Shader)
	{  

		uint8_t diffuseNr = 1, specularNr = 1;

		for (uint8_t idx = 0; idx < m_Textures.size(); idx++)
		{
			m_Textures[idx]->Bind(idx);

			std::string shaderNum;


		}

		//for (unsigned int i = 0; i < textures.size(); i++)
		//{
		//	glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
		//	// retrieve texture number (the N in diffuse_textureN)
		//	string number;
		//	string name = textures[i].type;
		//	if (name == "texture_diffuse")
		//		number = std::to_string(diffuseNr++);
		//	else if (name == "texture_specular")
		//		number = std::to_string(specularNr++);
		//
		//	shader.setInt(("material." + name + number).c_str(), i);
		//	glBindTexture(GL_TEXTURE_2D, textures[i].id);
		//}
		//glActiveTexture(GL_TEXTURE0);

		Renderer::Get()->Draw(Shader, m_VAO, GetModelMat());
	}
	
	void OMesh::SetupMesh()
	{
		m_VAO = RHIVertexArray::Create();

		RHIVertexBufferElement aPosition = RHIVertexBufferElement(RHIElementType::Float3, "aPosition");
		RHIVertexBufferElement aNormal = RHIVertexBufferElement(RHIElementType::Float3, "aNormal");
		RHIVertexBufferElement aTexUV = RHIVertexBufferElement(RHIElementType::Float3, "aTexUV");

		auto vbo = RHIVertexBuffer::Create(m_Vertices);

		RHIVertexBufferLayout layout = { aPosition, aNormal, aTexUV };
		vbo->SetLayout(std::make_unique< RHIVertexBufferLayout>(std::move(layout)));

		std::unique_ptr<RHIIndexBuffer> ibo = RHIIndexBuffer::Create(m_Indices);

		m_VAO->SetIndexBuffer(std::move(ibo));
		m_VAO->AddVertexBuffer(std::move(vbo));
	}
}