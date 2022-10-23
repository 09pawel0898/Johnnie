#pragma once

#include "Layer.hpp"

namespace Engine
{
	class LayerManager final
	{
	public:
		using LayerName = std::string_view;

	public:
		LayerManager() = default;

		~LayerManager();

		LayerManager(const LayerManager& rhs) = delete;
		LayerManager(LayerManager&& rhs) = delete;

		LayerManager& operator = (const LayerManager& rhs) = delete;
		LayerManager& operator = (LayerManager&& rhs) = delete;

	private:
		uint8_t m_InsertIndex = 0;
		std::vector<TSharedPtr<Layer>> m_Layers;
		
	public:	
		void PushLayer		(TSharedPtr<Layer> Layer);
		void PushOverlay	(TSharedPtr<Layer> Layer);
		void RemoveLayer	(std::string_view LayerName);
		void RemoveOverlay	(std::string_view LayerName);

		TSharedPtr<Layer>& GetLayer(LayerName LayerName);

		bool IsEmpty(void) const
		{
			return m_Layers.empty();
		}
		void Clear(void);

	public:
		using Iterator		= std::vector<TSharedPtr<Layer>>::iterator;
		using ConstIterator = std::vector<TSharedPtr<Layer>>::const_iterator;
		
		Iterator		begin()			{ return m_Layers.begin();	}
		Iterator		end()			{ return m_Layers.end();	}
		ConstIterator	begin() const	{ return m_Layers.begin();	}
		ConstIterator	end()	const	{ return m_Layers.end();	}
	};
}