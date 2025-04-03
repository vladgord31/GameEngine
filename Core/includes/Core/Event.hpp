#pragma once

#include <functional>
#include <array>

namespace Engine
{
	enum class EventType
	{
		WindowResize = 0,
		WindowClose,

		KeyPressed,
		KeyReleased,

		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,


		EventsCount,
	};

	struct BaseEvent
	{
		virtual ~BaseEvent() = default;
		virtual EventType get_type() const = 0;
	};

	class EventDispatcher
	{
	public:
		template <typename EventType>
		void add_event_listener(std::function<void(EventType&)> callback)
		{
			auto baseCallbak = [func = std::move(callback)](BaseEvent& be)
			{
				if (be.get_type() == EventType::m_type)
				{
					func(static_cast<EventType&>(be));
				}
			};
			m_eventCallbacks[static_cast<size_t>(EventType::m_type)] = std::move(baseCallbak);
		}

		void dispatch(BaseEvent& event)
		{
			auto& callback = m_eventCallbacks[static_cast<size_t>(event.get_type())];
			if (callback)
			{
				callback(event);
			}
		}

	private:
		std::array<std::function<void(BaseEvent&)>, static_cast<size_t>(EventType::EventsCount)> m_eventCallbacks;
	};

	struct EventMouseMoved : public BaseEvent
	{
		EventMouseMoved(const double x, const double y)
			: m_x(x), m_y(y)
		{
		}

		virtual EventType get_type() const override
		{
			return m_type;
		}

		double m_x;
		double m_y;

		static const EventType m_type = EventType::MouseMoved;
	};

	struct EventWindowResize : public BaseEvent
	{
		EventWindowResize(unsigned int width, unsigned int height)
			: m_width(width), m_height(height)
		{
		}

		virtual EventType get_type() const override
		{
			return m_type;
		}

		unsigned int m_width;
		unsigned int m_height;

		static const EventType m_type = EventType::WindowResize;
	};

	struct EventWindowClose : public BaseEvent
	{
		virtual EventType get_type() const override
		{
			return m_type;
		}

		static const EventType m_type = EventType::WindowClose;
	};

}
