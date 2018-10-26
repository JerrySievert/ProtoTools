// CV module, based on SynthDevKit CV module

/*
class CV {
  public:
    void update (float);
    uint32_t triggerInterval ( );
    uint32_t triggerTotal ( );
    bool isHigh ( );
    bool isLow ( );
};
*/

function CV (threshold) {
  this.threshold = threshold;
  reset();
}

CV.prototype.update = function update (current) {
  this.lastValue = current;

  this.triggerIntervalCount++;

  if (current >= this.threshold) {
    if (this.triggered === false) {
      this.triggered = true;

      this.triggerCount++;

      this.lastTriggerInterval = this.triggerIntervalCount;

      this.triggerIntervalCount = 0;
    }
  } else {
    this.triggered = false;
  }
};

CV.prototype.newTrigger = function newTrigger ( ) {
  if (this.triggered === true && this.lastTriggered === false) {
    this.lastTriggered = true;

    return true;
  }

  this.lastTriggered = this.triggered;

  return false;
};

CV.prototype.reset = function reset ( ) {
  this.triggered = false;
  this.lastTriggered = false;
  this.lastValue = 0;
  this.triggerCount = 0;
  this.triggerIntervalCount = 0;
  this.lastTriggerInterval = 0;
};

CV.prototype.currentValue = function ( ) {
  return lastValue;
};

CV.prototype.isLow = function isLow ( ) {
  return !this.triggered;
};

CV.prototype.isHigh = function isHigh ( ) {
  return this.triggered;
};

CV.prototype.triggerInterval = function ( ) {
  return this.lastTriggerInterval;
};

CV.prototype.triggerTotal = function ( ) {
  return this.triggerCount;
};

module.exports = exports = CV;
