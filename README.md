# Castle Process

Fort Processor-inspired noise mangler for the Music Thing Modular Workshop Computer.

This card turns the Workshop Computer into a gritty psychogeographic instrument:
bursts of noise, unstable echo paths, crude pitch haze, and triggerable texture shifts.

The design stays close to the hardware rules:

* integer-only DSP
* no divisions in the sample loop
* lightweight control updates
* audio path kept simple and stable

---

# What It Does

Castle Process is a noisy sound-mangler rather than a sequencer.

It generates:

* noise bursts
* lo-fi delay smear
* unstable pseudo-reverb
* a rough tonal layer
* trigger pulses for patching other gear

The two audio outputs are related but not identical, so the card can be used as a stereo texture source or as two separate mangled voices.

---

# Controls

## Main Knob

Sets the overall motion and pitch region.

## X Knob

Sets gain and texture density.

## Y Knob

Sets feedback, grit, and instability.

## Switch Up

Cleaner, tighter, more focused noise paths.

## Switch Middle

Wider echo field with more motion.

## Switch Down

Most aggressive and unstable mode.

---

# Inputs

## Pulse 1

External trigger input. Overrides the internal clock while active.

## Pulse 2

Freeze input. Holds the current texture state while high.

## CV 1

Modulates the X control.

## CV 2

Modulates the Y control.

---

# Outputs

## Pulse 1

Trigger pulse out for syncing other gear.

## Pulse 2

Secondary mode pulse for patching and clock decoration.

## Audio 1

Primary mangled output.

## Audio 2

Secondary mangled output with a different delay mix.

---

# Patch Ideas

* use Audio 1 and Audio 2 as a stereo noise instrument
* feed Audio 1 into filters or resonators for a dark industrial wash
* use Pulse 1 to trigger envelopes in a modular patch
* send CV 1 or CV 2 from sequencers for evolving external control
