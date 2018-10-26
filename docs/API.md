# Javascript API

This is the Javascript API for ProtoTools.  ProtoTools uses `Node.js` style
modules, which expose an `init` method and a `step` method.

```
function init ( ) {
  // initialize the module
}

function step ( ) {
  // called for each step
}

exports.init = init;
exports.step = step;
```

See the documentation for each VCVRack Module for specifics, as there will be
some differences.

## `bootstrap.js`

During initialization, the Javascript environment will be set up by executing
the contents of `bootstrap.js`.  This will set up the `require` command, as
well as a basic `console` object that contains `log`, `warn`, and `error`.

## `require`

`ProtoTools` provides a very basic `require` function that simply reads and
evaluates a file.  See the documentation for `require` in [`Modules`](Modules.md).

## Module Initialization

At program selection, `init()` will be called.  It is responsible for setting up
your local Javascript environment, as well as setting `LABELS` for the module.

## Module Step

Each step (or sometimes every other step) that VCVRack makes calls `step()`.
This is where your program interacts with VCVRack by examining inputs, knobs,
and sending output.

## Variables

There are several variables that you will have access to either in `init()` or
in `step()`.

### `LABELS`

`LABELS` is an `array` of labels for the module itself.  These labels are only
set when the `program` is initialized, and should be set in the `init()`
function.  _Note_ that this will truncate to the first `7` characters.

```
function init ( ) {
  LABELS[0] = 'V/OCT';
}
```

### `NAME`

`NAME` is a `String` that contains the name of the `program` that you have loaded.
If this is set, the `program` name of the module will be set.  _Note_ that this
will truncate to the first `7` characters.  This will only be set in `init()`.

### `engineSampleRate`

`engineSampleRate` is the sample rate that the module is currently running at.
_Note_ that this may be one half of the actual VCVRack engine sample rate.

### `engineSampleTime`

`engineSampleTime` is the number of milliseconds between each call to `step()`.
_Note_ that this may be twice the actual VCVRack engine sample time.

### `INPUTS`

`INPUTS` are an `array` of `objects` that represents the current state of any
inputs on the module itself.  These objects have two properties: `active` and
`value`:

```
{
  "active": true,
  "value": -1.043
}
```

`active` will be `true` when there is a cable plugged into the input in
VCVRack, and `value` will be set to the input value.

```
var voct = INPUTS[0].active ? INPUTS[0].value : Number.NaN;
```

The size of this `array` will vary per VCVRack module, so check the documentation
for each module.

### `PARAMS`

`PARAMS` are an `array` of `objects`, just like `INPUTS` except that `active`
will always be `true`.  Like `INPUTS`, this size will vary by module.  These
knobs are meant to be generic, thus will have values between `0` and `100`.

```
var cv_input = PARAMS[0].value;
```

### `OUTPUTS`

`OUTPUTS` are simply an `array`.  This `array` will be used to send output to
the ports they are mapped to in `VCVRack`.

```
OUTPUTS[0] = lfo.sin();
```
