#include "EnginePCH.hpp"

#include "DrawableManager.hpp"

namespace Engine
{
    void DrawableManager::RegisterActor(std::shared_ptr<Actor> const& DrawableActor)
    {
        Check(  m_DrawableObjects.cend() == 
                std::find_if(m_DrawableObjects.cbegin(), m_DrawableObjects.cend(),
                [&DrawableActor](std::pair<OUUID, std::weak_ptr<IDrawable>> const& Element) -> bool 
                {
                    return DrawableActor->GetUUID() == Element.first; 
                }));

        m_DrawableObjects.emplace_back(
            std::make_pair(DrawableActor->GetUUID(), std::weak_ptr<IDrawable>(DrawableActor)));
    }

    void DrawableManager::DrawActors(void)
    {
        for (auto const& Object : m_DrawableObjects)
        {
            if (auto drawable = Object.second.lock())
            {
                if (drawable->IsVisible())
                {
                    drawable->Draw();
                }
            }
        }
    }

    void DrawableManager::DrawWidgets(void)
    {
        for (auto const& Widget : m_DrawableWidgets)
        {
            if (auto drawable = Widget.second.lock())
            {
                if (drawable->IsVisible())
                {
                    drawable->OnRenderGui();
                }
            }
        }
    }
}