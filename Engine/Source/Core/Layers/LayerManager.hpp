#pragma once

#include "Layer.hpp"

namespace Engine
{
	using LayerPointer = std::shared_ptr<Layer>;

	class LayerManager final
	{
	public:
		using LayerName = std::string_view;

	public:
		LayerManager() noexcept
		{}

		~LayerManager();

		LayerManager(const LayerManager& rhs) = delete;
		LayerManager(LayerManager&& rhs) = delete;

		LayerManager& operator = (const LayerManager& rhs) = delete;
		LayerManager& operator = (LayerManager&& rhs) = delete;

	private:
		uint8_t m_InsertIndex = 0;
		std::vector<LayerPointer> m_Layers;
		
	public:	
		void PushLayer(LayerPointer Layer);
		void PushOverlay(LayerPointer Layer);
		void RemoveLayer(std::string_view LayerName);
		void RemoveOverlay(std::string_view LayerName);

		inline bool IsEmpty(void) const
		{
			return m_Layers.empty();
		}

		void Clear(void);

	public:
		using Iterator		= std::vector<LayerPointer>::iterator;
		using ConstIterator = std::vector<LayerPointer>::const_iterator;
		
		Iterator		begin()			{ return m_Layers.begin();	}
		Iterator		end()			{ return m_Layers.end();	}
		ConstIterator	begin() const	{ return m_Layers.begin();	}
		ConstIterator	end()	const	{ return m_Layers.end();	}
	};
}