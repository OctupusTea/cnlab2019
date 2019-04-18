const express = require('express');
const cookieParser = require('cookie-parser')
const bodyParser = require('body-parser');
const spawn = require('child_process').spawn;
const exec = require('child_process').exec;
const low = require('lowdb');
const FileSync = require('lowdb/adapters/FileSync')


// utils
function bytesToSize(bytes) {
       var sizes = ['Bytes', 'KB', 'MB', 'GB', 'TB'];
          if (bytes == 0) return '0 Byte';
             var i = parseInt(Math.floor(Math.log(bytes) / Math.log(1024)));
                return Math.round(bytes / Math.pow(1024, i), 2) + ' ' + sizes[i];
};

// db
const adapter = new FileSync('db.json')
const db = low(adapter)
db.defaults({ authorizedUsers: [] }).write()


// express
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
        let index = (db.get('authorizedUsers').value().length * 2 + 1).toString();
        console.log('index: ' + index);
        db.get('authorizedUsers').push(ip).write();
        spawn('iptables', ['-t', 'nat', '-I', 'PREROUTING', index, '-s', ip, '-j', 'ACCEPT']);
        spawn('iptables', ['-t', 'nat', '-I', 'PREROUTING', index, '-d', ip, '-j', 'ACCEPT']);
        spawn('iptables', ['-I', 'FORWARD', index, '-s', ip, '-j', 'ACCEPT']);
        spawn('iptables', ['-I', 'FORWARD', index, '-d', ip, '-j', 'ACCEPT']);
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
                let data = stdout;
                data = data.split('\n');
                data.shift();
                data.shift();
                data = data.map((rule) => rule.trim().split(/\s+/));
                let authorizedUsers = db.get('authorizedUsers').value();
                let info = {};
                data.forEach((rule) => {
                    let ip = '';
                    let type = '';
                    if (authorizedUsers.indexOf(rule[7]) != -1) {
                        ip = rule[7];
                        type = 'sent';
                    } else if (authorizedUsers.indexOf(rule[8]) != -1) {
                        ip = rule[8];
                        type = 'received';
                    } else {
                        return;
                    }
                    if (!info[ip]) info[ip] = {};
                    info[ip][type] = bytesToSize(rule[1]);
                });
                data = data.filter((rule) => authorizedUsers.indexOf(rule[7]) != -1 || authorizedUsers.indexOf(rule[8]) != -1);
                data = data.map((rule) => [bytesToSize(rule[1]), rule[7], rule[8]]);
                res.render(__dirname + '/admin.ejs', {authorizedUsers: authorizedUsers, info: info});
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

app.get(/^(?!(admin|login).*).*$/, (req, res) => {
    let hostUrl = req.get('host');
    if (hostUrl != '10.42.0.1:8888') {
        res.redirect(302, 'http://10.42.0.1:8888');
    }
    let ip = req.headers['x-forwarded-for'] || req.connection.remoteAddress;
    console.log(`${ip} is asking for wifi!`);
    res.setHeader('Content-type', 'text/html');
    res.sendFile(__dirname + '/index.html');
});

let port = 8888;
app.listen(port, '0.0.0.0');
console.log(`start listening on port ${port}!`);
