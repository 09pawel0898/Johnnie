#pragma once

#include <Engine/Utilities.hpp>
#include <Engine/Scene.hpp>

DECLARE_MULTICAST_DELEGATE(OnStaticMeshToLoadPathSelected, std::string const&);
DECLARE_MULTICAST_DELEGATE(OnSkeletalMeshToLoadPathSelected, std::string const&);
DECLARE_MULTICAST_DELEGATE(OnAnimationToLoadPathSelected, std::string const&);

class JohnnieDelegates : public Singleton<JohnnieDelegates>
{
public:
	OnStaticMeshToLoadPathSelected		OnStaticMeshToLoadPathSelected;

	OnSkeletalMeshToLoadPathSelected	OnSkeletalMeshToLoadPathSelected;
	
	OnAnimationToLoadPathSelected		OnAnimationToLoadPathSelected;
};