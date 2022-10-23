#include "EnginePCH.hpp"

#include "LayerManager.hpp"

namespace Engine
{
	namespace Utility
	{
		static decltype(auto) FindLayerByName(std::vector<TSharedPtr<Layer>>& LayersContainer, LayerManager::LayerName LayerName)
		{
			return std::find_if(LayersContainer.begin(), LayersContainer.end(), [LayerName](TSharedPtr<Layer> const& LayerPointer)
			{
				return LayerName == LayerPointer->GetName();
			});
		}
	}

	LayerManager::~LayerManager()
	{
		for (auto& layer : m_Layers)
		{
			layer->OnDetach();
		}
	}

	void LayerManager::PushLayer(TSharedPtr<Layer> Layer)
	{
		Layer->OnAwake();
		m_Layers.emplace(m_Layers.begin() + m_InsertIndex, MoveTemp(Layer));
	}

	void LayerManager::PushOverlay(TSharedPtr<Layer> Layer)
	{
		Layer->OnAwake();
		m_Layers.emplace_back(MoveTemp(Layer));
	}

	void LayerManager::RemoveLayer(std::string_view LayerName)
	{
		auto foundLayer = Utility::FindLayerByName(m_Layers, LayerName);

		if (foundLayer != m_Layers.begin() + m_InsertIndex)
		{
			foundLayer->get()->OnDetach();
			m_Layers.erase(foundLayer);
			m_InsertIndex--;
		}
	}

	void LayerManager::RemoveOverlay(std::string_view LayerName)
	{
		auto iter = std::find_if(m_Layers.begin() + m_InsertIndex, m_Layers.begin(), 
		[LayerName](TSharedPtr<Layer> const& LayerPointer)
		{
			return LayerName == LayerPointer->GetName();
		});

		if (iter != m_Layers.end())
		{
			iter->get()->OnDetach();
			m_Layers.erase(iter);
		}
	}

	TSharedPtr<Layer>& LayerManager::GetLayer(LayerName LayerName)
	{
		auto foundLayer = Utility::FindLayerByName(m_Layers, LayerName);

		CheckMsg(*foundLayer != nullptr, "Requested layer was not present in the container.");
		return *foundLayer;
	}

	void LayerManager::Clear(void)
	{
		while (!IsEmpty())
		{
			(--m_Layers.end())->get()->OnDetach();
			m_Layers.pop_back();
		}
	}
}