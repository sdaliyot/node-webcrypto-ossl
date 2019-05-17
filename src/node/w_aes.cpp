#include "w_aes.h"

const char* WAes::ClassName = "AesKey";

// NAN_MODULE_INIT(WAes::Init) {
void WAes::Init(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE target, v8::Local<v8::Context> context, v8::Isolate* isolate, v8::Local<v8::Value> addon_data_value) {
  AddonData* addon_data = static_cast<AddonData*>(addon_data_value.As<v8::External>()->Value());

	// v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, New, addon_data_value);
	// tpl->SetClassName(Nan::New(ClassName).ToLocalChecked());
	tpl->SetClassName(v8::String::NewFromUtf8(isolate, WAes::ClassName, v8::NewStringType::kNormal).ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// methods
	Nan::SetPrototypeMethod(tpl, "encrypt", Encrypt);
	Nan::SetPrototypeMethod(tpl, "decrypt", Decrypt);
	Nan::SetPrototypeMethod(tpl, "wrapKey", WrapKey);
	Nan::SetPrototypeMethod(tpl, "unwrapKey", UnwrapKey);
	Nan::SetPrototypeMethod(tpl, "encryptEcb", EncryptEcb);
	Nan::SetPrototypeMethod(tpl, "decryptEcb", DecryptEcb);
	Nan::SetPrototypeMethod(tpl, "encryptGcm", EncryptGcm);
	Nan::SetPrototypeMethod(tpl, "decryptGcm", DecryptGcm);
	Nan::SetPrototypeMethod(tpl, "encryptCtr", EncryptCtr);
	Nan::SetPrototypeMethod(tpl, "decryptCtr", DecryptCtr);
	Nan::SetPrototypeMethod(tpl, "export", Export);

	constructor(addon_data).Reset(Nan::GetFunction(tpl).ToLocalChecked());

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
		v8::String::NewFromUtf8(isolate, WAes::ClassName, v8::NewStringType::kNormal)
				.ToLocalChecked(),
		tpl->GetFunction(context).ToLocalChecked()).FromJust();
}

// NAN_METHOD(WAes::New) {
void WAes::New(const v8::FunctionCallbackInfo<v8::Value>& info) {
	LOG_FUNC();

	if (info.IsConstructCall()) {

		WAes * obj = new WAes();
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

/**
 * keySize: number
 * cb: (e: Error, key: AesKey)
 */
// NAN_METHOD(WAes::Generate) {
void WAes::Generate(const v8::FunctionCallbackInfo<v8::Value>& info) {
	LOG_FUNC();

	int keySize = Nan::To<int>(info[0]).FromJust();
	Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());

	Nan::AsyncQueueWorker(new AsyncAesGenerateKey(callback, keySize, info.Data()));
}

/**
 * cipher: string
 * iv: Buffer,
 * input: Buffer
 */
NAN_METHOD(WAes::Encrypt) {
	LOG_FUNC();

	LOG_INFO("cipher");
    Nan::Utf8String v8Cipher(Nan::To<v8::String>(info[0]).ToLocalChecked());

	LOG_INFO("iv");
	Handle<std::string> hIv = v8Buffer_to_String(info[1]);

	LOG_INFO("iv");
	Handle<std::string> hInput = v8Buffer_to_String(info[2]);

	LOG_INFO("this");
	WAes *wAes = WAes::Unwrap<WAes>(info.This());

	Nan::Callback *callback = new Nan::Callback(info[3].As<v8::Function>());

	if (strcmp("CBC", *v8Cipher) == 0) {
		Nan::AsyncQueueWorker(new AsyncAesEncryptCBC(callback, wAes->data, hInput, hIv, true));
	}
	else {
		v8::Local<v8::Value> argv[] = {
			Nan::New("Unknown cipher name").ToLocalChecked()
		};
        Nan::Call(Nan::To<v8::Function>(info[3]).ToLocalChecked(), Nan::GetCurrentContext()->Global(), 1, argv);
	}
}

/**
 * cipher: string
 * iv: Buffer,
 * input: Buffer
 */
NAN_METHOD(WAes::Decrypt) {
	LOG_FUNC();

	LOG_INFO("cipher");
    Nan::Utf8String v8Cipher(Nan::To<v8::String>(info[0]).ToLocalChecked());

	LOG_INFO("iv");
	Handle<std::string> hIv = v8Buffer_to_String(info[1]);

	LOG_INFO("iv");
	Handle<std::string> hInput = v8Buffer_to_String(info[2]);

	LOG_INFO("this");
	WAes *wAes = WAes::Unwrap<WAes>(info.This());

	Nan::Callback *callback = new Nan::Callback(info[3].As<v8::Function>());

	if (strcmp("CBC", *v8Cipher) == 0) {
		Nan::AsyncQueueWorker(new AsyncAesEncryptCBC(callback, wAes->data, hInput, hIv, false));
	}
	else {
		v8::Local<v8::Value> argv[] = {
			Nan::New("Unknown cipher name").ToLocalChecked()
		};
		Nan::Call(Nan::To<v8::Function>(info[3]).ToLocalChecked(), Nan::GetCurrentContext()->Global(), 1, argv);
	}
}

/**
* cipher: string
* input: Buffer
*/
NAN_METHOD(WAes::EncryptEcb) {
	LOG_FUNC();

	LOG_INFO("input");
	Handle<std::string> hInput = v8Buffer_to_String(info[0]);

	LOG_INFO("this");
	WAes *wAes = WAes::Unwrap<WAes>(info.This());

	Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());

	Nan::AsyncQueueWorker(new AsyncAesEncryptECB(callback, wAes->data, hInput, true));
}

