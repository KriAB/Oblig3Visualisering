#include "soundcomponent.h"
#include "soundsource.h"
#include "wavefilehandler.h"

SoundComponent::SoundComponent(std::string name, bool loop, float gain) :
    mName(name),
    mSource(0),
    mBuffer(0),
    mPosition(0.0f, 0.0f, 0.0f),
    mVelocity(0.0f, 0.0f, 0.0f)
{
    alGetError();
    alGenBuffers(1, &mBuffer);
    checkError("alGenBuffers");
    alGenSources(1, &mSource);
    checkError("alGenSources");
    alSourcef(mSource, AL_PITCH, 1.0f);
    alSourcef(mSource, AL_GAIN, gain);

    ALfloat temp[3] = {mPosition.x, mPosition.y, mPosition.z};
    alSourcefv(mSource, AL_POSITION, temp);
    ALfloat temp2[3] = {mVelocity.x, mVelocity.y, mVelocity.z};
    alSourcefv(mSource, AL_VELOCITY, temp2);

    alSourcei(mSource, AL_LOOPING, loop);
}
SoundComponent::~SoundComponent()
{
    std::cout << "Destroying SoundSource " + mName;
    stop();
    alGetError();
    alSourcei(mSource, AL_BUFFER, 0);
    checkError("alSourcei");
    alDeleteSources(1, &mSource);
    checkError("alDeleteSources");
    alDeleteBuffers(1, &mBuffer);
    checkError("alDeleteBuffers");
}

bool SoundComponent::loadWave(std::string filePath)
{
    std::cout << "Loading wave file!\n";
    ALuint frequency{};
    ALenum format{};
    wave_t* waveData = new wave_t();
    if (!WaveFileHandler::loadWave(filePath, waveData))
    {
        std::cout << "Error loading wave file!\n";
        return false; // error loading wave file data
    }

    frequency = waveData->sampleRate;

    switch (waveData->bitsPerSample)
    {
    case 8:
        switch (waveData->channels)
        {
        case 1: format = AL_FORMAT_MONO8;
            std::cout << "Format: 8bit Mono\n";
            break;
        case 2: format = AL_FORMAT_STEREO8;
            std::cout << "Format: 8bit Stereo\n";
            break;
        default: break;
        }
        break;
    case 16:
        switch (waveData->channels)
        {
        case 1: format = AL_FORMAT_MONO16;
            std::cout << "Format: 16bit Mono\n";
            break;
        case 2: format = AL_FORMAT_STEREO16;
            std::cout << "Format: 16bit Stereo\n";
            break;
        default: break;
        }
        break;
    default: break;
    }

    if (waveData->buffer == NULL)
    {
        std::cout << "NO WAVE DATA!\n";
    }

    std::ostringstream i2s;
    i2s << waveData->dataSize;
    std::cout << "DataSize: " << i2s.str() << " bytes\n";

    alGetError();
    alBufferData(mBuffer, format, waveData->buffer, waveData->dataSize, frequency);
    checkError("alBufferData");
    alSourcei(mSource, AL_BUFFER, mBuffer);
    checkError("alSourcei (loadWave)");

    std::cout << "Loading complete!\n";
    if (waveData->buffer) delete waveData->buffer;
    if (waveData) delete waveData;
    return true;
}

void SoundComponent::play()
{
    alSourcePlay(mSource);
}
void SoundComponent::pause()
{
    alSourcePause(mSource);
}
void SoundComponent::stop()
{
    alSourceStop(mSource);
}

void SoundComponent::setPosition(gsl::Vector3D newPos)
{
    mPosition = newPos;
    ALfloat temp[3] = {mPosition.x, mPosition.y, mPosition.z};
    alSourcefv(mSource, AL_POSITION, temp);
}
void SoundComponent::setVelocity(gsl::Vector3D newVel)
{
    mVelocity = newVel;
    ALfloat temp[3] = {mVelocity.x, mVelocity.y, mVelocity.z};
    alSourcefv(mSource, AL_VELOCITY, temp);
}

bool SoundComponent::checkError(std::string name)
{
    switch (alGetError())
    {
    case AL_NO_ERROR:
        break;
    case AL_INVALID_NAME:
        std::cout << "OpenAL Error: "+name+": Invalid name!\n";
        return false;
    case AL_INVALID_ENUM:
        std::cout << "OpenAL Error: "+name+": Invalid enum!\n";
        return false;
    case AL_INVALID_VALUE:
        std::cout << "OpenAL Error: "+name+": Invalid value!\n";
        return false;
    case AL_INVALID_OPERATION:
        std::cout << "OpenAL Error: "+name+": Invalid operation!\n";
        return false;
    case AL_OUT_OF_MEMORY:
        std::cout << "OpenAL Error: "+name+": Out of memory!\n";
        return false;
    default: break;
    }
    return true;
}
