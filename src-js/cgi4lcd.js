udp = require("dgram");

Queue = function() {

  this.commands = [];

  this.add = function(command) {

    var now = new Date().valueOf();

    if (typeof commands[command.line()] == "undefined") {
      commands[command.line()].response = "";
      commands[command.line()].last_request = now;

      commands[command.line()] = command;

      if (commands[command.line()].add_and_run) {
        commands[command.line()].run();
      }

    }
    else {
      commands[cmd.line()].last_request = now;
    }

  };

  this.run = function() {

    console.log("Processing " + this.commands.size + " commands");

    for (line in commands) {
      var now = new Date().valueOf();

      if (now >= commands[line].last_request + command.timeout) {
        console.log("Erasing '" + line + "'");
        delete commands[line];
      }
      else if (now >= commands[line].last_execution + command.interval) {
        console.log("Running '" + line + "'");
        commands[line].run();
        commands[line].last_execution = new Date().valueOf();
        console.log("Response: '" + commands[line].response);
      }

    }

  }

  this.get = function(command) {
    return commands[command.line()];
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
    this.response = "Running now: '" + this.line() + "'";
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
        cmd.is_malformed = true;
      }
      else if (packet[1] == "command" && packet[2] != "") {
        command.is_internal = true;
        command.executable = packet[1];
        command.arguments = packet[2];
      }
      else {
        cmd.executable = packet[1];
        cmd.arguments = packet[2];
        cmd.interval = parseInt(packet[3]);
        cmd.timeout = parseInt(packet[4]);
        cmd.do_not_queue = packet[5] == "1";
        cmd.add_and_run = packet[6] == "1";
        cmd.is_malformed = false;
      }        
    }

    return command;
  }

}

var server = udp.createSocket("udp4");
var queue = new Queue;
var protocol = new Protocol;

server.on("message", function (data, rinfo) {
  command = protocol.parse(data);

  if (!command.is_malformed) {
    if (command.do_not_queue) {
      command.run();
    }
    else {
      queue.add(command);
      command = queue.get(command);
    }
  }

  var message = new Buffer(command.response);
  server.send(message, 0, message.length, rinfo.port, rinfo.address);

});

setInterval(queue.run, 1000);
server.bind(65432);