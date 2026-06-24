# Castle Process

Fort Processor-inspired experimental card for the Music Thing Modular Workshop Computer.

Castle Process is currently a playable prototype rather than a finished recreation. The present build is centered on a noisy mangling path with external audio mixed into it, plus switch-driven mode changes that already feel useful in performance.

---

# Current Status

Hardware testing so far suggests:

* external audio input is audible
* the internal noise layer is still too dominant
* switch `Up` and momentary `Down` both feel good
* `CV 1` and `CV 2` do not yet make an obvious audible difference
* the intended bass voice is not yet clearly audible

So this version should be treated as an in-progress sound engine.

---

# What The Current Build Does

At the moment, Castle Process behaves mainly as:

* a noisy external-audio mangler
* a rough chopped texture source
* a switchable performance effect with a useful momentary chaos gesture on switch `Down`

The current code is aiming toward a Fort Processor-style structure:

* distorted input section
* crude squarewave section
* variable chopper between sources
* bass/drone voice

But those sections are not yet balanced correctly in the audio result.

---

# Controls

## Main Knob

Currently affects the overall engine voicing and drive behaviour.

## X Knob

Currently affects chopping and related internal texture behaviour.

## Y Knob

Currently affects tuning and internal interaction.

## Switch Up

Latched alternate mode. This is already behaving usefully.

## Switch Middle

Default mixed mode.

## Switch Down

Momentary bend / chaos injection while held. This is one of the strongest working parts of the current build.

---

# Inputs

## Audio In 1

External audio is audible here, but is still largely competing with the internal noise layer rather than taking command of the sound.

## CV 1 / CV 2

These are intended to affect the internal control structure, but in the current build they do not yet produce a clear audible response in practice.

## Pulse 1

Used as a trigger input in the current design.

## Pulse 2

Reserved for further interaction and future refinement.

---

# Outputs

## Audio 1 / Audio 2

Two related noisy outputs from the mangling engine.

## Pulse 1 / Pulse 2

Utility pulse outputs derived from internal activity.

---

# Next Tuning Priorities

The most important follow-up jobs are:

* bring external audio further forward in the mix
* make `CV 1` and `CV 2` clearly audible and useful
* make the bass voice clearly present
* rebalance the internal sections so the result feels more like a Fort Processor and less like noise layered over input

