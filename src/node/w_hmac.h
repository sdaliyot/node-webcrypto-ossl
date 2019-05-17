#ifndef OSSL_W_HMAC_H_INCLUDE
#define OSSL_W_HMAC_H_INCLUDE

#include "../core/common.h"
#include "../hmac/common.h"
#include "async_hmac.h"

class WHmac: public node::ObjectWrap {
public:
	static v8::Local<v8::Object> NewInstance(AddonData* addon_data) {
		v8::Local<v8::Function> cons = Nan::New(constructor(addon_data));
		return Nan::NewInstance(cons).ToLocalChecked();
	}
	static v8::Local<v8::Object> NewInstance(int argc, v8::Local<v8::Value> argv[], AddonData* addon_data) {
		v8::Local<v8::Function> cons = Nan::New(constructor(addon_data));
		return Nan::NewInstance(cons, argc, argv).ToLocalChecked();
	}

	static const char* ClassName;

	// static void Init(v8::Local<v8::Object> exports);
	static void Init(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE target, v8::Local<v8::Context> context, v8::Isolate* isolate, v8::Local<v8::Value> addon_data_value);
	
	// static NAN_METHOD(New);
	static void New(const v8::FunctionCallbackInfo<v8::Value>& info);
	
	// static NAN_METHOD(Generate);
	static void Generate(const v8::FunctionCallbackInfo<v8::Value>& info);
	static NAN_METHOD(Export);
	// static NAN_METHOD(Import);
	static void Import(const v8::FunctionCallbackInfo<v8::Value>& info);
	static NAN_METHOD(Sign);
	static NAN_METHOD(Verify);

	Handle<ScopedHMAC> data;

protected:
	// static inline Nan::Persistent<v8::Function> & constructor() {
	static inline Nan::Persistent<v8::Function> & constructor(AddonData* addon_data) {
		// static Nan::Persistent<v8::Function> my_constructor;
		// return my_constructor;
		return addon_data->HmacCTOR;
	}

};

#include "common.h"


#endif // OSSL_W_HMAC_H_INCLUDE
