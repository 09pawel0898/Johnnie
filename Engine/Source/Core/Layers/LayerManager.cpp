#include "EnginePCH.hpp"

#include "LayerManager.hpp"

namespace Engine
{
	LayerManager::~LayerManager()
	{
		for (auto& layer : m_Layers)
		{
			layer->OnDetach();
		}
	}

	void LayerManager::PushLayer(LayerPointer Layer)
	{
		Layer->OnAwake();
		m_Layers.emplace(m_Layers.begin() + m_InsertIndex, std::move(Layer));
	}

	void LayerManager::PushOverlay(LayerPointer Layer)
	{
		Layer->OnAwake();
		m_Layers.emplace_back(std::move(Layer));
	}

	void LayerManager::RemoveLayer(std::string_view LayerName)
	{
		auto iter = std::find_if(m_Layers.begin(), m_Layers.begin(), 
		[LayerName](LayerPointer const& LayerPointer)
		{
			return LayerName == LayerPointer->GetName();
		});

		if (iter != m_Layers.begin() + m_InsertIndex)
		{
			iter->get()->OnDetach();
			m_Layers.erase(iter);
			m_InsertIndex--;
		}
	}

	void LayerManager::RemoveOverlay(std::string_view LayerName)
	{
		auto iter = std::find_if(m_Layers.begin() + m_InsertIndex, m_Layers.begin(), 
		[LayerName](LayerPointer const& LayerPointer)
		{
			return LayerName == LayerPointer->GetName();
		});

		if (iter != m_Layers.end())
		{
			iter->get()->OnDetach();
			m_Layers.erase(iter);
		}
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