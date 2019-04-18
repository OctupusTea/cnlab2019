const express = require('express');
const cookieParser = require('cookie-parser')
const bodyParser = require('body-parser');
const spawn = require('child_process').spawn;
const exec = require('child_process').exec;

let app = express();
app.use(bodyParser.urlencoded({ extended: true }));
app.use(cookieParser())

app.post('/login', (req, res) => {
    console.log(req.body);
    let name = req.body.name;
    let password = req.body.password;
    let ip = req.headers['x-forwarded-for'] || req.connection.remoteAddress;
    console.log(ip);
    if (name == 'team16' && password == 'mycnlab') {
        res.send('<h3> success </h3>');
        spawn('iptables', ['-t', 'nat', '-I', 'PREROUTING', '1', '-s', ip, '-j', 'ACCEPT']);
        spawn('iptables', ['-t', 'nat', '-I', 'PREROUTING', '1', '-d', ip, '-j', 'ACCEPT']);
        spawn('iptables', ['-I', 'FORWARD', '-s', ip, '-j', 'ACCEPT']);
        spawn('iptables', ['-I', 'FORWARD', '-d', ip, '-j', 'ACCEPT']);
    } else {
        res.send('<h3> wrong password </h3>');
    }
});

let cookieToken = 'P6wT`Tt7aJA/@j9#5oqi@';
app.get('/admin', (req, res) => {
    if (req.cookies.admin == cookieToken) {
        let cmd = exec('iptables -L FORWARD -vx', (error, stdout, stderr) => {
            res.setHeader('Content-type', 'text/html');
            if (error) {
                console.error(error);
                res.send(error);
            } else {
                console.log('done');
                let info = stdout;
                console.log(info);
                res.render(__dirname + '/admin.ejs', {info: info});
            }
        });
    } else {
        res.setHeader('Content-type', 'text/html');
        res.sendFile(__dirname + '/admin-login.html');
    }
});

app.post('/admin/login', (req, res) => {
    console.log(req.body);
    let name = req.body.name;
    let password = req.body.password;
    let ip = req.headers['x-forwarded-for'] || req.connection.remoteAddress;
    console.log(ip);
    if (name == 'domen' && password == 'OctupusTea') {
        res.cookie('admin', cookieToken); // Not secure
        res.location('/admin');
        res.send('<h3> success </h3>');
    } else {
        res.send('<h3> wrong password </h3>');
    }
});

app.get(/\/*/, (req, res) => {
    let hostUrl = req.get('host');
    console.log(hostUrl);
    if (hostUrl != '10.42.0.1:8888') {
        res.redirect(302, 'http://10.42.0.1:8888');
    }
    let ip = req.headers['x-forwarded-for'] || req.connection.remoteAddress;
    console.log(`${ip} is asking for wifi!`);
    res.setHeader('Content-type', 'text/html');
    res.sendFile(__dirname + '/index.html');
});

let port = 8888;
app.listen(port);
console.log(`start listening on port ${port}!`);
