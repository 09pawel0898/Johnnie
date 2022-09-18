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

    template <typename TDrawable>
    static void Draw_Internal(std::vector<std::pair<OUUID,std::weak_ptr<TDrawable>>> const& DrawableObjects, std::vector<OUUID>& OutPendingToUnregister)
    {
        std::vector<OUUID> pendingDrawablesToUnregister;

        for (auto const& [uuid, drawablePtr] : DrawableObjects)
        {
            if (auto drawable = drawablePtr.lock())
            {
                if (drawable->IsVisible())
                {
                    if constexpr(std::is_same_v<TDrawable,IDrawable>)
                    {
                        drawable->Draw();
                    }
                    else if constexpr(std::is_same_v<TDrawable,IDrawableWidget>)
                    {
                        drawable->OnRenderGui();
                    }
                }
            }
            else
            {
                pendingDrawablesToUnregister.emplace_back(uuid);
            }
        }
    }

    void DrawableManager::DrawActors(void)
    {
        std::vector<OUUID> pendingDrawablesToUnregister;
        
        Draw_Internal(m_DrawableObjects, pendingDrawablesToUnregister);
        if (pendingDrawablesToUnregister.size() != 0)
        {
            UnRegisterDrawables(DrawableType::Widget, pendingDrawablesToUnregister);
        }
    }

    void DrawableManager::DrawWidgets(void)
    {
        std::vector<OUUID> pendingDrawablesToUnregister;

        Draw_Internal(m_DrawableWidgets, pendingDrawablesToUnregister);
        if(pendingDrawablesToUnregister.size() != 0)
        {
            UnRegisterDrawables(DrawableType::Widget, pendingDrawablesToUnregister);
        }
    }

    void DrawableManager::UnRegisterDrawables(DrawableType Type, std::vector<OUUID> const& PendingIDsToUnregister)
    {
        auto eraseFromContainerByID = 
        []<typename TDrawable>(OUUID const& UUID, std::vector<std::pair<OUUID, std::weak_ptr<TDrawable>>>& DrawableObjects)
        {
            auto const& foundElement = std::find_if(DrawableObjects.cbegin(), DrawableObjects.cend(),
                [&UUID](std::pair<OUUID, std::weak_ptr<TDrawable>> const& Element)
                {
                    return Element.first == UUID;
                });
            DrawableObjects.erase(foundElement);
        };

        for (auto const& uuid : PendingIDsToUnregister)
        {
            if (Type == DrawableType::Actor)
            {
                eraseFromContainerByID(uuid, m_DrawableObjects);
            }
            else if (Type == DrawableType::Widget)
            {
                eraseFromContainerByID(uuid, m_DrawableWidgets);
            }
        }
    }
}