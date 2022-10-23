#include "EnginePCH.hpp"
#include "Material.hpp"

namespace Engine
{
	namespace DefaultMaterials
	{
		const TSharedPtr<Material> BasicWhite = MakeShared<Material>();

		const TSharedPtr<Material> Emerald = MakeShared<Material>(
			MaterialUniform
			(	{ 0.07568,	0.61424,	0.07568 },
				{ 0.633f,	0.727f,		0.633f }, 
				76.8f
			));
		
		const TSharedPtr<Material> Gold = MakeShared<Material>(
			MaterialUniform
			(	{ 0.75164,	0.60648,	0.22648 },
				{ 0.628281,	0.555802,	0.366065 },
				51.2f
			));
		
		const TSharedPtr<Material> Silver = MakeShared<Material>(
			MaterialUniform
			(	{ 0.50754,	0.50754,	0.50754 },
				{ 0.508273, 0.508273,	0.508273 },
				51.2f
			));
	}
}