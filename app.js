const axios = require('axios');
const tf = require('@tensorflow/tfjs-node');
const fs = require('fs');
const csv = require('csv-parser');

var rssis = [];
var labels = [];
var xs = [];
var ys = [];
var model;

function readCSV() {
  return new Promise(function(resolve, reject) {
    fs.createReadStream('dataGateway.csv')
    .pipe(csv())
    .on('data', (row) => {
        rssis.push( [parseInt(row.G1), parseInt(row.G2), parseInt(row.G3), parseInt(row.G4)] )
        labels.push( parseInt(row.label) )
    })
    .on('end', () => {
        console.log('CSV file successfully processed');
        resolve( labels )
    });
  });
}

async function prepareData(){
  await readCSV();
  
  xs = tf.tensor2d(rssis);
  const labelsTensor = tf.tensor1d(labels, 'int32');
  ys = tf.oneHot(labelsTensor,4);
  // xs.print();
  // ys.print();
  // console.log(labels);
}

function model() {
  model = tf.sequential();

  model.add(tf.layers.dense({
    units: 16,
    activation: 'sigmoid',
    inputShape: [4],
  }));
  model.add(tf.layers.dense({
    units: 4,
    activation: 'softmax',
  }));

  const lr = 0.1;
  const optimizer = tf.train.sgd(lr);
  model.compile({
    optimizer: optimizer,
    loss: 'categoricalCrossentropy',
  })
  
  train();
  
}

async function train() {
  const options = {
    epochs: 50,
    validationSplit: 0.1,
    shuffle: true,
    callbacks : {
      onEpochEnd: (num,logs) => {
        console.log(logs);
      }
    }
  }
  await model.fit(xs,ys,options)
  await model.save('file://model');
}

async function postModel() {
  var body = {}

  // await axios({
  //   method: 'post',
  //   url: '/addUser',
  //   data: body
  // })
  // .then(function (response) {
  //   console.log(response);
  // })
  // .catch(function (error) {
  //   console.log(error);
  // });


} 

async function predict(){
  axios.get('http://webcode.me').then(resp => {
    console.log(resp.data);
  });
  let xv = tf.tensor2d([[-74,-43,-50,-53]]);
  let results = model.predict(xv);
  let index =results.argMax(1);
  index.print(); 

}

async function run(){
  await prepareData();
  await model();
  await postModel();
  await predict();
}

run();

