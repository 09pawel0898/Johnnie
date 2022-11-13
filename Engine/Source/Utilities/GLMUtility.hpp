#pragma once

#include "Core/CoreMinimal.hpp"

#include <glm/glm.hpp>
#include "assimp/scene.h"

namespace Engine::Utility
{
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
    
    FORCEINLINE glm::mat4 AiQuatToGlmRotationMatrix(aiQuaternion const& Quat)
	{
        glm::mat4 Result = glm::mat4(1);

        Result[0][0] = 2 * (Quat.x * Quat.x + Quat.y * Quat.y) - 1; 
        Result[0][1] = 2 * (Quat.y * Quat.z - Quat.x * Quat.w);
        Result[0][2] = 2 * (Quat.y * Quat.w + Quat.x * Quat.z);

        Result[1][0] = 2 * (Quat.y * Quat.z + Quat.x * Quat.w);
        Result[1][1] = 2 * (Quat.x * Quat.x + Quat.z * Quat.z) - 1;
        Result[1][2] = 2 * (Quat.z * Quat.w - Quat.x * Quat.y);

        Result[2][0] = 2 * (Quat.y * Quat.w - Quat.x * Quat.z);
        Result[2][1] = 2 * (Quat.z * Quat.w + Quat.x * Quat.y);
        Result[2][2] = 2 * (Quat.x * Quat.x + Quat.w * Quat.w) - 1;

        return Result;
	}
}