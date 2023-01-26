#include "EnginePCH.hpp"

#include "Actor.hpp"
#include "Renderer/Renderer.hpp"

namespace Engine
{
	
	static void UpdateRendererStats(bool IncreaseActorCount)
	{
		RendererStatistics& rendererStats = Renderer::GetMutableRendererStats();
		IncreaseActorCount ? rendererStats.ActorsCount++ : rendererStats.ActorsCount--;
	}

	Actor::Actor(glm::vec3 const& WorldLocation)
		:	Object(),
			m_Location(WorldLocation)
	{
		UpdateRendererStats(true);
	}

	Actor::~Actor()
	{
		UpdateRendererStats(false);
	}

	glm::mat4 Actor::GetModelMat(void) const
	{
		glm::mat4 ModelMat = glm::mat4(1.0f);

		ModelMat = glm::translate(ModelMat, m_Location);

		ModelMat = glm::rotate(ModelMat, m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		ModelMat = glm::rotate(ModelMat, m_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		ModelMat = glm::rotate(ModelMat, m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		
		ModelMat = glm::scale(ModelMat, m_Scale);

		return ModelMat;
	}
}