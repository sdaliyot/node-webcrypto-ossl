#ifndef OSSL_W_KEY_H_INCLUDE
#define OSSL_W_KEY_H_INCLUDE

#include "../core/common.h"
#include "../rsa/common.h"
#include "../ec/common.h"

#define v8Object_get_BN(v8Obj, v8Param, RsaKey, RsaKeyParam) \
{unsigned char* v8Param = (unsigned char*)node::Buffer::Data(Nan::To<v8::Object>(Nan::Get(v8Obj, Nan::New(#v8Param).ToLocalChecked()).ToLocalChecked()).ToLocalChecked()); \
    RsaKey->RsaKeyParam = BN_bin2bn(v8Param, (int)node::Buffer::Length(Nan::To<v8::Object>(Nan::Get(v8Obj, Nan::New(#v8Param).ToLocalChecked()).ToLocalChecked()).ToLocalChecked()), nullptr);}

class WKey : public node::ObjectWrap {
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

	// static NAN_MODULE_INIT(Init);
	static void Init(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE target, v8::Local<v8::Context> context, v8::Isolate* isolate, v8::Local<v8::Value> addon_data_value);
	
	// static NAN_METHOD(New);
	static void New(const v8::FunctionCallbackInfo<v8::Value>& info);
	
	static NAN_GETTER(Type);
	static NAN_METHOD(ModulusLength);
	static NAN_METHOD(PublicExponent);
	// static NAN_METHOD(GenerateRsa);
	static void GenerateRsa(const v8::FunctionCallbackInfo<v8::Value>& info);
	// static NAN_METHOD(GenerateEc);
	static void GenerateEc(const v8::FunctionCallbackInfo<v8::Value>& info);
	static NAN_METHOD(ExportJwk);
	static NAN_METHOD(ExportSpki);
	static NAN_METHOD(ExportPkcs8);
	// static NAN_METHOD(ImportJwk);
	static void ImportJwk(const v8::FunctionCallbackInfo<v8::Value>& info);
	// static NAN_METHOD(ImportSpki);
	static void ImportSpki(const v8::FunctionCallbackInfo<v8::Value>& info);
	// static NAN_METHOD(ImportPkcs8);
	static void ImportPkcs8(const v8::FunctionCallbackInfo<v8::Value>& info);
	static NAN_METHOD(RsaOaepEncDec);
	static NAN_METHOD(RsaPssSign);
	static NAN_METHOD(RsaPssVerify);
	static NAN_METHOD(EcdhDeriveKey);
	static NAN_METHOD(EcdhDeriveBits);

	static NAN_METHOD(Sign);
	static NAN_METHOD(Verify);

	Handle<ScopedEVP_PKEY> data;

protected:
	// static inline Nan::Persistent<v8::Function> & constructor() {
	static inline Nan::Persistent<v8::Function> & constructor(AddonData* addon_data) {
		// static Nan::Persistent<v8::Function> my_constructor;
		// return my_constructor;
		return addon_data->KeyCTOR;
	}

};

#include "common.h"


#endif // OSSL_W_KEY_H_INCLUDE
