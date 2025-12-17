#pragma once

#include "Handle.hpp"
#include "HandleTypeRegistry.hpp"
#include "IEvent.hpp"
#include "SubscriberCallbackInfo.hpp"
#include "helper/CastHelpers.hpp"
#include "core/logging/LoggingFactory.hpp"

#include <absl/container/flat_hash_map.h>

#include <cassert>
#include <functional>
#include <vector>

namespace star::common
{

template <typename T>
concept THasGetType = requires(const T handle) {
    { handle.getType() } -> std::same_as<uint16_t>;
};
class EventBus
{
  public:
    void subscribe(const uint16_t &eventHandleType, SubscriberCallbackInfo callbackInfo)
    {
        Handle *calleeHandle = callbackInfo.doCallbackGetSubscriberHandleForUpdate();
        if (calleeHandle != nullptr && calleeHandle->isInitialized() && calleeHandle->getType() == eventHandleType)
        {
            unsubscribe(*calleeHandle);
        }

        m_listeners[eventHandleType].push_back(callbackInfo);

        uint32_t id;
        helper::SafeCast<size_t, uint32_t>(m_listeners[eventHandleType].size(), id);
        id--;

        auto *handle = callbackInfo.doCallbackGetSubscriberHandleForUpdate();
        *handle = Handle{.type = eventHandleType, .id = id};
    }
    void subscribe(const std::string &eventName, SubscriberCallbackInfo callbackInfo)
    {
        using Registry = common::HandleTypeRegistry;

        uint16_t eventType = Registry::instance().contains(eventName) ? Registry::instance().getType(eventName).value()
                                                                      : Registry::instance().registerType(eventName);
        subscribe(eventType, callbackInfo);
    }

    template <typename TEvent>
        requires THasGetType<TEvent>
    void emit(const TEvent &event)
    {
        const auto &base = static_cast<const star::common::IEvent &>(event);

        std::vector<Callback> aliveCallbacks = std::vector<Callback>();

        if (!m_listeners.contains(event.getType()))
        {
            return;
        }
        auto indicesToRemove = std::vector<size_t>();

        for (size_t i = 0; i < m_listeners[event.getType()].size(); i++)
        {
            bool keepAlive = false;
            m_listeners[event.getType()][i].doCallback(base, keepAlive);

            if (!keepAlive)
            {
                indicesToRemove.push_back(i);
            }
        }

        if (indicesToRemove.size() > 0)
        {
            int indicesToRemoveSize;
            helper::SafeCast<size_t, int>(indicesToRemove.size(), indicesToRemoveSize);

            for (int i = indicesToRemoveSize - 1; i >= 0; i--)
            {
                size_t index;
                helper::SafeCast<int, size_t>(i, index);

                const Handle *subHandle =
                    m_listeners[event.getType()][indicesToRemove[index]].doCallbackGetSubscriberHandleForUpdate();
                if (subHandle != nullptr)
                {
                    removeSubscriber(*subHandle, m_listeners[event.getType()]);
                }
            }
        }
    }

    void unsubscribe(const Handle &subscriberHandle)
    {
        assert(assertCheckContains(subscriberHandle));

        removeSubscriber(subscriberHandle, m_listeners[subscriberHandle.getType()]);
    }

  private:
    absl::flat_hash_map<uint16_t, std::vector<SubscriberCallbackInfo>> m_listeners;

    bool assertCheckContains(const Handle &subscriberHandle)
    {
        const bool contains = m_listeners.contains(subscriberHandle.getType());
        if (!contains)
        {
            std::ostringstream oss;
            oss << "Listener does not exist" << std::endl;
            oss << "The following are registered:" << std::endl;

            for (const auto &type : m_listeners)
            {
                oss << common::HandleTypeRegistry::instance().getTypeName(type.first).value() << std::endl;
            }

            core::logging::log(boost::log::trivial::error, oss.str());
        }

        return contains;
    }

    void removeSubscriber(const Handle &subscriberHandle, std::vector<SubscriberCallbackInfo> &subs)
    {
        size_t handleID = 0;
        helper::SafeCast<uint32_t, size_t>(subscriberHandle.getID(), handleID);

        assert(handleID < subs.size() && "Handle does not correlate to any listener registered for the template type");

        subs[handleID].doCallbackNotifySubscriberHandleCanBeDeleted(subscriberHandle);

        // Shift elements left from handleID+1 to end
        for (size_t i = handleID + 1; i < subs.size(); ++i)
        {
            subs[i - 1] = std::move(subs[i]);

            // Update the moved subscriber's handle (its index decreased by 1)
            size_t index;
            {
                int updatedIndex = int(i) - 1;
                helper::SafeCast<int, size_t>(updatedIndex, index);
            }

            if (Handle *h = subs.at(index).doCallbackGetSubscriberHandleForUpdate())
            {
                if (h->id > 0)
                {
                    h->id -= 1;
                }
            }
        }

        // Actually remove the trailing duplicate after the shift
        subs.pop_back();
    }
};
} // namespace star::common