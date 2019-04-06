const express = require('express');
const bodyParser = require('body-parser');
const spawn = require('child_process').spawn;

let app = express();
app.use(bodyParser.urlencoded({ extended: true }));

app.get(/\/*/, (req, res) => {
    let ip = req.headers['x-forwarded-for'] || req.connection.remoteAddress;
    console.log(`${ip} is asking for wifi!`);
    res.setHeader('Content-type', 'text/html');
    res.sendFile(__dirname + '/index.html');
});

app.post('/login', (req, res) => {
    console.log(req.body);
    let name = req.body.name;
    let password = req.body.password;
    let ip = req.headers['x-forwarded-for'] || req.connection.remoteAddress;
    console.log(ip);
    if (true /*name == 'team16' && password == 'mycnlab'*/) {
        res.send('<h3> success </h3>');
        spawn('iptables', ['-t', 'nat', '-I', 'PREROUTING', '1', '-s', ip, '-j', 'ACCEPT']);
        spawn('iptables', ['-t', 'nat', '-I', 'PREROUTING', '1', '-d', ip, '-j', 'ACCEPT']);
        spawn('iptables', ['-I', 'FORWARD', '-s', ip, '-j', 'ACCEPT']);
        spawn('iptables', ['-I', 'FORWARD', '-d', ip, '-j', 'ACCEPT']);
    } else {
        res.send('<h3> wrong password </h3>');
    }
});


let port = 8888;
app.listen(port);
console.log(`start listening on port ${port}!`);
