#pragma once

#include "Core/CoreMinimal.hpp"

#include "Object.hpp"
#include "IDrawable.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	class Actor : public Object, public IDrawable
	{
	private:
		glm::vec3 m_Location	= { 0.f,0.f,0.f };
		glm::vec3 m_Rotation	= { 0.f,0.f,0.f };
		glm::vec3 m_Scale		= { 1.f,1.f,1.f };

	public:
		explicit Actor(glm::vec3 const& WorldLocation = { 0.f,0.f,0.f });

		glm::vec3 const& GetLocation(void) const;
		glm::vec3& GetLocation(void);
		void SetLocation(glm::vec3 const& Location);
		
		glm::vec3 const& GetRotation(void) const;
		void SetRotation(glm::vec3 const& Rotation);

		glm::vec3 const& GetScale(void) const;
		void SetScale(glm::vec3 const& Scale);

		glm::mat4 GetModelMat(void) const;

	private:
		/** Rendering */
		bool m_bVisible = true;

	public:
		virtual void OnTick(double DeltaTime) override {};
		virtual void Draw(void) const override {};

	public:
		virtual void SetVisibility(bool Visible);
		bool IsVisible(void) const override;
	};

	FORCEINLINE glm::vec3 const& Actor::GetLocation(void) const
	{
		return m_Location;
	}

	FORCEINLINE glm::vec3& Actor::GetLocation(void)
	{
		return m_Location;
	}

	FORCEINLINE void Actor::SetLocation(glm::vec3 const& Location)
	{
		m_Location = Location;
	}

	FORCEINLINE glm::vec3 const& Actor::GetRotation(void) const
	{
		return m_Rotation;
	}

	FORCEINLINE void Actor::SetRotation(glm::vec3 const& Rotation)
	{
		m_Rotation = Rotation;
	}

	FORCEINLINE glm::vec3 const& Actor::GetScale(void) const
	{
		return m_Scale;
	}

	FORCEINLINE void Actor::SetScale(glm::vec3 const& Scale)
	{
		m_Scale = Scale;
	}

	FORCEINLINE void Actor::SetVisibility(bool Visible)
	{
		m_bVisible = Visible;
	}

	FORCEINLINE bool Actor::IsVisible(void) const
	{
		return m_bVisible;
	}
}