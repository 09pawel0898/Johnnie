#include "EnginePCH.hpp"
#include "SkeletalMeshHelpers.hpp"

#include "Log/Log.hpp"
#include <assimp/scene.h>
#include <glm/gtc/quaternion.hpp>

namespace Engine
{
    void VertexBoneInfluenceData::AddBoneData(uint32_t GlobalVertexID, uint32_t BoneID, float Weight)
    {
        for (uint8_t idx = 0; idx < Index; idx++)
        {
            // Sanity check because importing with aiProcess_JoinIdenticalVertices produces duplicates of aiVertexWeight
            if (BoneIDs[idx] == BoneID)
            {
                return;
            }
        }

        if (Weight == 0.f)
        {
            return;
        }
        
        if (Index > 3)
        {
            return;
        }

        //CheckMsg(Index != s_MaxBonesPerVertex, "Program not supports so many vertices influenced by one bone.");

        BoneIDs[Index] = BoneID;
        Weights[Index] = Weight;

#ifdef DEBUG_MODEL_IMPORTER
        //LOG(Assimp, Trace, "GlobalVertexID {0} BoneID {1} Weight {2} InfoSlotIndex {3}", GlobalVertexID, BoneIDs[Index], Weights[Index], Index);
#endif
        Index++;
    }

    BoneData::BoneData(glm::mat4&& OffsetMatrix) noexcept
        :   OffsetMatrix(MoveTemp(OffsetMatrix))
    {}
    
    NodeData* SkeletonData::FindNodeByName(std::string const& Name)
    {
        return FindNodeByName_Internal(Name, &RootNode);
    }
    
    NodeData* SkeletonData::FindNodeByName_Internal(std::string const& Name, NodeData* Node)
    {
        if (Node->Name == Name)
        {
            return Node;
        }

        for (int16_t idx = 0; idx < Node->ChildrenCount; idx++)
        {
            NodeData* FoundInChild = FindNodeByName_Internal(Name, &Node->Children[idx]);
            if (FoundInChild)
            {
                return FoundInChild;
            }
        }
        return nullptr;
    }
}