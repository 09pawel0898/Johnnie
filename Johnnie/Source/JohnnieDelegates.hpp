#pragma once

#include <Engine/Utilities.hpp>
#include <Engine/Scene.hpp>

DECLARE_MULTICAST_DELEGATE(OnBeginLoadingNewModel, std::string const&);

class JohnnieDelegates : public Singleton<JohnnieDelegates>
{
public:
	OnBeginLoadingNewModel OnBeginLoadingNewModel;
};