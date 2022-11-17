#include "EnginePCH.hpp"

#include "Mesh.hpp"
#include "Renderer/RHI/Resources/RHIBuffers.hpp"
#include "Renderer/RHI/Resources/RHIVertexArray.hpp"
#include "Renderer/RHI/Resources/RHITexture.hpp"
#include "Renderer/RHI/Resources/RHIShader.hpp"
#include "Renderer/Renderer.hpp"
#include "Core/Application/Application.hpp"
#include "Utilities/VariantUtility.hpp"

#include "StaticMesh.hpp"
#include "SkeletalMesh/SkeletalMesh.hpp"
#include "AssetImporter.hpp"

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
		auto& RHI = Renderer::Get()->GetRHI();
		auto& ShaderManager = Renderer::Get()->GetShaderManager();

		RenderingFlags Flags = Renderer::Get()->GetRenderingFlags();

		if (Flags & R_ShadowMap)
		{
			return ShaderManager.GetResource("Shader_ShadowMap");
		}
		else if (Flags & R_Wireframe)
		{
			return ShaderManager.GetResource("Shader_Wireframe");
		}
		else
		{
			return (IsMaterialEmmissive) ?  ShaderManager.GetResource("Shader_EmissiveMesh") :
											ShaderManager.GetResource("Shader_Mesh");
		}
	}

	void Mesh::Draw(glm::mat4 const& ModelMat) const
	{  
		auto& shaderManager = Renderer::Get()->GetShaderManager();
		
		auto renderWithAssignedMaterial = 
		[this,&shaderManager, &ModelMat](Material* Material)
		{
			auto& meshShader = GetShaderForMesh(Material->IsMaterialEmissive());

			meshShader->SetInt("uIsSkinnedMesh", (int32_t)false);

			meshShader->SetFloat("uCastShadows", Renderer::Get()->GetShadowsSettings().bRenderShadows);
			meshShader->SetFloat("uShadowIntensity", Renderer::Get()->GetShadowsSettings().ShadowsIntensity);

			if(!Renderer::Get()->HasFlag(R_ShadowMap))
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
		else if (auto staticMeshSlotMaterial = GetMaterialFromOwnerActorSlot(m_MaterialIndex))
		{
			renderWithAssignedMaterial(staticMeshSlotMaterial);
		}
		else
		{
			auto& meshShader = GetShaderForMesh(false);

			meshShader->SetInt("uIsSkinnedMesh", (int32_t)false);

			if (!Renderer::Get()->HasFlag(R_ShadowMap))
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

	Material* Mesh::GetMaterialFromOwnerActorSlot(uint8_t Index) const
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

	SkinnedMesh::SkinnedMesh(std::vector<RHIVertex>&& Vertices, std::vector<uint32_t>&& Indices, std::vector<VertexBoneInfluenceData>&& BoneData)
	{
		m_bIsMeshLazyEvaluated = true;

		m_LazyVertices = MoveTemp(Vertices);
		m_LazyIndices = MoveTemp(Indices);
		m_LazyBoneInfluenceData = MoveTemp(BoneData);
	}

	void SkinnedMesh::SetupMesh(std::vector<RHIVertex> const& Vertices, std::vector<uint32_t> const& Indices, std::vector<VertexBoneInfluenceData> const& BoneData)
	{ 
		std::vector<RHISkinnedVertex> AnimatedVertices;

		for (uint32_t idx = 0 ; idx < Vertices.size() ;idx++)
		{
			RHISkinnedVertex SkinnedVertex = RHISkinnedVertex(Vertices[idx]);
			 
			std::memcpy(&SkinnedVertex.BoneIDs, BoneData[idx].BoneIDs, sizeof(int32_t) * 4);
			std::memcpy(&SkinnedVertex.Weights, BoneData[idx].Weights, sizeof(float) * 4);
		
			AnimatedVertices.emplace_back(MoveTemp(SkinnedVertex));
		}  
		    
		m_VAO = RHIVertexArray::Create();
		 
		RHIVertexBufferElement aPosition	= RHIVertexBufferElement(RHIElementType::Float3, "aPosition");
		RHIVertexBufferElement aNormal		= RHIVertexBufferElement(RHIElementType::Float3, "aNormal");
		RHIVertexBufferElement aTexUV		= RHIVertexBufferElement(RHIElementType::Float2, "aTexUV");
		RHIVertexBufferElement aTangent		= RHIVertexBufferElement(RHIElementType::Float3, "aTangent");
		RHIVertexBufferElement aBoneID		= RHIVertexBufferElement(RHIElementType::Int4, "aBoneIDs");
		RHIVertexBufferElement aWeight		= RHIVertexBufferElement(RHIElementType::Float4, "aWeights");
		 
		auto VBO = RHIVertexBuffer::Create(	AnimatedVertices.data(), 
											(uint32_t)(AnimatedVertices.size() * sizeof(RHISkinnedVertex)));
		  
		RHIVertexBufferLayout Layout = { aPosition, aNormal, aTexUV, aTangent,aBoneID, aWeight };
		VBO->SetLayout(MakeUnique<RHIVertexBufferLayout>(MoveTemp(Layout)));

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


	static TSharedPtr<RHIShader>& GetShaderForSkeletalMesh()
	{
		auto& RHI			= Renderer::Get()->GetRHI();
		auto& ShaderManager = Renderer::Get()->GetShaderManager();

		RenderingFlags Flags = Renderer::Get()->GetRenderingFlags();

		if (Flags & R_ShadowMap)
		{
			return ShaderManager.GetResource("Shader_ShadowMap");
		}
		else if(Flags & R_Wireframe)
		{
			return ShaderManager.GetResource("Shader_Wireframe");
		}
		else
		{
			return (Flags & R_BoneInfuence) ? ShaderManager.GetResource("Shader_SkinnedMeshBoneInfluence") :
											  ShaderManager.GetResource("Shader_Mesh");
		}
	}

	void SkinnedMesh::Draw(glm::mat4 const& ModelMat) const
	{
		auto& shaderManager = Renderer::Get()->GetShaderManager();

		auto renderWithAssignedMaterial =
			[this, &shaderManager, &ModelMat](Material* Material)
		{
			auto& meshShader = GetShaderForSkeletalMesh();

			meshShader->Bind();
			meshShader->SetInt("uIsSkinnedMesh", (int32_t)true);
			meshShader->SetFloat("uCastShadows", Renderer::Get()->GetShadowsSettings().bRenderShadows);
			meshShader->SetFloat("uShadowIntensity", Renderer::Get()->GetShadowsSettings().ShadowsIntensity);

			// update bone data
			std::visit(MakeInlineVisitor(
				[this,&meshShader](TWeakPtr<ASkeletalMesh> const& MeshOwner)
				{
					TSharedPtr<ASkeletalMesh> SkeletalMeshOwner = MeshOwner.lock(); 
					if (SkeletalMeshOwner)
					{
						const TSharedPtr<AssetImporter>& AssetImporter = SkeletalMeshOwner->GetImporter();
						if (AssetImporter)
						{
							SkeletalModelImporter* SkelModelImporter = Cast<SkeletalModelImporter>(AssetImporter.get());
							if (SkelModelImporter)
							{
								std::vector<glm::mat4> BoneTransforms;

								double AnimTimer = Application::Get()->GetTimeSinceStartInSeconds();
								
								SkelModelImporter->GetBoneTransforms((float)AnimTimer, BoneTransforms);
								meshShader->SetMat4Array("uBones", BoneTransforms.data(), BoneTransforms.size());

								glm::mat4 t = glm::mat4(1.f);
								t = glm::scale(t, SkelModelImporter->GetRootScale());
								meshShader->SetMat4("uFixedScaleMatrix", t);

							}
						}
					}
				},
				[this](TWeakPtr<AStaticMesh> const& MeshOwner){}
				)
			, m_OwnerActor);


			if (!Renderer::Get()->HasFlag(R_ShadowMap))
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
		else if (auto staticMeshSlotMaterial = GetMaterialFromOwnerActorSlot(m_MaterialIndex))
		{
			renderWithAssignedMaterial(staticMeshSlotMaterial);
		}
		else
		{
			auto& meshShader = GetShaderForMesh(false);
			meshShader->SetInt("uIsSkinnedMesh", (int32_t)true);

			if (!Renderer::Get()->HasFlag(R_ShadowMap))
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