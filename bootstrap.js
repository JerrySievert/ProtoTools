// bootstraps base functionality, using some `native` methods

// console
const console = {
  log: function log ( ) {
    var arr = Array.prototype.slice.call(arguments);
    arr.unshift('LOG:');
    ProtoTools.print.apply(undefined, arr);
  },
  warn: function warn ( ) {
    var arr = Array.prototype.slice.call(arguments);
    arr.unshift('WARN:');
    ProtoTools.print.apply(undefined, arr);
  },
  error: function error ( ) {
    var arr = Array.prototype.slice.call(arguments);
    arr.unshift('ERROR:');
    ProtoTools.print.apply(undefined, arr);
  },
  debug: function debug ( ) {
    var arr = Array.prototype.slice.call(arguments);
    arr.unshift('DEBUG:');
    ProtoTools.print.apply(undefined, arr);
  }
};

function require (id) {
  var module = { id: id };
  var exports = { };

  module.exports = exports;

  try {
    file = ProtoTools.readfile(ProtoTools.basedir + '/modules/' + id + '.js');
    eval(file);
  } catch (err) {
    // considered fatal
    throw new Error('Cannot load module \'' + id + '\': ' + err);
  }

  return exports;
}

// load a program
ProtoTools.loadProgram = function loadProgram (id) {
  for (var i = 0; i < 32; i++) {
    LABELS[i] = '';
  }

  try {
    // break out to the presets directory, and load it by id
    var loaded = require('../../../presets/ProtoTools/' + id);

    // if it is loaded, and is really a module
    if (typeof(loaded) === 'object') {
      ProtoTools.currentProgram = loaded;

      if (ProtoTools.currentProgram.init && typeof(ProtoTools.currentProgram.init === 'function')) {
        ProtoTools.currentProgram.init();
      }
    } else {
      ProtoTools.currentProgram = { };
    }
  } catch (err) {
    console.warn("Unable to load program '" + id + "'");
    console.warn(err);
    ProtoTools.currentProgram = { };
  }
};

ProtoTools.step = function step ( ) {
  if (typeof(ProtoTools.currentProgram.step) === 'function') {
    ProtoTools.currentProgram.step();
  }
};

// set the default program to an empty program
ProtoTools.currentProgram = { };

console.log('Javascript boostrap initiated');
