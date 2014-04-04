#define MAX_SND_CHAN 192
#ifndef AUDIOLIB_H_INCLUDED
#define AUDIOLIB_H_INCLUDED

#include <SFML/Audio.hpp>
#include <map>
#include <list>

#include "../HashString.h"

namespace rho
{

class AudioLib
{
//////////////////////////////
//  Variables
//////////////////////////////
public:

private:
    static AudioLib *m_pInstance;      // Class Instance
    std::map<HashString,sf::SoundBuffer> m_bufCache;
    std::list<sf::Sound> m_sndEffects;
    std::list<sf::Sound> m_sndBank;
    sf::Music m_musMusic;

protected:


//////////////////////////////
//  Functions
//////////////////////////////
public:
    static AudioLib *GetInstance();             // Retrieve the instance
    void DeleteInstance();                      // Delete the instance
    void update();
    int get_total_channels();
    // Music functions
    void load_music(HashString const &filename);
    void play_music();
    void stop_music();
    void pause_music();
    // Sound functions
    void playSound(HashString const &alias);
    sf::Sound * createSound(HashString const &alias);
    void setBuffer(sf::Sound * object, HashString const &alias);
    void deleteSound (sf::Sound * victim);
    void pauseAll();
    void stopAll();
    void playAll();

    // Cache functions
    void clear_cache();
    void add_to_cache(HashString const &filename, HashString const &alias);

private:
    AudioLib();                                 // Constructor
    AudioLib(const AudioLib&);                  // Copy Constructor
    AudioLib &operator =(const AudioLib&);      // Assignment Operator
    void showSoundBank();
    void showSoundEffects();


protected:
    ~AudioLib();                               // Destructor

};

}

#endif // AUDIOLIB_H_INCLUDED
