#pragma once

#include "Mesh.hpp"

#include "Renderer/RHI/RHITypes.hpp"

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;

namespace Engine
{
    namespace RHI
    {
        class RHITexture2D;
        class RHIShader;
    }

	class OStaticMesh : public Object, public Tickable
	{
    private:
        std::vector<OMesh>  m_SubMeshes;
        std::string         m_Directory;

        void LoadModel(std::string_view FilePath);

        void ProcessNode(aiNode* Node, const aiScene* Scene);
        OMesh ProcessMesh(aiMesh* Mesh, const aiScene* Scene);

        std::vector<std::shared_ptr<RHITexture2D>> LoadMaterialTextures(aiMaterial* Material, RHITextureType Type);

    public:
        OStaticMesh(std::string_view FilePath);

	public:
        void Draw(std::shared_ptr<RHIShader>& Shader);
		void OnTick(double DeltaTime) override;
	};
}