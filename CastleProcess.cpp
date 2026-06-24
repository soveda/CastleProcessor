/*
Fort Processor-inspired program card for the Music Thing Modular Workshop Computer.

Concept source:
- Isn'tses Fort Processor: psychogeographic noise synth / sound mangler
- Translated into Workshop Computer controls and I/O

Design goals:
- keep the audio interrupt lean
- use only integer DSP
- avoid divisions in the sample loop
- make the card feel unstable, noisy, and playable
*/

#include "ComputerCard.h"
#include "hardware/clocks.h"
#include <stdint.h>

class CastleProcess : public ComputerCard
{
public:
    static constexpr uint32_t kDelaySize = 2048;
    static constexpr uint32_t kDelayMask = kDelaySize - 1;
    static constexpr uint32_t kControlDiv = 256;
    static constexpr uint32_t kTransportDiv = 96;

    int32_t delayA[kDelaySize];
    int32_t delayB[kDelaySize];

    uint32_t writePos = 0;
    uint32_t rng = 0x5A17C3E1u;
    uint32_t sampleCounter = 0;

    uint32_t internalClockCounter = 0;
    uint32_t internalClockPeriod = 22000;
    uint32_t externalClockTimeout = 0;

    bool externalClockActive = false;
    bool triggerGate = false;
    uint32_t triggerGateSamples = 0;

    bool sourceWhite = true;
    bool manualLatch = false;

    int32_t burstEnv = 0;
    int32_t toneEnv = 0;
    int32_t tonePhase = 0;
    int32_t tonePitch = 48;
    int32_t modeStep = 0;

    int32_t smoothedGain = 1200;
    int32_t smoothedLength = 1600;
    int32_t smoothedSpace = 1800;
    int32_t smoothedPitch = 1700;
    int32_t smoothedFeedback = 1800;
    int32_t smoothedChaos = 900;

    int32_t controlGain = 1200;
    int32_t controlLength = 1600;
    int32_t controlSpace = 1800;
    int32_t controlPitch = 1700;
    int32_t controlFeedback = 1800;
    int32_t controlChaos = 900;

    int32_t modeIndex = 0;

    inline int32_t Clamp16(int32_t x)
    {
        if(x > 2047) return 2047;
        if(x < -2048) return -2048;
        return x;
    }

    inline int32_t Clamp12(int32_t x)
    {
        if(x > 4095) return 4095;
        if(x < 0) return 0;
        return x;
    }

    inline int32_t SoftClip(int32_t x)
    {
        if(x > 1536)
        {
            x = 1536 + ((x - 1536) >> 2);
        }
        else if(x < -1536)
        {
            x = -1536 + ((x + 1536) >> 2);
        }
        return x;
    }

    inline uint32_t Random()
    {
        rng ^= rng << 13;
        rng ^= rng >> 17;
        rng ^= rng << 5;
        return rng;
    }

    inline int32_t NoiseSample()
    {
        return (int32_t)(Random() & 0xFFF) - 2048;
    }

    inline int32_t BitCrush(int32_t x, int32_t amount)
    {
        int32_t shift = 3 + (amount >> 10);
        if(shift > 8)
        {
            shift = 8;
        }
        return (x >> shift) << shift;
    }

    inline int32_t ReadDelay(const int32_t *buf, uint32_t offset)
    {
        return buf[offset & kDelayMask];
    }

    inline void WriteDelay(int32_t *buf, uint32_t offset, int32_t value)
    {
        buf[offset & kDelayMask] = value;
    }

    void SeedRng()
    {
        uint64_t uid = UniqueCardID();
        rng ^= (uint32_t)uid;
        rng ^= (uint32_t)(uid >> 32);
        rng ^= 0x9E3779B9u;
        if(rng == 0)
        {
            rng = 0x5A17C3E1u;
        }
    }

