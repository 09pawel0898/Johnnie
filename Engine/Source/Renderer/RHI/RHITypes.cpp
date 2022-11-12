#include "EnginePCH.hpp"

#include "RHITypes.hpp"

namespace Engine::RHI
{
	size_t Engine::RHI::GetRHIElementTypeSize(RHIElementType ElementType)
	{
		static const std::unordered_map<RHIElementType, size_t> RHIElementSizes =
		{
			{RHIElementType::Int,		4},
			{RHIElementType::Int2,		4 * 2},
			{RHIElementType::Int3,		4 * 3},
			{RHIElementType::Int4,		4 * 4},
			{RHIElementType::Int10,		4 * 10},
			{RHIElementType::Bool,		1},
			{RHIElementType::Float,		4},
			{RHIElementType::Float2,	4 * 2},
			{RHIElementType::Float3,	4 * 3},
			{RHIElementType::Float4,	4 * 4},
			{RHIElementType::Float10,	4 * 10},
			{RHIElementType::Mat3,		4 * 3 * 3},
			{RHIElementType::Mat4,		4 * 4 * 4},
		};
		return RHIElementSizes.at(ElementType);
	}

	std::string GetUniformNameByTextureType(RHIMapTextureType TextureType)
	{
		static std::unordered_map<RHIMapTextureType, std::string> RHITextureUniformNames =
		{
			{RHIMapTextureType::BasicTexture2D,	"uTexture"},
			{RHIMapTextureType::DiffuseMap,			"uMaterial.TextureDiffuse"},
			{RHIMapTextureType::SpecularMap,			"uMaterial.TextureSpecular"},
			{RHIMapTextureType::NormalMap,			"uMaterial.TextureNormalMap"}
		};
		return RHITextureUniformNames[TextureType];
	}

	RHISkinnedVertex::RHISkinnedVertex(RHIVertex const& Vertex)
	{
		Position	= Vertex.Position;
		Tangent		= Vertex.Tangent;
		NormalMap	= Vertex.NormalMap;
		TexUV		= Vertex.TexUV;
	}

	RHISkinnedVertex& RHISkinnedVertex::operator=(RHIVertex const& Rhs)
	{
		Position	= Rhs.Position;
		Tangent		= Rhs.Tangent;
		NormalMap	= Rhs.NormalMap;
		TexUV		= Rhs.TexUV;

		return *this;
	}
}