/**
* input: Buffer
*/
NAN_METHOD(WAes::DecryptEcb) {
	LOG_FUNC();

	LOG_INFO("input");
	Handle<std::string> hInput = v8Buffer_to_String(info[0]);

	LOG_INFO("this");
	WAes *wAes = WAes::Unwrap<WAes>(info.This());

	Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());

	Nan::AsyncQueueWorker(new AsyncAesEncryptECB(callback, wAes->data, hInput, false));
}

/**
 * cb: function
 */
NAN_METHOD(WAes::Export) {
	LOG_FUNC();

	LOG_INFO("this");
	WAes *wAes = WAes::Unwrap<WAes>(info.This());

	Nan::Callback *callback = new Nan::Callback(info[0].As<v8::Function>());
	Nan::AsyncQueueWorker(new AsyncAesExport(callback, wAes->data));
}

/**
 * raw: buffer
 * cb: function
 */
// NAN_METHOD(WAes::Import) {
void WAes::Import(const v8::FunctionCallbackInfo<v8::Value>& info) {
	LOG_FUNC();

	LOG_INFO("raw");
	Handle<std::string> hRaw = v8Buffer_to_String(info[0]);

	Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());
	Nan::AsyncQueueWorker(new AsyncAesImport(callback, hRaw, info.Data()));
}

/**
 * iv: Buffer,
 * input: Buffer
 * aad: Buffer
 * tagSize: int
 */
NAN_METHOD(WAes::EncryptGcm) {
	LOG_FUNC();

	LOG_INFO("iv");
	Handle<std::string> hIv = v8Buffer_to_String(info[0]);

	LOG_INFO("input");
	Handle<std::string> hInput = v8Buffer_to_String(info[1]);

	LOG_INFO("aad");
	Handle<std::string> hAad = v8Buffer_to_String(info[2]);

	LOG_INFO("tag");
	int tag = Nan::To<int>(info[3]).FromJust();

	LOG_INFO("this");
	WAes *wAes = WAes::Unwrap<WAes>(info.This());

	Nan::Callback *callback = new Nan::Callback(info[4].As<v8::Function>());
	Nan::AsyncQueueWorker(new AsyncAesEncryptGCM(callback, wAes->data, hInput, hIv, hAad, tag, true));
}

