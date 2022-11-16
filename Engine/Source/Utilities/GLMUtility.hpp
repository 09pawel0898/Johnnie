#pragma once

#include "Core/CoreMinimal.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "assimp/scene.h"

namespace Engine::Utility
{
    FORCEINLINE glm::vec3 AiVec3ToGlmVec3(const aiVector3D& Vector) 
    { 
        return glm::vec3(Vector.x, Vector.y, Vector.z);
    }

    FORCEINLINE glm::quat AiQuatToGlmQuat(const aiQuaternion& Quat) 
    { 
        return glm::quat(Quat.w, Quat.x, Quat.y, Quat.z);
    }

    FORCEINLINE glm::mat4 AtMat4ToGlmMat4(const aiMatrix4x4& Matrix)
    { 
        return glm::transpose(glm::make_mat4(&Matrix.a1));
    }

    FORCEINLINE glm::mat4 AtMat3ToGlmMat3(const aiMatrix3x3& Matrix) 
    { 
        return glm::transpose(glm::make_mat3(&Matrix.a1)); 
    }

	FORCEINLINE glm::mat4 AiMat4ToGlmMat4(aiMatrix4x4 const& Matrix)
	{
        glm::mat4 Result;

        Result[0][0] = Matrix.a1; Result[0][1] = Matrix.b1;  Result[0][2] = Matrix.c1; Result[0][3] = Matrix.d1;
        Result[1][0] = Matrix.a2; Result[1][1] = Matrix.b2;  Result[1][2] = Matrix.c2; Result[1][3] = Matrix.d2;
        Result[2][0] = Matrix.a3; Result[2][1] = Matrix.b3;  Result[2][2] = Matrix.c3; Result[2][3] = Matrix.d3;
        Result[3][0] = Matrix.a4; Result[3][1] = Matrix.b4;  Result[3][2] = Matrix.c4; Result[3][3] = Matrix.d4;

        return Result;
	}
    
    FORCEINLINE glm::mat4 AiMat3ToGlmMat4(aiMatrix3x3 const& Matrix)
	{
        glm::mat4 Result = glm::mat4(1);

        Result[0][0] = Matrix.a1; Result[0][1] = Matrix.b1;  Result[0][2] = Matrix.c1;
        Result[1][0] = Matrix.a2; Result[1][1] = Matrix.b2;  Result[1][2] = Matrix.c2;
        Result[2][0] = Matrix.a3; Result[2][1] = Matrix.b3;  Result[2][2] = Matrix.c3;

        return Result;
	}
}