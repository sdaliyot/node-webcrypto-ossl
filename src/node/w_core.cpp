#include "w_core.h"

const char* WCore::ClassName = "Core";

// NAN_MODULE_INIT(WCore::Init) {
void WCore::Init(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE target, v8::Local<v8::Context> context, v8::Isolate* isolate, v8::Local<v8::Value> addon_data_value) {
  AddonData* addon_data = static_cast<AddonData*>(addon_data_value.As<v8::External>()->Value());

	// v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, New, addon_data_value);
	// tpl->SetClassName(Nan::New(ClassName).ToLocalChecked());
	tpl->SetClassName(v8::String::NewFromUtf8(isolate, WCore::ClassName, v8::NewStringType::kNormal).ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// methods

	// constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
	constructor(addon_data).Reset(tpl->GetFunction(context).ToLocalChecked());

	// static methods
	Nan::SetMethod(Nan::GetFunction(tpl).ToLocalChecked(), "digest", Digest);

	// Nan::Set(target,
	// 					Nan::New(ClassName).ToLocalChecked(),
	// 					Nan::GetFunction(tpl).ToLocalChecked());
	target->Set(context,
		v8::String::NewFromUtf8(isolate, WCore::ClassName, v8::NewStringType::kNormal)
				.ToLocalChecked(),
		tpl->GetFunction(context).ToLocalChecked()).FromJust();
}

// NAN_METHOD(WCore::New) {
void WCore::New(const v8::FunctionCallbackInfo<v8::Value>& info) {
	LOG_FUNC();

	if (info.IsConstructCall()) {

		WCore* obj = new WCore();
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
* digestName: string
* message: Buffer
* cb: function(err: Error, digest: Buffer)
*/
NAN_METHOD(WCore::Digest) {
	LOG_FUNC();

	LOG_INFO("digestName");
    v8::Local<v8::String> v8DigestName = Nan::To<v8::String>(info[0]).ToLocalChecked();
    Nan::Utf8String nanDigestName(v8DigestName);
	Handle<std::string> hDigestName(new std::string(*nanDigestName));

	LOG_INFO("message");
	Handle<std::string> hMessage = v8Buffer_to_String(info[1]);

	Nan::Callback *callback = new Nan::Callback(info[2].As<v8::Function>());
	Nan::AsyncQueueWorker(new AsyncDigest(callback, hDigestName, hMessage));
}
