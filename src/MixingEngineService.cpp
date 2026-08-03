#include "MixingEngineService.h"
#include <iostream>
#include <memory>

/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    decks[0] = nullptr;
    decks[1] = nullptr;

    std::cout << "[MixingEngineService] Initialized with 2 empty decks." << std::endl;
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService()
{
    std::cout << "[MixingEngineService] Cleaning up decks..." << std::endl;
    for (int i = 0; i < 2; i++)
    {
        if (decks[i] != nullptr)
        {
            delete decks[i];
            decks[i] = nullptr;
        }
    }
}

MixingEngineService::MixingEngineService(const MixingEngineService &other)
    : active_deck(other.active_deck),
      auto_sync(other.auto_sync),
      bpm_tolerance(other.bpm_tolerance)
{
    for (int i = 0; i < 2; i++)
    {
        if (other.decks[i] != nullptr)
        {
            decks[i] = other.decks[i]->clone().release();
        }
        else
        {
            decks[i] = nullptr;
        }
    }
}

MixingEngineService &MixingEngineService::operator=(const MixingEngineService &other)
{
    if (this == &other)
        return *this;
    for (int i = 0; i < 2; i++)
    {
        if (decks[i] != nullptr)
        {
            delete decks[i];
            decks[i] = nullptr;
        }
    }
    active_deck = other.active_deck;
    auto_sync = other.auto_sync;
    bpm_tolerance = other.bpm_tolerance;
    for (int i = 0; i < 2; i++)
    {
        if (other.decks[i] != nullptr)
        {
            decks[i] = other.decks[i]->clone().release();
        }
        else
        {
            decks[i] = nullptr;
        }
    }
    return *this;
}

    /**
     * TODO: Implement loadTrackToDeck method
     * @param track: Reference to the track to be loaded
     * @return: Index of the deck where track was loaded, or -1 on failure
     */
    int MixingEngineService::loadTrackToDeck(const AudioTrack &track)
    {
        auto clone_wrapper = track.clone();
        if (!clone_wrapper)
        {
            return -1;
        }
        int target = 1 - active_deck;
        std::cout << "[Deck Switch] Target deck: " << target << std::endl;

        if (decks[target] != nullptr)
        {
            delete decks[target];
            decks[target] = nullptr;
        }
        clone_wrapper->load();
        clone_wrapper->analyze_beatgrid();

        if (auto_sync && decks[active_deck] != nullptr)
        {
            if (!can_mix_tracks(clone_wrapper))
            {
                sync_bpm(clone_wrapper);
            }
        }
        else if (auto_sync)
        {

            std::cout << "[Sync BPM] Cannot sync - one of the decks is empty.\n";
        }

        AudioTrack *ptr = clone_wrapper.release();
        decks[target] = ptr;

        std::cout << "[Load Complete] '" << ptr->get_title()
                  << "' is now loaded on deck " << target
                  << std::endl;
        active_deck = target;
        std::cout << "[Active Deck] Switched to deck " << target << "\n";
        return target; // Placeholder
    }

    /**
     * @brief Display current deck status
     */
    void MixingEngineService::displayDeckStatus() const
    {
        std::cout << "\n=== Deck Status ===\n";
        for (size_t i = 0; i < 2; ++i)
        {
            if (decks[i])
                std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
            else
                std::cout << "Deck " << i << ": [EMPTY]\n";
        }
        std::cout << "Active Deck: " << active_deck << "\n";
        std::cout << "===================\n";
    }

    /**
     * TODO: Implement can_mix_tracks method
     *
     * Check if two tracks can be mixed based on BPM difference.
     *
     * @param track: Track to check for mixing compatibility
     * @return: true if BPM difference <= tolerance, false otherwise
     */
    bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack> &track) const
    {
        if (!decks[active_deck])
        {
            return false;
        }
        if (!track)
        {
            return false;
        }
        int bpm1 = decks[active_deck]->get_bpm();
        int bpm2 = track->get_bpm();

        int diff;
        if (bpm1 > bpm2)
        {
            diff = bpm1 - bpm2;
        }
        else
        {
            diff = bpm2 - bpm1;
        }

        if (diff <= bpm_tolerance)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    /**
     * TODO: Implement sync_bpm method
     * @param track: Track to synchronize with active deck
     */
    void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack> &track) const
    {
        if (!decks[active_deck] || !track)
        {
            return;
        }
        int old_bpm = track->get_bpm();
        int new_bpm = (old_bpm + decks[active_deck]->get_bpm()) / 2;
        track->set_bpm(new_bpm);
        std::cout << "[Sync BPM] Syncing BPM from "
                  << old_bpm << " to " << new_bpm << "\n";
    }
