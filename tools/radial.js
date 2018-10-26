const window = require('svgdom');
const SVG = require('svg.js')(window);
const document = window.document;

function getCoordinatesForPercent (percent) {
  percent -= 0.25;
  if (percent < 0) {
    percent += 1;
  }
  const x = Math.cos(2 * Math.PI * percent);
  const y = Math.sin(2 * Math.PI * percent);

  return [x, y];
}

function drawMarks (x, y, marks, context) {
  const radius = 16;


  for (var i = 0; i < marks.length; i++) {
    var coords = getCoordinatesForPercent(marks[i] / 100);
    var r = radius;
    var line = context.line();
    line.plot([ [ x, y ], [ (coords[0] * radius) + x, (coords[1] * radius) + y ] ]);
    line.stroke({ color: 'black', width: 0.75 });
  }
  var circle = draw.circle((radius - 3) * 2).move(x - (radius - 3), y - (radius - 3)).fill('#aaaaaa').stroke({ color: '#111111', width: 0.75 });
}

// larger radials
var marks = [ 0, 4.4, 8.8, 13.2, 17.6, 22, 26.4, 30.8, 35.2, 40, 95.6, 91.2, 86.8, 82.4, 77, 72.6, 68.2, 63.8, 59.4 ];

// smaller radius
var smallmarks = [ 60, 70, 80, 90, 0, 10, 20, 30, 40 ];

var draw = SVG(document.documentElement).size('165', '380');
draw.fill('#ffffff');
var rect = draw.rect(165, 380).fill('#ffffff');

drawMarks(112.5, 43, marks, draw);
drawMarks(112.5, 93, marks, draw);
drawMarks(112.5, 143, marks, draw);
drawMarks(112.5, 193, marks, draw);
drawMarks(112.5, 243, marks, draw);
drawMarks(112.5, 293, marks, draw);
drawMarks(90, 343, smallmarks, draw);

console.log(draw.svg());
