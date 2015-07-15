/*
 * This file is part of the Colobot: Gold Edition source code
 * Copyright (C) 2001-2014, Daniel Roux, EPSITEC SA & TerranovaTeam
 * http://epsiteс.ch; http://colobot.info; http://github.com/colobot
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://gnu.org/licenses
 */

/**
 * \file alsound.h
 * \brief OpenAL implementation of sound system
 */

#pragma once

#include "common/logger.h"
#include "sound/sound.h"

#include "sound/oalsound/buffer.h"
#include "sound/oalsound/channel.h"
#include "sound/oalsound/check.h"

#include <map>
#include <memory>
#include <string>
#include <list>

#include <al.h>


struct OldMusic
{
    std::unique_ptr<Channel> music;
    float fadeTime = 0.0f;
    float currentTime = 0.0f;

    inline friend bool operator<(const OldMusic & l, const OldMusic & r)
    {
        return l.currentTime < r.currentTime;
    }

    inline friend bool operator==(const OldMusic & l, const OldMusic & r)
    {
        return l.currentTime == r.currentTime;
    }
};

class ALSound : public CSoundInterface
{
public:
    ALSound();
    ~ALSound();

    bool Create() override;
    bool Cache(SoundType, const std::string &) override;
    bool CacheMusic(const std::string &) override;
    bool IsCached(SoundType) override;
    bool IsCachedMusic(const std::string &) override;

    bool GetEnable() override;
    void SetAudioVolume(int volume) override;
    int GetAudioVolume() override;
    void SetMusicVolume(int volume) override;
    int GetMusicVolume() override;

    void SetListener(const Math::Vector &eye, const Math::Vector &lookat) override;
    void FrameMove(float rTime) override;

    int Play(SoundType sound, float amplitude=1.0f, float frequency=1.0f, bool loop = false) override;
    int Play(SoundType sound, const Math::Vector &pos, float amplitude=1.0f, float frequency=1.0f, bool loop = false) override;
    bool FlushEnvelope(int channel) override;
    bool AddEnvelope(int channel, float amplitude, float frequency, float time, SoundNext oper) override;
    bool Position(int channel, const Math::Vector &pos) override;
    bool Frequency(int channel, float frequency) override;
    bool Stop(int channel) override;
    bool StopAll() override;
    bool MuteAll(bool mute) override;

    bool PlayMusic(int rank, bool repeat, float fadeTime=2.0f) override;
    bool PlayMusic(const std::string &filename, bool repeat, float fadeTime=2.0f) override;
    bool RestartMusic() override;
    void SuspendMusic() override;
    void StopMusic(float fadeTime=2.0f) override;
    bool IsPlayingMusic() override;
    bool PlayPauseMusic(const std::string &filename, bool repeat) override;
    void StopPauseMusic() override;

private:
    void CleanUp();
    int GetPriority(SoundType);
    bool SearchFreeBuffer(SoundType sound, int &channel, bool &alreadyLoaded);
    bool CheckChannel(int &channel);

    bool m_enabled;
    float m_audioVolume;
    float m_musicVolume;
    unsigned int m_channelsLimit;
    ALCdevice* m_device;
    ALCcontext* m_context;
    std::map<SoundType, std::unique_ptr<Buffer>> m_sounds;
    std::map<std::string, std::unique_ptr<Buffer>> m_music;
    std::map<int, std::unique_ptr<Channel>> m_channels;
    std::unique_ptr<Channel> m_currentMusic;
    std::list<OldMusic> m_oldMusic;
    OldMusic m_previousMusic;
    Math::Vector m_eye;
    Math::Vector m_lookat;
};

