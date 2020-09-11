//
// Created by Clay Bailey on 2020/09/10.
//

#ifndef SENIOR_PROJECT_IRENDERABLEAUDIO_H
#define SENIOR_PROJECT_IRENDERABLEAUDIO_H

#include <cstdint>

class IRenderableAudio {
public:
    virtual ~IRenderableAudio() = default;
    virtual void renderAudio(float *audioData, int32_t numFrames) = 0;
};

#endif //SENIOR_PROJECT_IRENDERABLEAUDIO_H
