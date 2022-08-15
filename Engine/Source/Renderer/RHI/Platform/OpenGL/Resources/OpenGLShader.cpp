#include "OpenGLShader.hpp"

#include <fstream>
#include <glad/glad.h>

#include "Log/Log.hpp"
#include "Core/Debug/Asserts.hpp"
#include "Utilities/VariantUtility.hpp"
#include "Utilities/FileUtility.hpp"

#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;

namespace Engine::RHI
{
    static std::string_view GLShaderToStringV(int32_t Type)
    {
        if (Type == GL_VERTEX_SHADER)
            return "vertex";
        if (Type == GL_FRAGMENT_SHADER)
            return "fragment";
        return "";
    }

    std::tuple<std::string_view, std::string_view> OpenGLShader::ParseGLSLShader(std::string_view Filepath)
    {
        std::cout << "Current path is: " << fs::current_path() << '\n';
        std::cout << "Current root path is: " << fs::current_path().root_path() << '\n';

        std::ifstream file(Filepath.data());
        
        if (!file.good())
        {
            LOG(RHI, Error, "Failed to open shader file named {0}", Filepath);
            Check(false);
        }

        RHIShaderType type;
        std::stringstream parsedShaders[2];
        std::string line;

        while (getline(file, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                    type = RHIShaderType::VERTEX;
                else if (line.find("fragment") != std::string::npos)
                    type = RHIShaderType::FRAGMENT;
            }
            parsedShaders[static_cast<int32_t>(type)] << line << '\n';
        }

        return std::make_tuple( parsedShaders[static_cast<int32_t>(RHIShaderType::VERTEX)].str(),
                                parsedShaders[static_cast<int32_t>(RHIShaderType::FRAGMENT)].str());
    }

    GLuint OpenGLShader::CreateOpenGLShader(std::string_view VertexShader, std::string_view FragmentShader)
    {
        GLuint shaderProgram = glCreateProgram();

        GLuint vertShader = CompileGLSLShader(VertexShader, GL_VERTEX_SHADER);
        GLuint fragShader = CompileGLSLShader(FragmentShader, GL_FRAGMENT_SHADER);

        glAttachShader(shaderProgram, vertShader);
        glAttachShader(shaderProgram, fragShader);
        glLinkProgram(shaderProgram);
        glValidateProgram(shaderProgram);

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);

        return shaderProgram;
    }

    GLuint OpenGLShader::CompileGLSLShader(std::string_view Source, uint32_t Type)
    {
        GLuint ID = glCreateShader(Type);
        int32_t compilationResult;
        const char* src = Source.data();

        glShaderSource(ID, 1, &src, nullptr);
        glCompileShader(ID);

        auto ersureCompilationSucceded = [&]()
        {
            glGetShaderiv(ID, GL_COMPILE_STATUS, &compilationResult);
            
            if (compilationResult == GL_FALSE)
            {
                int lenght;
                glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &lenght);
                char* msg = (char*)_malloca(lenght * sizeof(char));
                glGetShaderInfoLog(ID, lenght, &lenght, msg);

                std::string_view sourceFileName = GetSourceFileName(Type);

                LOG(RHI, Error, "Failed to compile {0} shader!\nMessage : {1}\nFilename : {2}", GLShaderToStringV(Type), msg, sourceFileName);
                glDeleteShader(ID);
                _freea(msg);
                Check(false);
            }
        };
        ersureCompilationSucceded();
        return (compilationResult == GL_FALSE) ? -1 : ID;
    }

	OpenGLShader::OpenGLShader(std::string_view ShaderName, std::string_view Filepath)
        :   RHIShader(ShaderName), 
            m_ID(0)
	{
        m_ShaderView.Source = Filepath;

        auto [vertexShaderSource, fragmentShaderSource] = ParseGLSLShader(Filepath);
        m_ID = CreateOpenGLShader(vertexShaderSource,fragmentShaderSource);
	}

    OpenGLShader::OpenGLShader(std::string_view ShaderName, std::string_view VertFilepath, std::string_view FragFilepath)
        :   RHIShader(ShaderName), 
            m_ID(0)
    {
        m_ShaderView.Source = std::make_pair(VertFilepath, FragFilepath);

        auto vertexShaderSource = Utility::ReadFile(VertFilepath);
        if (!vertexShaderSource.has_value())
        {
            LOG(RHI, Warning, "Failed to open shader file named {0}", VertFilepath);
            Check(false);
        }
        auto fragmentShaderSource = Utility::ReadFile(FragFilepath);
        if (!fragmentShaderSource.has_value())
        {
            LOG(RHI, Warning, "Failed to open shader file named {0}", FragFilepath);
            Check(false);
        }

        m_ID = CreateOpenGLShader(vertexShaderSource.value(), fragmentShaderSource.value());
    }

    OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ID);
	}

    void OpenGLShader::Bind(void) const
    {
        glUseProgram(m_ID);
    }

    void OpenGLShader::Unbind(void) const
    {
        glUseProgram(0);
    }

	std::string_view OpenGLShader::GetSourceFileName(int32_t Type) const
	{
        std::string_view outFileName = "NONE";

        auto filenameVisitor = MakeInlineVisitor
        (
            [&outFileName](SingleSourceView singleSourceView)
            {
                outFileName = singleSourceView;
            },
            [Type, &outFileName](DoubleSourceView doubleSourceView)
            {
                auto [vertShaderFileName, fragShaderFileName] = doubleSourceView;
                if (Type == GL_VERTEX_SHADER)
                {
                    outFileName = vertShaderFileName;
                }
                else if (Type == GL_FRAGMENT_SHADER)
                {
                    outFileName = fragShaderFileName;
                }
            }
        );
        std::visit(filenameVisitor, m_ShaderView.Source);
        return outFileName;
	}

    void OpenGLShader::SetFloat(std::string_view Name, float Value)
    {
        GLint location = glGetUniformLocation(m_ID, Name.data());
        glUniform1f(location, Value);
    }

    void OpenGLShader::SetFloat2(std::string_view Name, const glm::vec2& Value)
    {
        GLint location = glGetUniformLocation(m_ID, Name.data());
        glUniform2f(location, Value.x, Value.y);
    }

    void OpenGLShader::SetFloat3(std::string_view Name, const glm::vec3& Value)
    {
        GLint location = glGetUniformLocation(m_ID, Name.data());
        glUniform3f(location, Value.x, Value.y, Value.z);
    }

    void OpenGLShader::SetFloat4(std::string_view Name, const glm::vec4& Value)
    {
        GLint location = glGetUniformLocation(m_ID, Name.data());
        glUniform4f(location, Value.x, Value.y, Value.z, Value.w);
    }

    void OpenGLShader::SetMat4(std::string_view Name, const glm::mat4& Value)
    {
        GLint location = glGetUniformLocation(m_ID, Name.data());
        glUniformMatrix4fv(location, 1, GL_FALSE, &Value[0][0]);
    }

    void OpenGLShader::SetInt(std::string_view Name, int32_t Value)
    {
        GLint location = glGetUniformLocation(m_ID, Name.data());
        glUniform1i(location, Value);
    }

    void OpenGLShader::SetIntArray(std::string_view Name, int32_t* Values, uint32_t Count)
    {
        GLint location = glGetUniformLocation(m_ID, Name.data());
        glUniform1iv(location, Count, Values);
    }
}