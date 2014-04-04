#include "Audio.h"
#include <map>
#include <list>
#include <iostream>

using namespace rho;

// Initialize the Instance to NULL
AudioLib *AudioLib::m_pInstance = NULL;

AudioLib *AudioLib::GetInstance()
{
    if(!m_pInstance)
        m_pInstance = new AudioLib;

    return m_pInstance;
}

void AudioLib::DeleteInstance()
{
    if(m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

AudioLib::AudioLib()
{
    m_bufCache.clear();
    m_sndEffects.clear();
    m_sndBank.clear();
//    m_musMusic.clear();
}

AudioLib::~AudioLib()
{
    // invoke a Shutdown function to do cleanup and deallocate memory
//  Shutdown();
}

void AudioLib::update()
{
    std::list<sf::Sound>::iterator pm_it = m_sndEffects.begin();
    while(pm_it != m_sndEffects.end())
    {
        if (pm_it->getStatus() == sf::SoundSource::Status::Stopped)
        {
            m_sndEffects.erase(pm_it++);
        }
        else
        {
            ++pm_it;
        }
    }
    showSoundEffects();
}

void AudioLib::showSoundBank()
{
    std::list<sf::Sound>::iterator pm_it = m_sndBank.begin();
    while(pm_it != m_sndBank.end())
    {
        std::cout << &*pm_it << " ";
        ++pm_it;
    }
    std::cout << "\n";
}

void AudioLib::showSoundEffects()
{
    std::list<sf::Sound>::iterator pm_it = m_sndEffects.begin();
    while(pm_it != m_sndEffects.end())
    {
        std::cout << &*pm_it << " ";
        ++pm_it;
    }
    std::cout << "\n";
}

int AudioLib::get_total_channels()
{
    return m_sndEffects.size() + m_sndBank.size();
}

void AudioLib::load_music(HashString const &filename)
{
    m_musMusic.openFromFile(filename.getString());
}

void AudioLib::play_music()
{
    m_musMusic.play();
}

void AudioLib::stop_music()
{
    m_musMusic.stop();
}

void AudioLib::pause_music()
{
    m_musMusic.pause();
}

void AudioLib::playSound(HashString const &alias)
{
    sf::Sound tmpSound;
    m_sndEffects.push_front(tmpSound);
    std::list<sf::Sound>::iterator i = m_sndEffects.begin();
    i->setBuffer(m_bufCache[alias]);
    i->play();
    // showSoundEffects();
    // m_sndEffects[channel].setBuffer(m_bufCache[alias]);
    // m_sndEffects[channel].play();
}

void AudioLib::stopAll()
{
    std::list<sf::Sound>::iterator pm_it = m_sndBank.begin();
    while(pm_it != m_sndBank.end())
    {
        pm_it->stop();
    }
}

void AudioLib::pauseAll()
{
    std::list<sf::Sound>::iterator pm_it = m_sndBank.begin();
    while(pm_it != m_sndBank.end())
    {
        pm_it->pause();
    }
}

void AudioLib::playAll()
{
    std::list<sf::Sound>::iterator pm_it = m_sndBank.begin();
    while(pm_it != m_sndBank.end())
    {
        pm_it->play();
    }
}

sf::Sound * AudioLib::createSound(HashString const &alias)
{
    sf::Sound tmpSound;
    tmpSound.setBuffer(m_bufCache[alias]);
    m_sndBank.push_back(tmpSound);

    // showSoundBank();
    return &m_sndBank.back();
}

void AudioLib::setBuffer(sf::Sound * object, HashString const &alias)
{
    object->setBuffer(m_bufCache[alias]);
}

void AudioLib::deleteSound(sf::Sound * victim)
{
    m_sndBank.remove_if( [&](sf::Sound & sound) { return &sound == victim; });
    // showSoundBank();
}

void AudioLib::add_to_cache(HashString const &filename, HashString const &alias)
{
    m_bufCache[alias].loadFromFile(filename.getString());
}

void AudioLib::clear_cache()
{
    m_bufCache.clear();
}
