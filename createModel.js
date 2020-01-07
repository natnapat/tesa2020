function createModel(num_nodes){
  const model = tf.sequential();
  model.add(tf.layers.dense({units: num_nodes, activation: 'sigmoid'}));
  model.compile({optimizer: 'sgd', loss:'meanSquaredError'});
  
  return model;
}

module.exports = createModel;