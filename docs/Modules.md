# Modules for ProtoTools

ProtoTools uses a simplified `require` system for modules.  This means that there
is no `package.json`, and any modules are essentially _flat_.

Modules live in the `ProtoTools/modules` directory.

ProtoTools ships with a few modules to help with simple development.

## LowFrequencyOscillator

The LowFrequencyOscillator is a general oscillator that outputs a variety of
waveforms: `sine`, `triangle`, `sawtooth`, `square`, and `noise`.

### Usage

The Low Frequency Oscillator can be used with `require`:

```
var LFO = require('LFO');

var oscillator = new LFO();
```

#### `step`

`step()` is executed for each call into the module.  It requires the time between
executions, which is supplied to you as `engineSampleTime`.

```
oscillator.step(engineSampleTime);
```

#### `sin`

Returns the current position of a sine wave, with values between `-1` and `1`.

```
OUTPUTS[0] = oscillator.sin() * 5;
```

#### `tri`

Returns the current position of a triangle wave, with values between `-1` and `1`.

```
OUTPUTS[1] = oscillator.tri() * 5;
```

#### `saw`

Returns the current position of a saw wave, with values between `-1` and `1`.

```
OUTPUTS[2] = oscillator.saw() * 5;
```

#### `sqr`

Returns the current position of a square wave, with values either `-1` or `1`.

```
OUTPUTS[3] = oscillator.sqr() * 5;
```

#### `noise`

Returns a random number between `-1` and `1`.

```
OUTPUTS[4] = oscillator.noise() * 5;
```

#### `setPulseWidth`

Sets the pulse width for the square wave.  Values can be between `0` and `1`.

```
oscillator.setPulseWidth(PARAMS[0].value);
```

#### `setPitch`

Sets the pitch, which gets translated into `frequency`.  Values can be between
`-5` and `5`.

```
oscillator.setPitch(INPUTS[0].value);
```

#### `setFreqency`

Sets the frequency of the oscillator.

```
oscillator.setFrequency(220);
```


## CV

A simple CV module, used for triggers and other things.

### Usage

```
var CV = require('CV');

var trigger = new CV(1.7);
```

#### Constructor

`CV` allows for tracking of triggers, and thus requires a voltage level for
triggers to occur.  This is passed into the constructor:

```
var trigger = new CV(0.2); // triggers at 0.2
```

#### `update`

Updates the `CV` module with a new value.  This should occur on every step.

```
trigger.update(INPUTS[1].value);
```

#### `newTrigger`

Returns `true` or `false` as to whether the last `update()` caused a new trigger
to fire.

```
if (trigger.newTrigger()) {
  // ... do something
}
```

#### `currentValue`

The most recent value passed in via `update`.

#### `triggerInterval`

The number of steps between triggers.  This can be variable, but given a solid
source, it can be used to determine BPM.  It is calculated from the last two
triggers, and will not be accurate until at least the second trigger has fired.

#### `triggerCount`

Returns the number of times the trigger has been fired.

#### `isHigh`

Returns `true` if the value is currently above the trigger threshold.

#### `isLow`

Returs trus if the value is currently below the trigger threshold.

#### `reset`

Resets the CV object to fresh.
