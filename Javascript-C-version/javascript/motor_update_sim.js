// Author: Stevedan OGochukwu Omodolor
// Project: Bachelor thesis- Simulaton and control of a robot
// University: KU Leueven/ UPC EEBE
// Tutor : Herman Bruyninckx



// Update every N milliseconds
const TICK_RATE = 50;
// Size limit of the table
const TABLE_SIZE = 200;
const NUMBER_MEMBER_STRUCT = 10;
const POSITION_CONTROL = 2;
const MILLISECONDS_SECONDS_CONVER = 1000;
// var fps = 60;

// requestAnimationFrame
var requestAnimationFrame = window.requestAnimationFrame ||
      window.mozRequestAnimationFrame ||
      window.webkitRequestAnimationFrame ||
      window.msRequestAnimationFrame

var cancelAnimationFrame = window.cancelAnimationFrame ||
      window.mozCancelAnimationFrame


// initializing simulation variables
var controler = 0;
var state = "run";
var pos_output = 0;
var time = 0;

// restart function calling from c
function restart() {
      console.log("You restarted")
      _restart();
      controler = 0;
      state = "run";

}

// initialize computattion function
const init_comp = () => {

      console.log("Computation initialized");
      _restart();
}


// main update function
function update_motor() {

      // Geting user's input
      var rows = [];
      var Vbus = document.getElementById('vbus').value * 1.0;
      var rpm = document.getElementById('rpm').value * 1.0;
      var slidersetpos = document.getElementById('pos').value * 1.0;
      var loadt = document.getElementById('loadt').value * 1e-2;
      var loadj = document.getElementById('loadj').value * 1e-3;

      // Storing data read from webassembly heap memory into a javascript Array 

      let array_result = new Float32Array(Module.HEAPF64.buffer, _computational_data(Vbus, loadt, loadj, slidersetpos, rpm, controler), NUMBER_MEMBER_STRUCT);


      if (controler == POSITION_CONTROL) {
            pos_output = array_result[5];
            
      } else {
            pos_output = 180;
      }



      // Updating Perspective table's row 
      rows.push({
            lastUpdate: new Date(),
            comp_time: array_result[0], // time of simulation in seconds
            va: array_result[1], //array_result[1],
            vc: array_result[2],
            vb: array_result[3],
            rpm: array_result[4],
            position: pos_output, // For visualitzation purpose
            iq: array_result[6],
            id: array_result[7],
            frequency: array_result[8],
            avg_frequency: array_result[9]

      });


      return (rows);

}

// execute only when webassembly is ready

window.addEventListener("WebComponentsReady", function () {
      var elem = document.getElementsByTagName("perspective-viewer")[0];
      // elem.style.color = "#ff0000";
      // elem.restyleElement();
      var worker = perspective.worker();
      var schema = {
            lastUpdate: "datetime",
            comp_time: "float", // time of simulation in seconds
            va: "float",
            vc: "float",
            vb: "float",
            rpm: "float",
            position: "float", // curent position
            iq: "float",
            id: "float",
            frequency: "integer",
            avg_frequency: "integer"

      };
      const table = worker.table(schema, {
            limit: TABLE_SIZE
      });
      elem.load(table);
      elem.toggleConfig();
      init_comp();

      // // function postRow(timestamp) {
      // //       setTimeout(function() {
      // //             if (state == "run") {
      // //                   table.update(update_motor());

      // //             }


      // //             requestAnimationFrame(postRow)
      // //       }, MILLISECONDS_SECONDS_CONVER / fps)
      // // }

      // requestAnimationFrame(postRow)

      (function postRow() {
            if (state == "run") {
                  table.update(update_motor());

            }
            requestAnimationFrame(postRow);
            // setTimeout(postRow, 20);

      })();

}, {
      passive: true,
      capture: true
});
