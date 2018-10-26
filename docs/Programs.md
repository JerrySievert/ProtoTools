# Programs

Programs in ProtoTools are simply Javascript modules built some some specific
methods.  You can learn more about the interface by reading about the [API](API.md).

Like normal modules in Javascript, each module is meant to be self-contained.
That means that anything that you instantiate and use in the scope of the module
is available for you to use.

## Developing Programs

Programs are fairly easy to use, and live inside of the `Rack/presets/ProtoTools`
directory.  In MacOS, this defaults to `~/Documents/Rack/presets/ProtoTools`.  It
will vary based on operating system.  For more information, see the VCVRack
documentation.

By default, programs are named after the module loading them, followed by a number.
For instance, Mix Tape programs would be numbered from `1` to `9`, and would be
named such as `mixtape_1.js`.

Whenever a program is selected, the previous program is unloaded, and the new
program is loaded from disk.  This allows you to make changes to your program
without restarting Rack itself.

### Initialization

Initialization occurs in the `init` method.  This is the entry point for when
a module gets loaded.  This is a good opportunity to set the labels and name
of the program.

```
var lfo = new LowFrequencyOscillator();

function init ( ) {
  // set up some defaults and other things here
  LABELS[0] = 'V/OCT';
  LABELS[18] = 'OUT';

  NAME = 'LFO';
}

exports.init = init;
```

### Stepping

Each step calls the `step` method, after setting up any paramters and inputs.

```
function step ( ) {
  lfo.step(engineGetSampleTime);
  OUTPUTS[0] = lfo.sin() * 5;
}

exports.step = step;
```