    void UpdateControls()
    {
        int32_t main = KnobVal(Knob::Main);
        int32_t x = KnobVal(Knob::X);
        int32_t y = KnobVal(Knob::Y);

        x += CVIn1() << 1;
        y += CVIn2() << 1;

        controlGain = Clamp12((x + main) >> 1);
        controlLength = Clamp12(main + (y >> 1));
        controlSpace = Clamp12(4095 - ((x + y) >> 1));
        controlPitch = Clamp12(main + (x >> 1));
        controlFeedback = Clamp12(y);
        controlChaos = Clamp12((x >> 1) + (y >> 1));

        modeIndex = SwitchVal();

        if(modeIndex == Switch::Up)
        {
            controlGain = Clamp12((controlGain * 3) >> 2);
            controlSpace = Clamp12(controlSpace + 256);
        }
        else if(modeIndex == Switch::Middle)
        {
            controlLength = Clamp12(controlLength + 256);
            controlChaos = Clamp12(controlChaos + 192);
        }
        else
        {
            controlGain = Clamp12((controlGain * 5) >> 2);
            controlFeedback = Clamp12(controlFeedback + 384);
            controlChaos = Clamp12(controlChaos + 512);
        }

        smoothedGain += (controlGain - smoothedGain) >> 4;
        smoothedLength += (controlLength - smoothedLength) >> 4;
        smoothedSpace += (controlSpace - smoothedSpace) >> 4;
        smoothedPitch += (controlPitch - smoothedPitch) >> 4;
        smoothedFeedback += (controlFeedback - smoothedFeedback) >> 4;
        smoothedChaos += (controlChaos - smoothedChaos) >> 4;
    }

    void UpdateTransport()
    {
        if(PulseIn1RisingEdge())
        {
            externalClockActive = true;
            externalClockTimeout = 48000;
            triggerGate = true;
            triggerGateSamples = 600;
        }

        if(externalClockTimeout > 0)
        {
            externalClockTimeout--;
        }
        else
        {
            externalClockActive = false;
        }

        bool freeze = PulseIn2();

        if(!externalClockActive)
        {
            uint32_t slow = 28000u - ((uint32_t)smoothedPitch * 5u);
            if(slow < 2500u)
            {
                slow = 2500u;
            }

            internalClockPeriod = slow;

            if(modeIndex == Switch::Middle)
            {
                internalClockPeriod += 1600u;
            }
            else if(modeIndex == Switch::Down)
            {
                internalClockPeriod += 3600u;
            }

            internalClockCounter++;
            if(internalClockCounter >= internalClockPeriod)
            {
                internalClockCounter = 0;
                triggerGate = true;
                triggerGateSamples = 700;
            }
        }

        if(triggerGateSamples > 0)
        {
            triggerGateSamples--;
        }
        else
        {
            triggerGate = false;
        }

        if(triggerGate)
        {
            burstEnv = 4095;
            toneEnv = 2048 + (smoothedGain >> 1);
            tonePitch = 36 + (smoothedPitch >> 8);

            if(tonePitch < 18) tonePitch = 18;
            if(tonePitch > 96) tonePitch = 96;

            if(!freeze && ((Random() & 7u) == 0u))
            {
                sourceWhite = !sourceWhite;
            }

            if((Random() & 3u) == 0u)
            {
                manualLatch = !manualLatch;
            }
        }

        if(!freeze && (sampleCounter & 1023u) == 0u)
        {
            if((Random() & 31u) < (uint32_t)(smoothedChaos >> 7))
            {
                modeStep = (modeStep + 1) & 3;
            }
        }
    }

