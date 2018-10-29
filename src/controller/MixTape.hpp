#include <cstdint>

#include "../ProtoTools.hpp"
#include "duktape.h"

struct InputStatus {
  float value;
  bool active;
};


struct MixTapeModule : Module {
  enum ParamIds { KNOB, PROGRAM = 6, NUM_PARAMS };
  enum InputIds { IN, NUM_INPUTS = 12 };
  enum OutputIds { OUT, NUM_OUTPUTS = 6 };
  enum LightIds { NUM_LIGHTS };

  MixTapeModule();
  void step() override;

  void loadProgram (int programId);
  void setupVariables ( );
  void getOutputs ( );
  void setupLabels ( );
  void setVariables ( );
  void setupStates ( );

  // javascript context for script execution
  duk_context *ctx;

  // labels
  char labels[32][32];

  InputStatus states[24];

  int currentProgram = -1;
  float engineSR;
  float engineST;
  bool skip = false;

};
