# Castle Process

Fort Processor-inspired performance card for the Music Thing Modular Workshop Computer.

Castle Process is a noisy chopper, mangler, and low-end pulse generator built around external audio, crude internal square sources, and a momentary performance bend on switch `Down`.

This is now behaving as a playable performance card, even though it is still an interpretation rather than a strict clone of the original Fort Processor.

---

# What It Does

Castle Process combines four ideas:

* distorted and chopped external audio input
* crude squarewave internal voice
* variable chopping between sources
* a separate bass pulse voice

The outputs are intentionally split by role:

* `Audio 1` carries the majority of the chopped external input character
* `Audio 2` carries the bass voice plus supporting texture

---

# Current Behaviour

Current hardware testing suggests:

* external audio is now behaving well as a choppy performance source
* switch `Up` and momentary switch `Down` both work well
* the card feels usable as a live texture/performance effect
* `Pulse 1` can be used to test and drive the bass voice directly
* `CV 1` and `CV 2` are still not the main focus of the present build

---

# Controls

## Main Knob

Sets overall drive and voicing behaviour.

## X Knob

Controls chopping character and speed feel.

## Y Knob

Controls tuning and interaction inside the engine.

## Switch Middle

Default mode.

This is the tighter chopped mode and the best starting point for general testing.

## Switch Up

Alternate latched mode.

This lets a little more body through and adds more squarewave colour, so it feels slightly fuller and less brutal than `Middle`.

## Switch Down

Momentary bend / chaos gesture while held.

This is intended as a live performance action rather than a third steady state.

---

# Inputs

## Audio In 1

Main external audio input.

This should sound chopped, broken up, and pushed into the machine rather than simply passed through cleanly.

## CV 1 / CV 2

These feed internal control relationships, but are not yet the strongest audible part of the design.

## Pulse In 1

Bass trigger input.

This is the best way to test the bass voice directly.

## Pulse In 2

Reserved for further interaction and future refinement.

---

# Outputs

## Audio 1

Primary chopped external-input output.

If you want to hear whether the input gating is working, this is the output to monitor first.

## Audio 2

Bass-focused output with additional supporting texture.

If you want to hear whether the bass voice is working, this is the output to monitor first.

## Pulse Out 1 / Pulse Out 2

Utility pulse outputs derived from internal activity.

---

# How To Test The Bass Properly

The clearest bass test is:

1. Monitor `Audio 2`.
2. Leave `Audio In 1` unplugged or very low.
3. Start in switch `Middle`.
4. Set `X` around noon.
5. Set `Y` low to mid.
6. Send a clear trigger stream into `Pulse In 1`.

What you should hear:

* a distinct low pulse for each incoming trigger
* less continuous drone than earlier versions
* pitch and character shifting as `Y` moves
* more instability if you hold switch `Down`

If `Pulse In 1` is patched and `Audio 2` still does not give clear low pulses, the bass section needs more work.

---

# Patch Ideas

* feed a drum loop or oscillator into `Audio In 1` and listen to `Audio 1` for chopped destruction
* use `Audio 2` as the low companion output
* strike `Pulse In 1` from a sequencer or clock divider to turn the bass voice into a rhythmic layer
* use switch `Down` as a manual performance accent

