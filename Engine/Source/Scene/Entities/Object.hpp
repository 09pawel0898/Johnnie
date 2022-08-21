#pragma once

#include "Core/CoreMinimal.hpp"

#include <glm/glm.hpp>

namespace Engine
{
	class Object
	{
	private:
		glm::vec3 m_Location;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;

	public:
		glm::vec3 const& GetLocation(void) const;
		void SetLocation(glm::vec3 const& Location);
		
		glm::vec3 const& GetRotation(void) const;
		void SetRotation(glm::vec3 const& Rotation);

		glm::vec3 const& GetScale(void) const;
		void SetScale(glm::vec3 const& Scale);
	};

	FORCEINLINE glm::vec3 const& Object::GetLocation(void) const
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

	

	//template <typename ObjectType>
	//requires InstanceOf<
	//FORCEINLINE
}