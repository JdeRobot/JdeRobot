try{
	const yaml = require('js-yaml');
	const fs = require('fs');
} catch (e) {

}
let config = {};
var w;

		try {
			config = yaml.safeLoad(fs.readFileSync('public/config.yml', 'utf8'));
		} catch (e) {
			config.Server = "localhost";
			config.Port = "11000";
			console.log(e);
		}

    function startWorker(){
      if(typeof(Worker) !== "undefined") {
        if(typeof(w) == "undefined") {
          w = new Worker("js/3DViewer_worker.js");
          w.postMessage({func:"Start",server:config.Server, port:config.Port});
      }
      } else {
        Console.log("Sorry, your browser does not support Web Workers...");
      }
      w.onmessage = function(event) {
        if (event.data == "Connect"){
          console.log(event.data);
          setPoint();
        } else {
          console.log(event.data);
          w.terminate();
        }
      }
    }

    function stop(){
      if(typeof(w) != "undefined") {
        w.postMessage({func:"Stop"});
        w.onmessage = function(event) {
            w.terminate();
            w = undefined;
      }
    }
    }

    function setPoint(){
      w.postMessage({func:"setPoint"});
      w.onmessage = function(event) {
        addPoint(event.data);
    }
    }

    function clearAll(){
      if(typeof(w) == "undefined") {
        deleteObj();
        console.log("Clear all");
        startWorker();
      } else {
        w.postMessage({func:"ClearAll"});
        w.onmessage = function(event) {
          if (event.data == "ClearAll"){
          if(typeof(w) != "undefined") {
            w.terminate();
            w = undefined;
          }
          deleteObj();
          startWorker();
        }}
    }
  }
