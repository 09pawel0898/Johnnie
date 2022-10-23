#pragma once

#include "../ImGui/ImGuiProperties.hpp"
#include "RHI/DynamicRHI.hpp"
#include "Utilities/Singleton.hpp"

namespace Engine
{
	class ImGuiRenderer final : public MSingleton<ImGuiRenderer>
	{
	private:
		ImGuiProperties m_ImGuiProperties{};

	public:
		static void Init(RenderingAPI RenderingAPI);

		static void Shutdown(void);

		void BeginFrame(void);
		void EndFrame(void);
	
	private:
		void InitTheme(void);

	public:
		ImGuiProperties& GetImGuiProperties(void)			
		{ 
			return m_ImGuiProperties;	
		}

		ImGuiProperties const& GetImGuiProperties() const	
		{ 
			return m_ImGuiProperties; 
		}
	};
}
