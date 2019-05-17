#ifndef OSSL_ADDON_DATA_H_INCLUDE
#define OSSL_ADDON_DATA_H_INCLUDE

// Class containing information needed for as long as the addon exists. It
// replaces the use of global static data with per-addon-instance data by
// associating an instance of this class with each instance of this addon during
// addon initialization. The instance of this class is then passed to each
// binding the addon provides. Thus, the data stored in an instance of this
// class is available to each binding, just as global static data would be.
class AddonData {
 public:
  // This is boilerplate. It creates a new instance of this class and wraps it
  // into a v8::External. The isolate and the exports are necessary, because we
  // want the instance of this class to be destroyed along with the exports
  // object when the addon is eventually unloaded.
  static v8::Local<v8::Value> New(v8::Isolate* isolate, v8::Local<v8::Object> exports) {
    return v8::External::New(isolate, new AddonData(isolate, exports));
  }

  Nan::Persistent<v8::Function> KeyCTOR;
  Nan::Persistent<v8::Function> AesCTOR;
  Nan::Persistent<v8::Function> HmacCTOR;
  Nan::Persistent<v8::Function> WPbkdf2CTOR;
  Nan::Persistent<v8::Function> CoreCTOR;

 private:
  explicit AddonData(v8::Isolate* isolate, v8::Local<v8::Object> exports) {
    // The payload is initialized here. The rest of the constructor is
    // boilerplate that ensures that the instance of this addon data is
    // destroyed along with the instance of this addon (`exports`).
    exports_persistent.Reset(isolate, exports);
    exports_persistent.SetWeak(this, DeleteMe, v8::WeakCallbackType::kParameter);
  }

  // The rest of the class definition is boilerplate.

  // The persistent reference must be reset before the instance of this class is
  // destroyed, otherwise memory will leak on the V8 side.
  ~AddonData() {
    exports_persistent.Reset();
  }

  // This static function will be called when the addon instance is unloaded. It
  // merely destroys the per-addon-instance data.
  static void DeleteMe(const v8::WeakCallbackInfo<AddonData>& info) {
    delete info.GetParameter();
  }

  v8::Persistent<v8::Object> exports_persistent;
};

#endif // OSSL_ADDON_DATA_H_INCLUDE
