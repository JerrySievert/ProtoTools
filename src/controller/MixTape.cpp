#include "MixTape.hpp"
#include <cstdio>
#include <cstring>
#include "../lib/javascript.hpp"

MixTapeModule::MixTapeModule() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
  ctx = createJavascriptContext(plugin->path.c_str());

  // set up LABELS
  duk_idx_t arr_idx;

  arr_idx = duk_push_array(ctx);
  for (int i = 0; i < 32; i++) {
    duk_push_string(ctx, "");
    duk_put_prop_index(ctx, arr_idx, i);
  }

  duk_put_global_string(ctx, "LABELS");

  engineSR = engineGetSampleRate();
  engineST = engineGetSampleTime();

  // set up inputs/params/outputs
  setupVariables();

  // set up initial states
  setupStates();
}

// copy input variables and parameters, set up outputs, sample rate
void MixTapeModule::setupVariables ( ) {
  // inputs, 12 of them
  duk_idx_t arr_idx = duk_push_array(ctx);

  for (int i = 0; i < 12; i++) {
    duk_idx_t obj_idx = duk_push_object(ctx);
    duk_push_string(ctx, "active");
    duk_push_boolean(ctx, inputs[i].active ? true : false);
    duk_put_prop(ctx, obj_idx);

    duk_push_string(ctx, "value");
    duk_push_number(ctx, inputs[i].value);
    duk_put_prop(ctx, obj_idx);

    duk_put_prop_index(ctx, arr_idx, i);
  }

  duk_put_global_string(ctx, "INPUTS");

  // params, 6 of them
  arr_idx = duk_push_array(ctx);

  for (int i = 0; i < 6; i++) {
    duk_idx_t obj_idx = duk_push_object(ctx);
    duk_push_string(ctx, "value");
    duk_push_number(ctx, params[i].value);
    duk_put_prop(ctx, obj_idx);

    duk_put_prop_index(ctx, arr_idx, i);
  }

  duk_put_global_string(ctx, "PARAMS");

  // outputs, 6 of them
  arr_idx = duk_push_array(ctx);

  for (int i = 0; i < 6; i++) {
    duk_idx_t obj_idx = duk_push_object(ctx);
    duk_push_string(ctx, "active");
    duk_push_boolean(ctx, outputs[i].active ? true : false);
    duk_put_prop(ctx, obj_idx);

    duk_push_string(ctx, "value");
    duk_push_number(ctx, outputs[i].value);
    duk_put_prop(ctx, obj_idx);

    duk_put_prop_index(ctx, arr_idx, i);
  }

  duk_put_global_string(ctx, "OUTPUTS");

  duk_push_number(ctx, engineGetSampleRate() / 2);
  duk_put_global_string(ctx, "engineSampleRate");

  duk_push_number(ctx, engineGetSampleTime() * 2);
  duk_put_global_string(ctx, "engineSampleTime");

  duk_pop(ctx);
}

void MixTapeModule::setupLabels ( ) {
  if (duk_get_global_literal(ctx, "LABELS")) {
    duk_size_t length = duk_get_length(ctx, -1);

    for (duk_size_t i = 0; i < length && i < 24; i++) {
      duk_get_prop_index(ctx, -1, i);
      strncpy(labels[i], duk_get_string(ctx, -1), 7);
      labels[i][7] = '\0';

      duk_pop(ctx);
    }

    duk_pop(ctx);
  }
}

void MixTapeModule::setVariables ( ) {
  if (duk_get_global_literal(ctx, "INPUTS")) {
    for (duk_size_t i = 0; i < duk_get_length(ctx, -1) && i < 12; i++) {
      duk_idx_t prop_idx = duk_get_prop_index(ctx, -1, i);

      if (states[i].active != inputs[i].active) {
        states[i].active = inputs[i].active;
        duk_push_boolean(ctx, states[i].active);
        duk_put_prop_string(ctx, prop_idx, "active");
      }

      if (states[i].value != inputs[i].value) {
        states[i].value = inputs[i].value;
        duk_push_number(ctx, states[i].value);
        duk_put_prop_string(ctx, prop_idx, "value");
      }

      duk_pop(ctx);
    }

    duk_pop(ctx);
  }

  if (duk_get_global_literal(ctx, "PARAMS")) {
    for (duk_size_t i = 0; i < duk_get_length(ctx, -1) && i < 6; i++) {
      duk_idx_t prop_idx = duk_get_prop_index(ctx, -1, i);

      if (states[i + 12].value != params[i].value) {
        states[i + 12].value = params[i].value;
        duk_push_number(ctx, states[i + 12].value);
        duk_put_prop_string(ctx, prop_idx, "value");
      }

      duk_pop(ctx);
    }

    duk_pop(ctx);
  }

  if (engineSR != engineGetSampleRate()) {
    duk_push_number(ctx, engineGetSampleRate() / 2);
    duk_put_global_string(ctx, "engineSampleRate");
    engineSR = engineGetSampleRate();
  }

  if (engineST != engineGetSampleTime()) {
    duk_push_number(ctx, engineGetSampleTime() * 2);
    duk_put_global_string(ctx, "engineSampleTime");
    engineST = engineGetSampleTime();
  }
}

void MixTapeModule::getOutputs ( ) {
  if (duk_get_global_literal(ctx, "OUTPUTS")) {
    duk_size_t length = duk_get_length(ctx, -1);

    for (duk_size_t i = 0; i < length && i < 6; i++) {
      if (outputs[i].active) {
        duk_get_prop_index(ctx, -1, i);
        outputs[i].value = duk_get_number(ctx, -1);
        duk_pop(ctx);
      } else {
        outputs[i].value = 0.0f;
      }
    }
  }

  duk_pop(ctx);
}

void MixTapeModule::setupStates ( ) {
  for (int8_t i = 0; i < 18; i++) {
    if (i < 12) {
      // inputs
      states[i].active = inputs[i].active;
      states[i].value = inputs[i].value;
    } else {
      // params
      states[i].active = true;
      states[i].value = params[i - 12].value;
    }
  }
}

void MixTapeModule::step() {
  static bool skip = false;

  if (skip) {
    skip = false;
    return;
  } else {
    skip = true;
  }

  // set the current variables
  setVariables();

  // check to see if the program has changed
  float program = params[PROGRAM].value;
  if ((int) program != currentProgram) {
    char buf[256];

    sprintf(buf, "ProtoTools.loadProgram('mixtape_%d');", (int) program);
    if (duk_peval_string(ctx, buf) != 0) {
      fprintf(stderr, "eval failed: %s\n", duk_safe_to_string(ctx, -1));
    }

    duk_pop(ctx);

    // labels should only change on module init(), so that's when they get processed
    setupLabels();

    currentProgram = program;
  }

  // do the step
  if (duk_peval_string(ctx, "ProtoTools.step();") != 0) {
    fprintf(stderr, "eval failed: %s\n", duk_safe_to_string(ctx, -1));
  }

  duk_pop(ctx);

  // send the output results
  getOutputs();
}
