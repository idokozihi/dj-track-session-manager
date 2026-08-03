#include "AudioTrack.h"
#include <iostream>
#include <cstring>
#include <random>

AudioTrack::AudioTrack(const std::string& title, const std::vector<std::string>& artists, 
                      int duration, int bpm, size_t waveform_samples)
    : title(title), artists(artists), duration_seconds(duration), bpm(bpm), 
      waveform_data(nullptr), waveform_size(waveform_samples) {

    // Allocate memory for waveform analysis
    waveform_data = new double[waveform_size];

    // Generate some dummy waveform data for testing
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-1.0, 1.0);

    for (size_t i = 0; i < waveform_size; ++i) {
        waveform_data[i] = dis(gen);
    }
    //std::cout << "AudioTrack created: " << title << " by " << std::endl;
    //for (const auto& artist : artists) {
       // std::cout << artist << " ";
    //}
    //std::cout << std::endl;
}

// ========== TODO: STUDENTS IMPLEMENT RULE OF 5 ==========

AudioTrack::~AudioTrack() {
    // TODO: Implement the destructor
    #ifdef DEBUG
    std::cout << "AudioTrack destructor called for: " << title << std::endl;
    #endif
    delete[] waveform_data;
}

AudioTrack::AudioTrack(const AudioTrack& other) :  title(other.title),
      artists(other.artists),
      duration_seconds(other.duration_seconds),
      bpm(other.bpm),
      waveform_data(nullptr),
    waveform_size(other.waveform_size)

{
    // TODO: Implement the copy constructor
    //std::cout << "AudioTrack copy constructor called for: " << other.title << std::endl;


    if(other.waveform_data != nullptr && waveform_size> 0 ){
        waveform_data = new double[waveform_size];
        for (size_t i = 0; i < waveform_size; i++){
        waveform_data[i] = other.waveform_data[i];
    }
}

}


AudioTrack& AudioTrack::operator=(const AudioTrack& other) {
    // TODO: Implement the copy assignment operator
    //std::cout << "AudioTrack copy assignment called for: " << other.title << std::endl;
    // Your code here...
    if(this==&other){
    return *this;
    }
    this->title = other.title;
    this->artists = other.artists;
    this->duration_seconds = other.duration_seconds;
    this->bpm = other.bpm;
    this->waveform_size = other.waveform_size;

    delete[] waveform_data;
    if(other.waveform_data){
        waveform_data = new double[waveform_size];
        for (size_t i = 0; i < waveform_size; i++){
            waveform_data[i] = other.waveform_data[i];
        }
    }else{
        waveform_data = nullptr;
    }
    return *this;
}

AudioTrack::AudioTrack(AudioTrack&& other) noexcept
:  title(std::move(other.title)),
      artists(std::move(other.artists)),
      duration_seconds(other.duration_seconds),
      bpm(other.bpm),
      waveform_data(other.waveform_data) ,
      waveform_size(other.waveform_size)
       {
    // TODO: Implement the move constructor
    #ifdef DEBUG
    std::cout << "AudioTrack move constructor called for: " << other.title << std::endl;
    #endif

    other.waveform_data = nullptr;
    other.waveform_size = 0;

    ///delete[] waveform_data;
}

AudioTrack& AudioTrack::operator=(AudioTrack&& other) noexcept {
    // TODO: Implement the move assignment operator


    //std::cout << "AudioTrack move assignment called for: " << other.title << std::endl;
    if(this!=&other){
        this->title = std::move(other.title);
        this->artists = std::move(other.artists);
        this->duration_seconds = other.duration_seconds;
        this->bpm = other.bpm;
        this->waveform_size = other.waveform_size;
        delete[](this->waveform_data);
        this->waveform_data = other.waveform_data;

        other.title = {};
        other.artists = {};
        other.duration_seconds = 0;
        other.bpm = 0;
        other.waveform_size = 0;
        other.waveform_data = nullptr;
        
    }
    return *this;
}

void AudioTrack::get_waveform_copy(double* buffer, size_t buffer_size) const {
    if (buffer && waveform_data && buffer_size <= waveform_size) {
        std::memcpy(buffer, waveform_data, buffer_size * sizeof(double));
    }
}