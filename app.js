const tf = require('@tensorflow/tfjs-node');
const plotly = require('plotly')("kotesa1","BBALHGgk9gQ55slloNod");

const num_pts = 50;
const num_nodes = 10;
const epochs = 30;

function createData (num_pts){
  var xs = [];
  var ys = [];
    for(let i=0; i<=num_pts; i++){
      xs.push(i);
      ys.push(2*xs[i] + Math.random(0,1));
    }
    return [xs,ys];
}

function createModel(num_nodes){
  const model = tf.sequential();
  model.add(tf.layers.dense({units: num_nodes, activation: 'relu', inputShape: [num_pts,1]}));
  model.compile({
    optimizer: 'sgd',
    loss: 'meanSquaredError',
  });
  return model;
}

function trainModel(model, xs, ys, epochs){
  const h = model.fit(xs, ys, {batchSize: 64, epochs: epochs});
  return h.history;
}

function predictModel(){
  var yv = [];
  var xv = model.predict(x_test);

  for(let i=0; i<=xv.length; i++){
    yv.push(2*xv[i] + Math.random(0,1));
  }

  return [xv,yv];
}

function plotResults(xv,yv){
  var data = [{x:xv, y:yv, type: 'bar'}];
  var layout = {fileopt : "overwrite", filename : "simple-node-example"};

  plotly.plot(data, layout, function (err, msg) {
	if (err) return console.log(err);
	  console.log(msg);
  });
}

async function main(){
  
  var train = createData(num_pts);
  const model = createModel(num_nodes);
  var loss = trainModel(model,train[0],train[1],epochs);
  console.log(loss);
  plotResults(results[0],results[1]);
}

main();