const express = require('express');
const http = require('http');
const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');
const bodyParser = require('body-parser');
const cors = require('cors');
const app = express();
const server = http.createServer(app);
const jsonParser = bodyParser.json();

const port = new SerialPort({
  path: "COM4",
  baudRate: 9600,
}, (err) => {
  if (err) {
    return console.log('Error on create:', err.message);
  }
});

const parser = port.pipe(new ReadlineParser({ delimiter: '\r\n' }));

parser.on('error', (err) => {
  console.log('Error:', err.message);
});

parser.on('data', (data) => {
  console.log("Response:", data);
});

app.use(express.static('css'));

port.on('open', () => {
  console.log('Serial port is open');
});

app.use(cors());

app.post('/change', jsonParser, (req, res) => {
  const { mode, direction, duration } = req.body;

  console.log(`STR: ${mode}D${duration}S${direction}\n`);

  port.write(`M${mode}D${duration}S${direction}\n`, (err) => {
    if (err) {
      console.error('Error writing to serial port:', err);
      res.status(500).json({ message: 'Error writing to serial port' });
    } else {
      console.log(`Mode set: ${mode}, Duration set: ${duration}, Direction set: ${direction}`);
      res.json({ message: 'Success' });
    }
  });
});

app.get('/', (req, res) => {
  res.sendFile(__dirname + '/lightshow.html');
});

server.listen(3000, () => {
  console.log('Server running on port 3000');
});