/**
 * iv: Buffer,
 * input: Buffer
 * aad: Buffer
 * tagSize: int
 */
NAN_METHOD(WAes::DecryptGcm) {
	LOG_FUNC();

	LOG_INFO("iv");
	Handle<std::string> hIv = v8Buffer_to_String(info[0]);

	LOG_INFO("input");
	Handle<std::string> hInput = v8Buffer_to_String(info[1]);

	LOG_INFO("aad");
	Handle<std::string> hAad = v8Buffer_to_String(info[2]);

	LOG_INFO("tag");
	int tag = Nan::To<int>(info[3]).FromJust();

	LOG_INFO("this");
	WAes *wAes = WAes::Unwrap<WAes>(info.This());

	Nan::Callback *callback = new Nan::Callback(info[4].As<v8::Function>());
	Nan::AsyncQueueWorker(new AsyncAesEncryptGCM(callback, wAes->data, hInput, hIv, hAad, tag, false));
}

/**
 * input: Buffer
 * cb: (e: Error, data: Buffer)
 */
NAN_METHOD(WAes::WrapKey) {
	LOG_FUNC();

	LOG_INFO("input");
	Handle<std::string> hInput = v8Buffer_to_String(info[0]);

	LOG_INFO("this");
	WAes *wAes = WAes::Unwrap<WAes>(info.This());

	Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());
	Nan::AsyncQueueWorker(new AsyncAesWrapKey(callback, wAes->data, hInput, true));
}

/**
 * input: Buffer
 * cb: (e: Error, raw: Buffer)
 */
NAN_METHOD(WAes::UnwrapKey) {
	LOG_FUNC();

	LOG_INFO("input");
	Handle<std::string> hInput = v8Buffer_to_String(info[0]);

	LOG_INFO("this");
	WAes *wAes = WAes::Unwrap<WAes>(info.This());

	Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());
	Nan::AsyncQueueWorker(new AsyncAesWrapKey(callback, wAes->data, hInput, false));
}

/**
 * input: Buffer
 * counter: Buffer,
 * length: int
 */
NAN_METHOD(WAes::EncryptCtr) {
    LOG_FUNC();
    
    LOG_INFO("input");
    Handle<std::string> hInput = v8Buffer_to_String(info[0]);
    
    LOG_INFO("counter");
    Handle<std::string> hCounter = v8Buffer_to_String(info[1]);
    if (hCounter->length() != 16) {
        THROW_ERROR("Wrong size of 'counter'. Must be 16 bytes.");
    }
    
    LOG_INFO("length");
    int length = Nan::To<int>(info[2]).FromJust();
    
    LOG_INFO("this");
    WAes *wAes = WAes::Unwrap<WAes>(info.This());
    
    Nan::Callback *callback = new Nan::Callback(info[3].As<v8::Function>());
    Nan::AsyncQueueWorker(new AsyncAesEncryptCTR(callback, wAes->data, hInput, hCounter, length, true));
}

/**
 * input: Buffer
 * counter: Buffer,
 * length: int
 */
NAN_METHOD(WAes::DecryptCtr) {
    LOG_FUNC();
    
    LOG_INFO("input");
    Handle<std::string> hInput = v8Buffer_to_String(info[0]);
    
    LOG_INFO("counter");
    Handle<std::string> hCounter = v8Buffer_to_String(info[1]);
    if (hCounter->length() != 16) {
        THROW_ERROR("Wrong size of 'counter'. Must be 16 bytes.");
    }
    
    LOG_INFO("length");
    int length = Nan::To<int>(info[2]).FromJust();
    
    LOG_INFO("this");
    WAes *wAes = WAes::Unwrap<WAes>(info.This());
    
    Nan::Callback *callback = new Nan::Callback(info[3].As<v8::Function>());
    Nan::AsyncQueueWorker(new AsyncAesEncryptCTR(callback, wAes->data, hInput, hCounter, length, false));
}
