#include "EnginePCH.hpp"

#include "Mesh.hpp"
#include "Renderer/RHI/Resources/RHIBuffers.hpp"
#include "Renderer/RHI/Resources/RHIVertexArray.hpp"
#include "Renderer/RHI/Resources/RHITexture.hpp"
#include "Renderer/RHI/Resources/RHIShader.hpp"
#include "Renderer/Renderer.hpp"
#include "Utilities/VariantUtility.hpp"

#include "StaticMesh.hpp"
#include "SkeletalMesh.hpp"

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
			m_LazyVertices	= MoveTemp(Vertices);
			m_LazyIndices	= MoveTemp(Indices);
		}
	}

	Mesh::~Mesh()
	{}

	static TSharedPtr<RHIShader>& GetShaderForMesh(bool IsMaterialEmmissive)
	{
		auto& shaderManager = Renderer::Get()->GetShaderManager();

		if (Renderer::Get()->bIsRenderingShadowMap)
		{
			return shaderManager.GetResource("Shader_ShadowMap");
		}
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
		[this,&shaderManager, &ModelMat](Material* Material)
		{
			auto& meshShader = GetShaderForMesh(Material->IsMaterialEmissive());

			if(!Renderer::Get()->bIsRenderingShadowMap)
			{
				Material->Bind(meshShader);
			}
			Renderer::Get()->Draw(meshShader, m_VAO, ModelMat);
		};

		if (m_bUseHardMaterialReference)
		{
			if (auto hardMaterialRef = m_HardMaterialReference.lock())
			{
				renderWithAssignedMaterial(hardMaterialRef.get());
			}
		}
		else if (auto staticMeshSlotMaterial = GetMaterialFromStaticMeshSlot(m_MaterialIndex))
		{
			renderWithAssignedMaterial(staticMeshSlotMaterial);
		}
		else
		{
			auto& meshShader = GetShaderForMesh(false);

			if (!Renderer::Get()->bIsRenderingShadowMap)
			{
				DefaultMaterials::BasicWhite->Bind(meshShader);
			}
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
		vbo->SetLayout(MakeUnique< RHIVertexBufferLayout>(MoveTemp(layout)));

		TUniquePtr<RHIIndexBuffer> ibo = RHIIndexBuffer::Create(Indices);

		m_VAO->SetIndexBuffer(MoveTemp(ibo));
		m_VAO->AddVertexBuffer(MoveTemp(vbo));
	}

	Material* Mesh::GetMaterialFromStaticMeshSlot(uint8_t Index) const
	{
		return std::visit(
			[this](auto const& MeshOwner) -> Material*
			{
				if (auto meshOwner = MeshOwner.lock())
				{
					return meshOwner->GetMaterialInSlot(m_MaterialIndex);
				}
				return nullptr;
			}
		, m_OwnerActor);

		return nullptr;
	}

	void Mesh::SetHardMaterialReference(TWeakPtr<Material> Material)
	{
		m_bUseHardMaterialReference = true;
		m_HardMaterialReference = MoveTemp(Material);
	}

	SkinnedMesh::SkinnedMesh(std::vector<RHIVertex>&& Vertices, std::vector<uint32_t>&& Indices, std::vector<VertexBoneData>&& BoneData)
	{
		m_bIsMeshLazyEvaluated = true;

		m_LazyVertices = MoveTemp(Vertices);
		m_LazyIndices = MoveTemp(Indices);
		m_LazyBoneInfluenceData = MoveTemp(BoneData);
	}

	void SkinnedMesh::SetupMesh(std::vector<RHIVertex> const& Vertices, std::vector<uint32_t> const& Indices, std::vector<VertexBoneData> const& BoneData)
	{ 
		std::vector<RHIAnimatedVertex> AnimatedVertices;

		for (uint32_t idx = 0 ; idx < Vertices.size() ;idx++)
		{
			RHIAnimatedVertex v;
			v = Vertices[idx]; 
			 
			std::memcpy(&v.BoneIDs, BoneData[idx].BoneIDs, sizeof(int32_t) * 4);
			std::memcpy(&v.Weights, BoneData[idx].Weights, sizeof(float) * 4);
		
			AnimatedVertices.emplace_back(MoveTemp(v));
		}  
		    
		m_VAO = RHIVertexArray::Create();
		 
		RHIVertexBufferElement aPosition	= RHIVertexBufferElement(RHIElementType::Float3, "aPosition");
		RHIVertexBufferElement aNormal		= RHIVertexBufferElement(RHIElementType::Float3, "aNormal");
		RHIVertexBufferElement aTexUV		= RHIVertexBufferElement(RHIElementType::Float2, "aTexUV");
		RHIVertexBufferElement aTangent		= RHIVertexBufferElement(RHIElementType::Float3, "aTangent");
		RHIVertexBufferElement aBoneID		= RHIVertexBufferElement(RHIElementType::Int4, "aBoneIDs");
		RHIVertexBufferElement aWeight		= RHIVertexBufferElement(RHIElementType::Float4, "aWeights");
		 
		auto VBO = RHIVertexBuffer::Create(AnimatedVertices.data(), AnimatedVertices.size() * sizeof(RHIAnimatedVertex));
		  
		RHIVertexBufferLayout Layout = { aPosition, aNormal, aTexUV, aTangent,aBoneID, aWeight };
		VBO->SetLayout(MakeUnique< RHIVertexBufferLayout>(MoveTemp(Layout)));

		TUniquePtr<RHIIndexBuffer> IBO = RHIIndexBuffer::Create(Indices);

		m_VAO->SetIndexBuffer(MoveTemp(IBO));
		m_VAO->AddVertexBuffer(MoveTemp(VBO));
	}

	void SkinnedMesh::EvaluateMesh(void)
	{
		if (IsMeshLazyEvaluated() && !IsManualEvaluationPerformed())
		{
			SetupMesh(m_LazyVertices, m_LazyIndices,m_LazyBoneInfluenceData);
			m_LazyVertices.clear();
			m_LazyIndices.clear();
			m_LazyBoneInfluenceData.clear();

			m_bEvaluated = true;
		}
	}

	void SkinnedMesh::Draw(glm::mat4 const& ModelMat) const
	{
		auto& shaderManager = Renderer::Get()->GetShaderManager();

		auto renderWithAssignedMaterial =
			[this, &shaderManager, &ModelMat](Material* Material)
		{
			auto& meshShader = shaderManager.GetResource("Shader_SkinnedMeshBoneInfluence");

			if (!Renderer::Get()->bIsRenderingShadowMap)
			{
				Material->Bind(meshShader);
			}
			Renderer::Get()->Draw(meshShader, m_VAO, ModelMat);
		};

		if (m_bUseHardMaterialReference)
		{
			if (auto hardMaterialRef = m_HardMaterialReference.lock())
			{
				renderWithAssignedMaterial(hardMaterialRef.get());
			}
		}
		else if (auto staticMeshSlotMaterial = GetMaterialFromStaticMeshSlot(m_MaterialIndex))
		{
			renderWithAssignedMaterial(staticMeshSlotMaterial);
		}
		else
		{
			auto& meshShader = GetShaderForMesh(false);

			if (!Renderer::Get()->bIsRenderingShadowMap)
			{
				DefaultMaterials::BasicWhite->Bind(meshShader);
			}
			Renderer::Get()->Draw(meshShader, m_VAO, ModelMat);
		}

		RendererStatistics& rendererStats = Renderer::GetMutableRendererStats();
		rendererStats.MeshesCount++;
		rendererStats.TotalTrisCount += m_MeshStatistics.TrisCount;
	}
}