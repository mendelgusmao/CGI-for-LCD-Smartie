var udp = require("dgram");
var exec = require('child_process').exec;

Queue = function() {

  this.commands = {};

  this.now = function() {
    return new Date().valueOf() / 1000;
  };

  this.add = function(command) {

    if (typeof this.commands[command.line()] == "undefined") {
      command.last_request = this.now();

      this.commands[command.line()] = command;

      if (this.commands[command.line()].add_and_run) {
        this.commands[command.line()].run();
      }

    }
    else {
      this.commands[command.line()].last_request = this.now();
    }

  };

  this.run = function() {

    for (line in this.commands) {

      if (this.now() >= this.commands[line].last_request + this.commands[line].timeout) {
        delete this.commands[line];
      }
      else if (this.now() >= this.commands[line].last_execution + this.commands[line].interval) {
        this.commands[line].run();
        this.commands[line].last_execution = this.now();
      }

    }

  }

  this.get = function(command) {
    return this.commands[command.line()];
  }

}

Command = function() {

  this.executable = "";
  this.arguments = "";
  this.timeout = 0;
  this.interval = 0;
  this.last_execution = 0;
  this.last_request = 0;
  this.response = "";
  this.is_malformed = false;
  this.is_internal = false;
  this.do_not_queue = false;
  this.add_and_run = true;

  this.line = function() {
    return this.executable + " " + this.arguments;
  };

  this.run = function() {
    var _this = this;
    exec(this.line(), function(error, stdout, stderr) {
      if (error == null) {
        _this.response = stdout;
      }
    });
  };

};

Protocol = function() {
  
  this.protocol_delimiter = "|";
  this.protocol_header = "cgi4lcd";
  this.protocol_expected_size = 8;

  this.parse = function(data) {

    var command = new Command;
    var message = data.toString().split(this.protocol_delimiter);

    if (message.length != this.protocol_expected_size) {
      command.is_malformed = true;
    }
    else {
      if (message[0] != this.protocol_header) {
        command.is_malformed = true;
      }
      else if (message[1] == "command" && message[2] != "") {
        command.is_internal = true;
        command.executable = message[1];
        command.arguments = message[2];
      }
      else {
        command.executable = message[1];
        command.arguments = message[2];
        command.interval = parseInt(message[3]);
        command.timeout = parseInt(message[4]);
        command.do_not_queue = message[5] == "1";
        command.add_and_run = message[6] == "1";
        command.is_malformed = false;
      }        
    }

    return command;
  }

}

var server = udp.createSocket("udp4");
var queue = new Queue();
var protocol = new Protocol;

server.on("message", function (data, rinfo) {

  var command = protocol.parse(data.toString());

  if (!command.is_malformed) {
    if (command.do_not_queue) {
      command.run();
    }
    else {
      queue.add(command);
      command = queue.get(command);
    }
  }

  if (command.response == "") {
    command.response = "-"
  };

  var message = new Buffer(command.response);

  server.send(message, 0, message.length, rinfo.port, rinfo.address);

});

setInterval(function() { queue.run(); }, 1000);
server.bind(65432);