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

        Index++;
    }

    BoneData::BoneData(glm::mat4&& OffsetMatrix) noexcept
        :   OffsetMatrix(MoveTemp(OffsetMatrix))
    {}
    
    NodeData* Skeleton::FindNodeByName(std::string const& Name)
    {
        return FindNodeByName_Internal(Name, &m_RootNode);
    }

    const NodeData* Skeleton::FindNodeByName(std::string const& Name) const
    {
        return FindNodeByName_Internal(Name, const_cast<NodeData*>(&m_RootNode));
    }

    void Skeleton::GetRootBone(NodeData** OutResult) const
    {
        return GetRootBone_Internal(&m_RootNode, OutResult);
    }

    void Skeleton::GetRootBone_Internal(const NodeData* Node, NodeData** OutResult) const
    {
        if (*OutResult != nullptr)
        {
            return;
        }

        auto IsBone = [this](std::string const& NodeName) -> bool
        {
            return (m_BoneNameIndexMap.find(NodeName) != m_BoneNameIndexMap.end()) ? true : false;
        };

        const std::string NodeName = Node->Name;

        if (IsBone(NodeName))
        {
            if (Node->Parent == nullptr)
            {
                return;
            }
            ;
            if (!IsBone(Node->Parent->Name))
            {
                *OutResult = const_cast<NodeData*>(Node);
            }
        }

        for (uint16_t idx = 0; idx < Node->ChildrenCount; idx++)
        {
            GetRootBone_Internal(&Node->Children[idx], OutResult);
        }
    }
    
    NodeData* Skeleton::FindNodeByName_Internal(std::string const& Name, NodeData* Node) const
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