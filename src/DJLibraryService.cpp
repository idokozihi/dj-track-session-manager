#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist) , library()  {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
      for(const auto& info : library_tracks){
        if(info.type=="MP3"){
            AudioTrack* new_track = new MP3Track(info.title, info.artists,
                 info.duration_seconds,
            info.bpm, info.extra_param1, info.extra_param2 );
            library.push_back(new_track);
        } else if(info.type=="WAV"){
            AudioTrack* new_track = new WAVTrack(info.title, info.artists, info.duration_seconds,
            info.bpm, info.extra_param1, info.extra_param2 );
            library.push_back(new_track);
        }       
        // else{
           // std::cout << "[WARNING] Unknown format: " << info.type << "\n";
        //}
    }
    std::cout << "[INFO] Track library built: " 
          << library.size() 
          << " tracks loaded" << std::endl;
}

DJLibraryService::~DJLibraryService() {
    for (auto track : library) {
        delete track;  
    }
    library.clear();     
}

DJLibraryService::DJLibraryService(const DJLibraryService& other)
    : playlist(other.playlist), library()
{
      for (auto* track : other.library) {
        library.push_back(track->clone().release());
    }
}

DJLibraryService& DJLibraryService::operator=(const DJLibraryService& other) 
{
    if (this == &other) return *this;
    for (auto* track : library) {
        delete track;
    }
    library.clear();
    playlist = other.playlist;
        for (auto* track : other.library) {
        library.push_back(track->clone().release());
    }
    return *this;
}


/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    return (playlist.find_track(track_title));
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
      const std::vector<int>& track_indices) {
    playlist = Playlist(playlist_name);
    for (int index : track_indices) {
        if (index < 1 || index > (int)library.size()) {
            std::cout << "[WARNING] Invalid track index: " << index << "\n";
        continue;
    }
    AudioTrack* original = library[index - 1];
    PointerWrapper<AudioTrack> clone_wrapper = original->clone();
    if (!clone_wrapper) {
        std::cout << "[ERROR] Clone failed for: " << original->get_title() << "\n";
        continue;
    }
    clone_wrapper->load();
    clone_wrapper->analyze_beatgrid();
    
    
    playlist.add_track(clone_wrapper.release());
}
    // For now, add a placeholder to fix the linker error
    (void)playlist_name;  // Suppress unused parameter warning
    (void)track_indices;  // Suppress unused parameter warning
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    std::vector<std::string> titles;
    for(auto track : playlist.getTracks()){
        titles.push_back(track->get_title());
    }
    return titles;

}
