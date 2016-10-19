var express = require('express');
var fs = require("fs");
var config = require('config');
var bodyParser = require('body-parser');
var mongoose = require('mongoose');

//Loading Models
var Step = require('./models/step')

//Config 
var dbConfig = config.get('mongodb');
var connectionString = 'mongodb://'+dbConfig.user+':'+dbConfig.password+'@'+dbConfig.url+':'+dbConfig.port+'/'+dbConfig.database;

mongoose.Promise = global.Promise;
mongoose.connect(connectionString);

var createServer = function(port) {
    var app = express();
   
    app.use(bodyParser.json()); // support json encoded bodies
    app.use(bodyParser.urlencoded({ extended: true })); // support encoded bodies
    app.use(function(req, res, next) {
       res.setHeader("Access-Control-Allow-Origin", "*");
       res.setHeader('Content-Type', 'application/json');
       return next();
    });

    app.get('/', function(req, res){
       res.send(JSON.stringify({ message : 'API up and running' }));
    });

    app.post('/step', function(req, res){
        var direction = req.body.direction;
        var gas_level = req.body.gas_level;

        var step = new Step({ direction: direction, gas_level: gas_level });
        step.save(function(err, step, numAffected) {
            if (err) return res.status(460).send({ error: err });
            res.send(JSON.stringify({ message : 'Step -> Direction: ' + step.direction + ' GasLevel '+ step.gas_level +' has been created.' , 'id': step._id }));
        }); 
    });

    app.delete('/step/:step_id', function(req, res){
       //TODO Validate
       var id = req.params.step_id;
       var query = Step.find().remove({ _id : id })
       query.exec()
       res.send(JSON.stringify({ message : 'Step ' + id + ' has been deleted.' }));
    });

    app.get('/step', function(req, res) {
        Step.find({}, function(err, steps) {
            var stepMap = {};
            steps.forEach(function(step) {
              stepMap[step._id] = step;
            });
        res.send(steps);  
      });
    });

    app.get('/step/latest', function(req, res) {

        Step.find({}).sort('-createdAt').limit(4).exec(function(err, step) { 
          res.send(step);
         });      
    });
    app.get('/step/latest/50', function(req, res) {
        Step.find({}).sort('-createdAt').limit(20).exec(function(err, step) { 
          res.send(step);
         });      
    });


    return app.listen(port, '0.0.0.0');
    //return app.listen(8081, '0.0.0.0', function () {
    //    var host = server.address().address
    //    var port = server.address().port
    //    console.log("Example app listening at http://%s:%s", host, port)
    //});

};

module.exports = createServer;
