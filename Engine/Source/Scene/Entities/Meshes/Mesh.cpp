#include "EnginePCH.hpp"

#include "Mesh.hpp"
#include "Renderer/RHI/Resources/RHIBuffers.hpp"
#include "Renderer/RHI/Resources/RHIVertexArray.hpp"
#include "Renderer/RHI/Resources/RHITexture.hpp"
#include "Renderer/RHI/Resources/RHIShader.hpp"
#include "Renderer/Renderer.hpp"

#include "StaticMesh.hpp"

namespace Engine
{
	
	Mesh::Mesh(std::vector<RHIVertex> const& Vertices, std::vector<uint32_t> const& Indices, bool LazyEvaluateMesh)
		:	m_bIsMeshLazyEvaluated(LazyEvaluateMesh)
	{
		if (!m_bIsMeshLazyEvaluated)
		{
			SetupMesh(Vertices, Indices);
		}
		else
		{
			m_LazyVertices	= Vertices;
			m_LazyIndices	= Indices;
		}
	}

	Mesh::Mesh(std::vector<RHIVertex>&& Vertices, std::vector<uint32_t>&& Indices, bool LazyEvaluateMesh)
		:	m_bIsMeshLazyEvaluated(LazyEvaluateMesh)
	{
		if (!m_bIsMeshLazyEvaluated)
		{
			SetupMesh(Vertices, Indices);
		}
		else
		{
			m_LazyVertices	= std::move(Vertices);
			m_LazyIndices	= std::move(Indices);
		}
	}

	Mesh::~Mesh()
	{}

	static std::shared_ptr<RHIShader>& GetShaderForMesh(bool IsMaterialEmmissive)
	{
		auto& shaderManager = Renderer::Get()->GetShaderManager();

		if (IsMaterialEmmissive)
		{
			return Renderer::Get()->GetRHI()->IsWireframeMode() ?
				shaderManager.GetResource("Shader_Wireframe") : shaderManager.GetResource("Shader_EmissiveMesh");
		}
		else
		{
			return Renderer::Get()->GetRHI()->IsWireframeMode() ?
				shaderManager.GetResource("Shader_Wireframe") : shaderManager.GetResource("Shader_StaticMesh");
		}
	}

	void Mesh::Draw(glm::mat4 const& ModelMat) const
	{  
		auto& shaderManager = Renderer::Get()->GetShaderManager();

		auto renderWithAssignedMaterial = 
		[this,&shaderManager, &ModelMat](std::shared_ptr<Material>& Material)
		{
			auto& meshShader = GetShaderForMesh(Material->IsMaterialEmissive());

			Material->Bind(meshShader);

			Renderer::Get()->Draw(meshShader, m_VAO, ModelMat);
		};

		if (m_bUseHardMaterialReference)
		{
			if (auto hardMaterialRef = m_HardMaterialReference.lock())
			{
				renderWithAssignedMaterial(hardMaterialRef);
			}
		}
		else if (auto staticMeshSlotMaterialRef = GetMaterialFromStaticMeshSlot(m_MaterialIndex))
		{
			renderWithAssignedMaterial(staticMeshSlotMaterialRef);
		}
		else
		{
			auto& meshShader = GetShaderForMesh(false);
			DefaultMaterials::BasicWhite->Bind(meshShader);

			Renderer::Get()->Draw(meshShader, m_VAO, ModelMat);
		}

		RendererStatistics& rendererStats = Renderer::GetMutableRendererStats();
		rendererStats.MeshesCount++;
		rendererStats.TotalTrisCount += m_MeshStatistics.TrisCount;
	}
	
	void Mesh::EvaluateMesh(void)
	{
		if (IsMeshLazyEvaluated() && !IsManualEvaluationPerformed())
		{ 
			SetupMesh(m_LazyVertices, m_LazyIndices);
			m_LazyVertices.clear();
			m_LazyIndices.clear();

			m_bEvaluated = true;
		}
	}

	void Mesh::SetupMesh(std::vector<RHIVertex> const& Vertices, std::vector<uint32_t> const& Indices)
	{
		m_VAO = RHIVertexArray::Create();

		RHIVertexBufferElement aPosition = RHIVertexBufferElement(RHIElementType::Float3, "aPosition");
		RHIVertexBufferElement aNormal = RHIVertexBufferElement(RHIElementType::Float3, "aNormal");
		RHIVertexBufferElement aTexUV = RHIVertexBufferElement(RHIElementType::Float2, "aTexUV");
		RHIVertexBufferElement aTangent = RHIVertexBufferElement(RHIElementType::Float3, "aTangent");

		auto vbo = RHIVertexBuffer::Create(Vertices);

		RHIVertexBufferLayout layout = { aPosition, aNormal, aTexUV, aTangent};
		vbo->SetLayout(std::make_unique< RHIVertexBufferLayout>(std::move(layout)));

		std::unique_ptr<RHIIndexBuffer> ibo = RHIIndexBuffer::Create(Indices);

		m_VAO->SetIndexBuffer(std::move(ibo));
		m_VAO->AddVertexBuffer(std::move(vbo));
	}

	std::shared_ptr<Material> Mesh::GetMaterialFromStaticMeshSlot(uint8_t Index) const
	{
		if (auto staticMeshOwner = m_StaticMeshOwner.lock())
		{
			auto material = staticMeshOwner->GetMaterialInSlot(m_MaterialIndex);
			if (material.has_value())
			{
				return material->get();
			}
			return nullptr;
		}
		return nullptr;
	}

	void Mesh::SetHardMaterialReference(std::weak_ptr<Material> Material)
	{
		m_bUseHardMaterialReference = true;
		m_HardMaterialReference = std::move(Material);
	}
}