#pragma once

#include "Entities/Primitives/Tickable.hpp"

namespace Engine
{
	class Scene
	{
	public:
		Scene() = default;
		virtual ~Scene();

		Scene(Scene const&) = delete;
		Scene& operator=(Scene const&) = delete;

		Scene(Scene&&) = delete;
		Scene& operator=(Scene&&) = delete;

	public:
		void Load(void);
	};
}