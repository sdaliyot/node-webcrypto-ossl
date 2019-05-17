"use strict";
var assert = require('assert');
var native = require("../buildjs/native");

var worker_threads = require('worker_threads');

describe("Context-aware", function () {


  // it("Should not throw an error when module is loaded multiple times", function (done) {
  //   native.Key.generateRsa(1024, native.RsaPublicExponent.RSA_3, function (err, key) {
  //     assert(key != null, true, "Error on key generation");
  //     delete require.cache[require.resolve('../buildjs/native')];
  //     delete require.cache[require.resolve('../build/Release/nodessl.node')];
  //     const native2 = require("../buildjs/native");
  //     native2.Key.generateRsa(1024, native.RsaPublicExponent.RSA_3, function (err, key2) {
  //       assert(key2 != null, true, "Error on key generation");
  //       done();
  //     })
  //   });
  // });

  // it("Should not crash when module is used in multiple contexts", function (done) {
  //   const worker1 = new worker_threads.Worker(`
  //     var native = require("C:/2del/node-webcrypto-ossl-master/buildjs/native");
  //     native.Key.generateRsa(1024, native.RsaPublicExponent.RSA_3, function (err, key) {
  //       console.log('worker1 key: ', key);
  //       process.exit(key != null ? 0 : 1);
  //     });
  //   `, { eval: true });
  //   worker1.on('error', (err) => {
  //     console.error('worker1 err: ', err);
  //     done(err);
  //   });
  //   worker1.on('exit', (code) => {
  //     console.log('worker1 exit code: ', code);
  //     const worker2 = new worker_threads.Worker(`
  //       var native = require("C:/2del/node-webcrypto-ossl-master/buildjs/native");
  //       native.Key.generateRsa(1024, native.RsaPublicExponent.RSA_3, function (err, key) {
  //         console.log('worker2 key: ', key);
  //         process.exit(key != null ? 0 : 1);
  //       });
  //     `, { eval: true });
  //     worker2.on('error', (err) => {
  //       console.error('worker2 err: ', err);
  //       done(err);
  //     });
  //     worker2.on('exit', (code) => {
  //       console.log('worker2 exit code: ', code);
  //       done();
  //     });
  //   });
  // });

});