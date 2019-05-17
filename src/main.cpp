#include "node/common.h"

#define SET_ENUM(obj, name, value)												    \
    Nan::Set(obj, Nan::New(#name).ToLocalChecked(), Nan::New<v8::Number>(value));	\
    Nan::Set(obj, value, Nan::New(#name).ToLocalChecked());

void EcNamedCurves(v8::Local<v8::Object> target) {
	v8::Local<v8::Object> ecNamedCurves = Nan::New<v8::Object>();
#define SET_NAMED_CURVE(namedCurve)												\
	SET_ENUM(ecNamedCurves, namedCurve, NID_##namedCurve);

	SET_NAMED_CURVE(secp112r1);
	SET_NAMED_CURVE(secp112r2);
	SET_NAMED_CURVE(secp128r1);
	SET_NAMED_CURVE(secp128r2);
	SET_NAMED_CURVE(secp160k1);
	SET_NAMED_CURVE(secp160r1);
	SET_NAMED_CURVE(secp160r2);
	SET_ENUM(ecNamedCurves, secp192r1, 409);
	SET_NAMED_CURVE(secp192k1);
	SET_NAMED_CURVE(secp224k1);
	SET_NAMED_CURVE(secp224r1);
	SET_NAMED_CURVE(secp256k1);
	SET_ENUM(ecNamedCurves, secp256r1, 415);
	SET_NAMED_CURVE(secp384r1);
	SET_NAMED_CURVE(secp521r1);
	SET_NAMED_CURVE(sect113r1);
	SET_NAMED_CURVE(sect113r2);
	SET_NAMED_CURVE(sect131r1);
	SET_NAMED_CURVE(sect131r2);
	SET_NAMED_CURVE(sect163k1);
	SET_NAMED_CURVE(sect163r1);
	SET_NAMED_CURVE(sect163r2);
	SET_NAMED_CURVE(sect193r1);
	SET_NAMED_CURVE(sect193r2);
	SET_NAMED_CURVE(sect233k1);
	SET_NAMED_CURVE(sect233r1);
	SET_NAMED_CURVE(sect239k1);
	SET_NAMED_CURVE(sect283k1);
	SET_NAMED_CURVE(sect283r1);
	SET_NAMED_CURVE(sect409k1);
	SET_NAMED_CURVE(sect409r1);
	SET_NAMED_CURVE(sect571k1);
	SET_NAMED_CURVE(sect571r1);

    Nan::Set(target, Nan::New("EcNamedCurves").ToLocalChecked(), ecNamedCurves);

#undef SET_NAMED_CURVE
}

void RsaPublicExponent(v8::Local<v8::Object> target) {
	v8::Local<v8::Object> rsaPublicExponent = Nan::New<v8::Object>();

	SET_ENUM(rsaPublicExponent, RSA_3, 0);
	SET_ENUM(rsaPublicExponent, RSA_F4, 1);

    Nan::Set(target, Nan::New("RsaPublicExponent").ToLocalChecked(), rsaPublicExponent);
}

void KeyType(v8::Local<v8::Object> target) {
	v8::Local<v8::Object> keyType = Nan::New<v8::Object>();

	SET_ENUM(keyType, PUBLIC, 0);
	SET_ENUM(keyType, PRIVATE, 1);

    Nan::Set(target, Nan::New("KeyType").ToLocalChecked(), keyType);
}

// Initialize the addon in such a way that it may be initialized multiple times
// per process. The function body following this macro is provided the value
// `exports` of type `Local<Object>`, the value `module` of type
// `Local<Object>`, and `context` of type `Local<Context>`. It may either define
// new properties on the `exports` object, or define the property named
// "exports" on the `module` object.
NODE_MODULE_INIT(/*exports, module, context*/) {
	v8::Isolate* isolate = context->GetIsolate();

	// Create a new instance of the addon data that will be associated with this
	// instance of the addon, and that will be freed along with this instance of
	// the addon.
	v8::Local<v8::Value> addon_data = AddonData::New(isolate, exports);

	Nan::HandleScope scope;

	OPENSSL_init();
	OpenSSL_add_all_algorithms();
	ERR_load_crypto_strings();

	WKey::Init(exports, context, isolate, addon_data);
	WAes::Init(exports, context, isolate, addon_data);
	WHmac::Init(exports, context, isolate, addon_data);
	WPbkdf2::Init(exports, context, isolate, addon_data);
	WCore::Init(exports, context, isolate, addon_data);

	// Enums
	EcNamedCurves(exports);
	RsaPublicExponent(exports);
	KeyType(exports);

}
