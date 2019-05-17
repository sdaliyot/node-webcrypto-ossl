#include "w_hmac.h"

const char* WHmac::ClassName = "HmacKey";

// NAN_MODULE_INIT(WHmac::Init) {
void WHmac::Init(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE target, v8::Local<v8::Context> context, v8::Isolate* isolate, v8::Local<v8::Value> addon_data_value) {
  AddonData* addon_data = static_cast<AddonData*>(addon_data_value.As<v8::External>()->Value());

	// v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, New, addon_data_value);
	// tpl->SetClassName(Nan::New(ClassName).ToLocalChecked());
	tpl->SetClassName(v8::String::NewFromUtf8(isolate, WHmac::ClassName, v8::NewStringType::kNormal).ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// methods

	SetPrototypeMethod(tpl, "export", Export);
	SetPrototypeMethod(tpl, "sign", Sign);
	SetPrototypeMethod(tpl, "verify", Verify);

	// constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
	constructor(addon_data).Reset(tpl->GetFunction(context).ToLocalChecked());

	// static methods
	// Nan::SetMethod(Nan::GetFunction(tpl).ToLocalChecked(), "generate", Generate);
	v8::Local<v8::FunctionTemplate> generateFT = v8::FunctionTemplate::New(isolate, Generate, addon_data_value);
	v8::Local<v8::String> generateName = v8::String::NewFromUtf8(isolate, "generate", v8::NewStringType::kNormal).ToLocalChecked();
	generateFT->SetClassName(generateName);
	v8::Local<v8::Function> generateF = generateFT->GetFunction(context).ToLocalChecked();
	tpl->GetFunction(context).ToLocalChecked()->Set(context, generateName, generateF);

	// Nan::SetMethod(Nan::GetFunction(tpl).ToLocalChecked(), "import", Import);
	v8::Local<v8::FunctionTemplate> importFT = v8::FunctionTemplate::New(isolate, Import, addon_data_value);
	v8::Local<v8::String> importName = v8::String::NewFromUtf8(isolate, "import", v8::NewStringType::kNormal).ToLocalChecked();
	importFT->SetClassName(importName);
	v8::Local<v8::Function> importF = importFT->GetFunction(context).ToLocalChecked();
	tpl->GetFunction(context).ToLocalChecked()->Set(context, importName, importF);

	// Nan::Set(target,
	// 					Nan::New(ClassName).ToLocalChecked(),
	// 					Nan::GetFunction(tpl).ToLocalChecked());
	target->Set(context,
		v8::String::NewFromUtf8(isolate, WHmac::ClassName, v8::NewStringType::kNormal)
				.ToLocalChecked(),
		tpl->GetFunction(context).ToLocalChecked()).FromJust();
}

// NAN_METHOD(WHmac::New) {
void WHmac::New(const v8::FunctionCallbackInfo<v8::Value>& info) {
	LOG_FUNC();

	if (info.IsConstructCall()) {

		WHmac * obj = new WHmac();
		obj->Wrap(info.This());
		info.GetReturnValue().Set(info.This());
	}
	else {
		//const int argc = 1;
		//v8::Local<v8::Value> argv[argc] = { info[0] };

		// v8::Local<v8::Function> cons = Nan::New(constructor());
		AddonData* addon_data = static_cast<AddonData*>(info.Data().As<v8::External>()->Value());
		v8::Local<v8::Function> cons = Nan::New(constructor(addon_data));

		info.GetReturnValue().Set(Nan::NewInstance(cons, 0, nullptr).ToLocalChecked());
	}
};

/*
* keySize: number
* cb: function
*/
// NAN_METHOD(WHmac::Generate) {
void WHmac::Generate(const v8::FunctionCallbackInfo<v8::Value>& info) {
	LOG_FUNC();

	int keySize = Nan::To<int>(info[0]).FromJust();
	Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());

	Nan::AsyncQueueWorker(new AsyncHmacGenerateKey(callback, keySize, info.Data()));
}

/*
* cb: function
*/
NAN_METHOD(WHmac::Export) {
	LOG_FUNC();

	LOG_INFO("this");
	WHmac *that = WHmac::Unwrap<WHmac>(info.This());

	Nan::Callback *callback = new Nan::Callback(info[0].As<v8::Function>());
	Nan::AsyncQueueWorker(new AsyncHmacExport(callback, that->data));
}

/*
* raw: buffer
* cb: function
*/
// NAN_METHOD(WHmac::Import) {
void WHmac::Import(const v8::FunctionCallbackInfo<v8::Value>& info) {
	LOG_FUNC();

	LOG_INFO("raw");
	Handle<std::string> hRaw = v8Buffer_to_String(info[0]);

	Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());
	Nan::AsyncQueueWorker(new AsyncHmacImport(callback, hRaw, info.Data()));
}

/*
* digestName: string
* data: Buffer
*/
NAN_METHOD(WHmac::Sign) {
	LOG_FUNC();

	LOG_INFO("digestName");
    Nan::Utf8String v8DigestName(Nan::To<v8::String>(info[0]).ToLocalChecked());
	const EVP_MD *md = EVP_get_digestbyname(*v8DigestName);
	if (!md) {
		Nan::ThrowError("Unknown digest name");
		return;
	}

	LOG_INFO("data");
	Handle<std::string> hBio = v8Buffer_to_String(info[1]);

	LOG_INFO("this->Key");
	WHmac *that = WHmac::Unwrap<WHmac>(info.This());

	LOG_INFO("Check key HMAC");
	switch (that->data->type) {
	case EVP_PKEY_HMAC:
		break;
	default:
		v8::Local<v8::Value> argv[] = {
			Nan::New("Unsupported Key in use").ToLocalChecked()
		};

		Nan::CallAsFunction(Nan::To<v8::Object>(info[2]).ToLocalChecked(), info.This(), 1, argv);
		return;
	}

	Nan::Callback *callback = new Nan::Callback(info[2].As<v8::Function>());
	Nan::AsyncQueueWorker(new AsyncHmacSign(callback, md, that->data, hBio));
}

/*
* digestName: string
* data: Buffer
* signature: Buffer
* cb: function
*/
NAN_METHOD(WHmac::Verify) {
	LOG_FUNC();

	LOG_INFO("digestName");
    Nan::Utf8String v8DigestName(Nan::To<v8::String>(info[0]).ToLocalChecked());
	const EVP_MD *md = EVP_get_digestbyname(*v8DigestName);
	if (!md) {
		Nan::ThrowError("Unknown digest name");
		return;
	}

	LOG_INFO("data");
	Handle<std::string> data = v8Buffer_to_String(info[1]);

	LOG_INFO("signature");
	Handle<std::string> sig = v8Buffer_to_String(info[2]);

	LOG_INFO("this->Key");
	WHmac *that = WHmac::Unwrap<WHmac>(info.This());
	Handle<ScopedHMAC> pkey = that->data;

	LOG_INFO("Check key HMAC");
	switch (pkey->type) {
	case EVP_PKEY_HMAC:
		break;
	default:
		v8::Local<v8::Value> argv[] = {
			Nan::New("Unsupported Key in use").ToLocalChecked()
		};

		Nan::CallAsFunction(Nan::To<v8::Object>(info[2]).ToLocalChecked(), info.This(), 1, argv);
		return;
	}

	Nan::Callback *callback = new Nan::Callback(info[3].As<v8::Function>());
	Nan::AsyncQueueWorker(new AsyncHmacVerify(callback, md, pkey, data, sig));

}
