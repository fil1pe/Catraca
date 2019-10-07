const express = require('express');
const path = require('path');
const app = express();
const bodyParser = require('body-parser');
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

const port = process.argv.length >= 3 ? process.argv[2] : 80;
if (process.argv.length < 3)
    console.log("Uso: node server.js <porta do servidor>");

var accesses = [];
var registers = [];

registers.addRegister = function(codigo, nome, cpf, tipo) {
    registers.push({ codigo, nome, cpf, tipo });
};

function addAccess(codigo, catraca, dateTime){
    var x;
    for (x=0; x<registers.length; x++)
        if (registers[x].codigo == codigo)
            break;
    if (x == registers.length) return;
    accesses.push([codigo, registers[x].nome, registers[x].cpf, catraca, dateTime]);
}

// Exemplos de registros:
registers.addRegister("1000", "Bianca Aparecida", "12345678999", "Professor");
registers.addRegister("1001", "Lucas Augusto", "12347678999", "Servidor");
var nextCode = 1002;
// --------------------------------

app.set('views', path.join(__dirname, 'static'));
app.set('view engine', 'jade');

function indexPage(res){
    res.status(200);
    res.render('index', {accesses: accesses, registers: registers});
}
function getRegisters(res){
    res.status(200);
    var to_print = '';
    for (var i=0; i<registers.length; i++)
        to_print += registers[i].codigo + ',' + registers[i].nome + ',' + registers[i].cpf + ',' + registers[i].tipo + ';';
    res.send(to_print);
}
function sendRegisters(res, nome, cpf, tipo){
    registers.addRegister((nextCode++).toString(10), nome, cpf, tipo);
    res.status(201);
    res.redirect('/');
}

app.post('/sendRegisters', (req, res) => sendRegisters(res, req.body.nome, req.body.cpf, req.body.tipo));
app.get('/getRegisters', (req, res) => getRegisters(res));
app.post('/accessed', (req, res) => {
    var newAccesses = JSON.parse(req.body.list);
    for (var i=0; i<newAccesses.length; i++)
        addAccess(newAccesses[i].codigo, newAccesses[i].catraca, newAccesses[i].dateTime);
    res.status(201).send('ok');
});
app.get('/index', (req, res) => indexPage(res));
app.get('/', (req, res) => indexPage(res));
app.get('/style.css', (req, res) => res.status(200).sendFile(path.join(__dirname, 'static/style.css')));
app.get('/scripts.js', (req, res) => res.status(200).sendFile(path.join(__dirname, 'static/scripts.js')));
app.get('*', (req, res) => res.status(404).render('404'));

app.listen(port, function(){
    console.log("Servidor rodando na porta " + port);
    console.log("http://localhost" + (port != 80 ? ":"+port : "") + "/");
});