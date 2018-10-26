var LFO = require('LFO');

exports.init = function ( ) {
  // input labels
  LABELS[0] = 'V/OCT';

  // param labels
  LABELS[12] = 'PW';

  // output labels
  LABELS[18] = 'SIN';
  LABELS[19] = 'TRI';
  LABELS[20] = 'SAW';
  LABELS[21] = 'SQUARE';
  LABELS[22] = 'NOISE';

  this.lfo = new LFO();
  console.log("loaded " + module.id);
};

exports.step = function ( ) {
  // if v/oct is active, set the pitch
  if (INPUTS[0].active) {
    this.lfo.setPitch(INPUTS[0].value);
  }

  // always set the sample time, as it may have changed
  this.lfo.step(engineSampleTime);

  // parameters are 0 - 100, but pulsewidth is 0 - 1
  this.lfo.setPulseWidth(PARAMS[0].value / 100);

  // set the outputs with the correct waveform
  OUTPUTS[0] = this.lfo.sin() * 5;
  OUTPUTS[1] = this.lfo.tri() * 5;
  OUTPUTS[2] = this.lfo.saw() * 5;
  OUTPUTS[3] = this.lfo.sqr() * 5;
  OUTPUTS[4] = this.lfo.noise() * 5;
};
