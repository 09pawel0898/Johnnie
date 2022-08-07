#pragma once

#include <string>
#include <memory>
#include <variant>

#include <glm/glm.hpp>

#include "RHIResource.hpp"

namespace Engine::RHI
{
	enum class RHIShaderType : uint8_t
	{
		VERTEX = 0, 
		FRAGMENT
	};

	using SingleSourceView = std::string_view;
	using DoubleSourceView = std::pair<std::string_view, std::string_view>;

	struct RHIShaderView
	{
		std::string_view ShaderName;
		std::string_view Filename;
		std::variant<SingleSourceView,DoubleSourceView> Source;

		explicit RHIShaderView(std::string_view ShaderName)
			:	ShaderName(ShaderName)
		{}
	};

	class RHIShader : public RHIResource
	{
	protected:
		RHIShaderView m_ShaderView;

	public:
		explicit RHIShader(std::string_view ShaderName);
		virtual ~RHIShader() = default;
	
	public:
		[[nodiscard]] static std::unique_ptr<RHIShader> Create(std::string_view Name, std::string_view FilePath);
		[[nodiscard]] static std::unique_ptr<RHIShader> Create(std::string_view Name, std::string_view VertShaderFilePath, std::string_view FragShaderFilePath);

		virtual void Bind(void) const = 0;
		virtual void Unbind(void) const = 0;

		RHIShaderView const& GetShaderView(void);

		/** Data */

		virtual void SetFloat(std::string_view Name, float Value) = 0;
		virtual void SetFloat2(std::string_view Name, const glm::vec2& Value) = 0;
		virtual void SetFloat3(std::string_view Name, const glm::vec3& Value) = 0;
		virtual void SetFloat4(std::string_view Name, const glm::vec4& Value) = 0;
		virtual void SetMat4(std::string_view Name, const glm::mat4& Value) = 0;
		virtual void SetInt(std::string_view Name, int32_t Value) = 0;
		virtual void SetIntArray(std::string_view Name, int32_t* Values, uint32_t Count) = 0;
	};
}