    virtual void ProcessSample()
    {
        if(sampleCounter == 0)
        {
            SeedRng();
        }

        sampleCounter++;

        if((sampleCounter & (kControlDiv - 1)) == 0)
        {
            UpdateControls();
        }

        if((sampleCounter & (kTransportDiv - 1)) == 0)
        {
            UpdateTransport();
        }

        int32_t input = AudioIn1();
        int32_t accent = AudioIn2();
        int32_t noise = sourceWhite ? NoiseSample() : BitCrush(NoiseSample(), smoothedChaos);

        int32_t writeA = input + ((noise * (smoothedGain + 256)) >> 10);
        int32_t writeB = accent + ((noise * (smoothedLength + 256)) >> 11);

        if(triggerGate)
        {
            writeA += (noise >> 1);
            writeB += (noise >> 2);
        }

        int32_t feedbackA = ReadDelay(delayB, writePos - (smoothedSpace >> 1));
        int32_t feedbackB = ReadDelay(delayA, writePos - (smoothedLength >> 1));

        if(modeIndex == Switch::Up)
        {
            writeA += (feedbackB * (4095 - smoothedFeedback)) >> 12;
            writeB += (feedbackA * (smoothedFeedback >> 1)) >> 12;
        }
        else if(modeIndex == Switch::Middle)
        {
            writeA += (feedbackA * (smoothedFeedback + 256)) >> 12;
            writeB += (feedbackB * (4095 - (smoothedFeedback >> 1))) >> 12;
        }
        else
        {
            writeA += (feedbackA * (smoothedFeedback + 512)) >> 11;
            writeB += (feedbackB * (smoothedFeedback + 512)) >> 11;
        }

        int32_t delayOutA = ReadDelay(delayA, writePos - (256 + (smoothedPitch >> 3)));
        int32_t delayOutB = ReadDelay(delayB, writePos - (384 + (smoothedPitch >> 2)));

        int32_t reverbA = (delayOutA >> 1) + (delayOutB >> 2);
        int32_t reverbB = (delayOutB >> 1) - (delayOutA >> 3);

        if(triggerGate)
        {
            reverbA += 512;
            reverbB -= 256;
        }

        int32_t out1 = SoftClip((writeA >> 1) + reverbA);
        int32_t out2 = SoftClip((writeB >> 1) + reverbB);

        if(burstEnv > 0)
        {
            out1 += (burstEnv >> 2);
            out2 -= (burstEnv >> 3);
            burstEnv -= (burstEnv >> 10) + 8;
            if(burstEnv < 0)
            {
                burstEnv = 0;
            }
        }

        if(toneEnv > 0)
        {
            toneEnv -= (toneEnv >> 9) + 1;
            if(toneEnv < 0)
            {
                toneEnv = 0;
            }
        }

        tonePhase += tonePitch;
        tonePhase &= 4095;

        int32_t tone = (tonePhase & 2048) ? (2048 - (tonePhase & 2047)) : (tonePhase & 2047);
        tone -= 1024;
        tone <<= 1;
        tone = (tone * toneEnv) >> 10;

        out1 += tone;
        out2 += (tone >> 1);

        out1 = SoftClip(out1);
        out2 = SoftClip(out2);

        AudioOut1(Clamp16(out1));
        AudioOut2(Clamp16(out2));

        WriteDelay(delayA, writePos, out1 + ((delayOutB * smoothedFeedback) >> 12));
        WriteDelay(delayB, writePos, out2 + ((delayOutA * smoothedSpace) >> 12));

        writePos = (writePos + 1) & kDelayMask;

        PulseOut1(triggerGate && triggerGateSamples > 0);
        PulseOut2((modeStep & 1) != 0);

        if((sampleCounter & (kTransportDiv - 1)) == 0)
        {
            int32_t ledMain = (smoothedGain * 4095) >> 12;
            int32_t ledSpace = (smoothedSpace * 4095) >> 12;
            int32_t ledChaos = (smoothedChaos * 4095) >> 12;

            LedBrightness(0, triggerGate ? 4095 : 0);
            LedBrightness(1, ledMain > 4095 ? 4095 : ledMain);
            LedBrightness(2, ledSpace > 4095 ? 4095 : ledSpace);
            LedBrightness(3, ledChaos > 4095 ? 4095 : ledChaos);
            LedBrightness(4, externalClockActive ? 4095 : 0);
            LedBrightness(5, manualLatch ? 4095 : 0);
        }
    }
};

int main()
{
    set_sys_clock_khz(144000, true);

    CastleProcess card;
    card.Run();
}
