#include "../controller/MixTape.hpp"

#include "../lib/components.hpp"

struct MixTapeWidget : ModuleWidget {
  MixTapeWidget(MixTapeModule *module);
};

MixTapeWidget::MixTapeWidget(MixTapeModule *module) : ModuleWidget(module) {
  box.size = Vec(12 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

  {
    SVGPanel *panel = new SVGPanel();
    panel->box.size = box.size;
    panel->setBackground(SVG::load(assetPlugin(plugin, "res/MixTape.svg")));
    addChild(panel);
  }


  for (int i = 0; i < 6; i++) {
    addInput(Port::create<ProtoPort>(Vec(10, 35 + (i * 50)), Port::INPUT, module,
                                  MixTapeModule::IN + i));

    addInput(Port::create<ProtoPort>(Vec(55, 35 + (i * 50)), Port::INPUT, module,
                                  MixTapeModule::IN + i + 6));

    addParam(ParamWidget::create<ProtoKnob>(
        Vec(100, 30 + (i * 50)), module, MixTapeModule::KNOB + i, 0.0f, 100.0f, 0.0f));

    addOutput(Port::create<ProtoPort>(Vec(145, 35 + (i * 50)), Port::OUTPUT, module,
                                   MixTapeModule::OUT + i));

    // labels
    for (int j = 0; j < 4; j++) {
      {
        LabelDisplay *label = new LabelDisplay();
        label->value = module->labels[i + (j * 6)];
        label->box.pos = Vec(22.5 + (45 * j), 70 + (50 * i));
        label->box.size = Vec(10, 18);
        addChild(label);
      }
    }
  }

  addParam(ParamWidget::create<ProtoKnobSnap>(
      Vec(77.5, 330), module, MixTapeModule::PROGRAM, 1.0f, 9.0f, 1.0f));
}

Model *modelMixTape = Model::create<MixTapeModule, MixTapeWidget>("Proto Tools", "Mix Tape", "Mix Tape",
                                                   UTILITY_TAG);
