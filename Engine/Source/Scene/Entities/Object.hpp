#pragma once

#include "Core/CoreMinimal.hpp"

#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

namespace Engine
{
	class Object
	{
	private:
		glm::vec3 m_Location	= { 0,0,0 };
		glm::vec3 m_Rotation	= { 0,0,0 };
		glm::vec3 m_Scale		= { 0,0,0 };

	public:
		glm::vec3 const& GetLocation(void) const;
		glm::vec3& GetLocation(void);
		void SetLocation(glm::vec3 const& Location);
		
		glm::vec3 const& GetRotation(void) const;
		void SetRotation(glm::vec3 const& Rotation);

		glm::vec3 const& GetScale(void) const;
		void SetScale(glm::vec3 const& Scale);

		glm::mat4 GetModelMat(void) const;
	};

	FORCEINLINE glm::vec3 const& Object::GetLocation(void) const
	{
		return m_Location;
	}

	FORCEINLINE glm::vec3& Object::GetLocation(void) 
	{
		return m_Location;
	}

	FORCEINLINE void Object::SetLocation(glm::vec3 const& Location)
	{
		m_Location = Location;
	}

	FORCEINLINE glm::vec3 const& Object::GetRotation(void) const
	{
		return m_Rotation;
	}

	FORCEINLINE void Object::SetRotation(glm::vec3 const& Rotation)
	{
		m_Rotation = Rotation;
	}

	FORCEINLINE glm::vec3 const& Object::GetScale(void) const
	{
		return m_Scale;
	}

	FORCEINLINE void Object::SetScale(glm::vec3 const& Scale)
	{
		m_Scale = Scale;
	}

	FORCEINLINE glm::mat4 Object::GetModelMat(void) const
	{
		glm::mat4 modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, m_Location);
		modelMat = glm::rotate(modelMat, m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		modelMat = glm::rotate(modelMat, m_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMat = glm::rotate(modelMat, m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		return modelMat;
	}
}