#pragma once

#include <stdint.h>
#include <vector>

#include <cassert>

namespace star::common
{
class FrameTracker
{
  public:
    class Setup
    {
      public:
        Setup(uint8_t numFramesInFlight, uint8_t numUniqueTargetFramesForFinalization)
            : m_numFramesInFlight(numFramesInFlight),
              m_numUniqueTargetFramesForFinalization(numUniqueTargetFramesForFinalization)
        {
        }

        const uint8_t &getNumFramesInFlight() const
        {
            return m_numFramesInFlight;
        }

        const uint8_t &getNumUniqueTargetFramesForFinalization() const
        {
            return m_numUniqueTargetFramesForFinalization;
        }

      private:
        friend class FrameTracker;
        uint8_t m_numFramesInFlight;                    // total number of frames which can be in flight at once
        uint8_t m_numUniqueTargetFramesForFinalization; // equitable to the total number of swapcahin images or images
                                                        // looped over for an offscreen render target
    };
    class Current
    {
      public:
        class FramesInFlightTracking
        {
          public:
            FramesInFlightTracking() = default;
            FramesInFlightTracking(const uint8_t numOfFramesInFlight)
                : m_numOfTimesFrameProcessed(std::vector<uint64_t>((size_t)numOfFramesInFlight, uint64_t(0)))
            {
            }
            uint64_t &getNumOfTimesFrameProcessed(const uint8_t &frameInFlightIndex)
            {
                size_t index = static_cast<size_t>(frameInFlightIndex);
                assert(index < m_numOfTimesFrameProcessed.size());
                return m_numOfTimesFrameProcessed[index];
            }
            const uint64_t &getNumOfTimesFrameProcessed(const uint8_t &frameInFlightIndex) const
            {
                assert(frameInFlightIndex < m_numOfTimesFrameProcessed.size());

                return m_numOfTimesFrameProcessed[frameInFlightIndex];
            }
            size_t getSize()
            {
                return m_numOfTimesFrameProcessed.size();
            }

          private:
            std::vector<uint64_t> m_numOfTimesFrameProcessed;
        };

        explicit Current(uint8_t totalNumOfFramesInFlight)
            : m_globalFrameCounter(0), m_framesInFlightTracking(totalNumOfFramesInFlight), m_frameInFlightIndex(0),
              m_finalTargetImageIndex(0)
        {
        }

        void setFrameInFlightIndex(uint8_t index)
        {
            m_frameInFlightIndex = index;
        }
        void setFinalTargetImageIndex(uint8_t index)
        {
            m_finalTargetImageIndex = index;
        }
        const uint64_t &getGlobalFrameCounter() const
        {
            return m_globalFrameCounter;
        }
        const FramesInFlightTracking &getFramesInFlightTracking() const
        {
            return m_framesInFlightTracking;
        }
        const uint8_t &getFrameInFlightIndex() const
        {
            return m_frameInFlightIndex;
        }
        const uint8_t &getFinalTargetImageIndex() const
        {
            return m_finalTargetImageIndex;
        }
        const uint64_t &getNumTimesFrameProcessed() const
        {
            return m_framesInFlightTracking.getNumOfTimesFrameProcessed(m_frameInFlightIndex);
        }

      private:
        friend class FrameTracker;
        uint64_t m_globalFrameCounter;
        FramesInFlightTracking m_framesInFlightTracking;
        uint8_t m_frameInFlightIndex;
        uint8_t m_finalTargetImageIndex;
    };

    FrameTracker() : m_setup(Setup(1, 1)), m_current(1)
    {
    }
    explicit FrameTracker(Setup setup) : m_setup(std::move(setup)), m_current(setup.getNumFramesInFlight())
    {
    }

    void triggerIncrementForCurrentFrame()
    {
        m_current.m_globalFrameCounter++;
        m_current.m_framesInFlightTracking.getNumOfTimesFrameProcessed(m_current.getFrameInFlightIndex())++;
    }

    Setup &getSetup()
    {
        return m_setup;
    }
    const Setup &getSetup() const
    {
        return m_setup;
    }
    Current &getCurrent()
    {
        return m_current;
    }
    const Current &getCurrent() const
    {
        return m_current;
    }

  private:
    Setup m_setup;
    Current m_current;
};
} // namespace star::common