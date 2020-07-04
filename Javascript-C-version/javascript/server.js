// Author: Stevedan OGochukwu Omodolor
// Project: Bachelor thesis- Simulaton and control of a robot
// University: KU Leueven/ UPC EEBE
// Tutor : Herman Bruyninckx


const {WebSocketServer} = require("@finos/perspective");
new WebSocketServer({assets: [__dirname]}, ()=>console.log(`Listening on port ${server.address().port}`));
