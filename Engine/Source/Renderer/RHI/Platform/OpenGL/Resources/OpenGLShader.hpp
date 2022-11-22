#pragma once

#include "Core/CoreMinimal.hpp"
#include "../../../Resources/RHIShader.hpp"

namespace Engine::RHI
{
	class OpenGLShader : public RHIShader
	{
	private:
		uint32_t m_ID;

	public:
		OpenGLShader(std::string_view Name, std::string_view Filepath);
		OpenGLShader(std::string_view Name, std::string_view VertFilepath, std::string_view FragFilepath);

		~OpenGLShader();

		virtual void Bind(void) const override;
		virtual void Unbind(void) const override;

	private:
		/** Parses Vertex and Fragment shaders from one file. */
		std::tuple<std::string, std::string> ParseGLSLShader(std::string_view Filepath);
		
		/** Compiles GLSL Shader */
		uint32_t CompileGLSLShader(std::string_view Source, uint32_t Type);

		/** Creates OpenGL Shader */
		uint32_t CreateOpenGLShader(std::string_view VertexShader, std::string_view FragmentShader);

		/** Returns source file name */
		std::string_view GetSourceFileName(int32_t Type) const;

	public:
		/* Uniforms */

		virtual void SetFloat(std::string_view Name, float Value) override;
		virtual void SetFloat2(std::string_view Name, glm::vec2 const& Value) override;
		virtual void SetFloat3(std::string_view Name, glm::vec3 const& Value) override;
		virtual void SetFloat4(std::string_view Name, glm::vec4 const& Value) override;
		virtual void SetMat4(std::string_view Name, glm::mat4 const& Value) override;
		virtual void SetMat4Array(std::string_view Name, const glm::mat4* Values, uint32_t Count) override;
		virtual void SetMat3(std::string_view Name, glm::mat3 const& Value) override;
		virtual void SetInt(std::string_view Name, int32_t Value) override;
		virtual void SetIntArray(std::string_view Name, int32_t* Values, uint32_t Count) override;
	};
}