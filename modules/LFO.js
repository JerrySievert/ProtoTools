function LowFrequencyOscillator ( ) {
  this.phase = 0;
  this.pw = 0.5;
  this.freq = 1;
}

LowFrequencyOscillator.prototype.setPitch = function setPitch (pitch) {
  pitch = Math.min(pitch, 8.0);
  this.freq = 261.626 * Math.pow(2, pitch);
};

LowFrequencyOscillator.prototype.setFrequency = function setFrequency (frequency) {
  this.freq = frequency;
};

LowFrequencyOscillator.prototype.setPulseWidth = function setPulseWidth (pw) {
  this.pw = Math.max(Math.min(pw, 0.99), 0.01);
};

LowFrequencyOscillator.prototype.step = function step (dt) {
  var deltaPhase = Math.min(this.freq * dt, 0.5);
  this.phase += deltaPhase;

  if (this.phase >= 1) {
    this.phase -= 1;
  }
};

LowFrequencyOscillator.prototype.sin = function sin ( ) {
  return Math.sin(2 * Math.PI * this.phase);
};

LowFrequencyOscillator.prototype.tri = function tri ( ) {
  var x = this.phase - 0.75;

  return (4 * Math.abs(x - Math.round(x))) - 1;
};

LowFrequencyOscillator.prototype.saw = function saw ( ) {
  return 2 * (this.phase - Math.round(this.phase));
};

LowFrequencyOscillator.prototype.sqr = function sqr ( ) {
  return (this.phase < this.pw) ? 1 : -1;
};

LowFrequencyOscillator.prototype.noise = function noise ( ) {
  return (Math.random() - 0.5) * 2;
};

module.exports = exports = LowFrequencyOscillator;
