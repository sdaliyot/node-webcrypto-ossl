"use strict";

var worker_threads = require('worker_threads');

const worker1 = new worker_threads.Worker(`
  process.umask = () => 0;
  const path = require('path');
  const mochaPath = path.join(process.env.USERPROFILE, '.vscode/extensions/maty.vscode-mocha-sidebar-0.20.24/node_modules/mocha/bin/_mocha');
  process.argv = [ process.argv0, mochaPath, 'test/**/*.js' ]; // , '--grep', '^Context-aware Should not crash when module is used in multiple contexts$'];
  require(mochaPath);
`, { eval: true });
worker1.on('error', (err) => {
  console.error('worker1 err: ', err);
});
worker1.on('exit', (code) => {
  console.log('worker1 exit code: ', code);
  const worker2 = new worker_threads.Worker(`
    process.umask = () => 0;
    const path = require('path');
    const mochaPath = path.join(process.env.USERPROFILE, '.vscode/extensions/maty.vscode-mocha-sidebar-0.20.24/node_modules/mocha/bin/_mocha');
    process.argv = [ process.argv0, mochaPath, 'test/**/*.js' ]; // , '--grep', '^Context-aware Should not crash when module is used in multiple contexts$'];
    require(mochaPath);
  `, { eval: true });
  worker2.on('error', (err) => {
    console.error('worker2 err: ', err);
  });
  worker2.on('exit', (code) => {
    console.log('worker2 exit code: ', code);
  });
});



// const path = require('path');
// const mochaPath = path.join(process.env.USERPROFILE, '.vscode/extensions/maty.vscode-mocha-sidebar-0.20.24/node_modules/mocha/bin/_mocha');
// process.argv = [ process.argv0, mochaPath, 'test/**/*.js' ]; // , '--grep', '^Context-aware Should not crash when module is used in multiple contexts$'];
// require(mochaPath);

// var assert = require('assert');
// var native = require("./buildjs/native");

// native.Key.generateRsa(1024, native.RsaPublicExponent.RSA_3, function (err, key) {
//   assert(key != null, true, "Error on key generation");
//   delete require.cache[require.resolve('./buildjs/native')];
//   delete require.cache[require.resolve('./build/Release/nodessl.node')];
//   const native2 = require("./buildjs/native");
//   native2.Key.generateRsa(1024, native.RsaPublicExponent.RSA_3, function (err, key2) {
//     assert(key2 != null, true, "Error on key generation");
//     done();
//   })
// });


// const worker1 = new worker_threads.Worker(`
//   var native = require("C:/2del/node-webcrypto-ossl-master/buildjs/native");
//   native.Key.generateRsa(1024, native.RsaPublicExponent.RSA_3, function (err, key) {
//     console.log('worker1 key: ', key);
//     process.exit(key != null ? 0 : 1);
//   });
// `, { eval: true });
// worker1.on('error', (err) => {
//   console.error('worker1 err: ', err);
//   done(err);
// });
// worker1.on('exit', (code) => {
//   console.log('worker1 exit code: ', code);
//   const worker2 = new worker_threads.Worker(`
//     var native = require("C:/2del/node-webcrypto-ossl-master/buildjs/native");
//     native.Key.generateRsa(1024, native.RsaPublicExponent.RSA_3, function (err, key) {
//       console.log('worker2 key: ', key);
//       process.exit(key != null ? 0 : 1);
//     });
//   `, { eval: true });
//   worker2.on('error', (err) => {
//     console.error('worker2 err: ', err);
//     done(err);
//   });
//   worker2.on('exit', (code) => {
//     console.log('worker2 exit code: ', code);
//     done();
//   });
// });
