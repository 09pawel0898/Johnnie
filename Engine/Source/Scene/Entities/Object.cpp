#include "EnginePCH.hpp"

#include "Object.hpp"

namespace Engine
{
	glm::mat4 Object::GetModelMat(void) const
	{
		glm::mat4 modelMat = glm::mat4(1.0f);

		modelMat = glm::translate(modelMat, m_Location);

		modelMat = glm::rotate(modelMat, m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		modelMat = glm::rotate(modelMat, m_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMat = glm::rotate(modelMat, m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

		modelMat = glm::scale(modelMat, m_Scale);

		return modelMat;
	}
}