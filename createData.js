function createData (num_pts){
  var xs = [];
  var ys = [];
    for(let i=0; i<=num_pts; i++){
      xs.push(i);
      ys.push(2*xs[i] + Math.random(0,1));
    }
    return xs,ys;
}

module.exports = createData;