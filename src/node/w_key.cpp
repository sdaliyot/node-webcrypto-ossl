#include "w_key.h"

const char* WKey::ClassName = "Key";

// NAN_MODULE_INIT(WKey::Init) {
void WKey::Init(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE target, v8::Local<v8::Context> context, v8::Isolate* isolate, v8::Local<v8::Value> addon_data_value) {
  AddonData* addon_data = static_cast<AddonData*>(addon_data_value.As<v8::External>()->Value());

	// v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, New, addon_data_value);
	// tpl->SetClassName(Nan::New(WKey::ClassName).ToLocalChecked());
	tpl->SetClassName(v8::String::NewFromUtf8(isolate, WKey::ClassName, v8::NewStringType::kNormal).ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// methods
	Nan::SetPrototypeMethod(tpl, "exportJwk", ExportJwk);
	Nan::SetPrototypeMethod(tpl, "exportSpki", ExportSpki);
	Nan::SetPrototypeMethod(tpl, "exportPkcs8", ExportPkcs8);
	Nan::SetPrototypeMethod(tpl, "sign", Sign);
	Nan::SetPrototypeMethod(tpl, "verify", Verify);
	Nan::SetPrototypeMethod(tpl, "RsaOaepEncDec", RsaOaepEncDec);
	Nan::SetPrototypeMethod(tpl, "RsaPssSign", RsaPssSign);
	Nan::SetPrototypeMethod(tpl, "RsaPssVerify", RsaPssVerify);
	Nan::SetPrototypeMethod(tpl, "EcdhDeriveKey", EcdhDeriveKey);
	Nan::SetPrototypeMethod(tpl, "EcdhDeriveBits", EcdhDeriveBits);
	Nan::SetPrototypeMethod(tpl, "modulusLength", ModulusLength);
	Nan::SetPrototypeMethod(tpl, "publicExponent", PublicExponent);

	v8::Local<v8::ObjectTemplate> itpl = tpl->InstanceTemplate();
	Nan::SetAccessor(itpl, Nan::New("type").ToLocalChecked(), Type);

	// constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
	constructor(addon_data).Reset(tpl->GetFunction(context).ToLocalChecked());

	// static methods

	// Nan::SetMethod(Nan::GetFunction(tpl).ToLocalChecked(), "generateRsa", GenerateRsa);
	v8::Local<v8::FunctionTemplate> generateRsaFT = v8::FunctionTemplate::New(isolate, GenerateRsa, addon_data_value);
	v8::Local<v8::String> generateRsaName = v8::String::NewFromUtf8(isolate, "generateRsa", v8::NewStringType::kNormal).ToLocalChecked();
	generateRsaFT->SetClassName(generateRsaName);
	v8::Local<v8::Function> generateRsaF = generateRsaFT->GetFunction(context).ToLocalChecked();
	tpl->GetFunction(context).ToLocalChecked()->Set(context, generateRsaName, generateRsaF);

	// Nan::SetMethod(Nan::GetFunction(tpl).ToLocalChecked(), "generateEc", GenerateEc);
	v8::Local<v8::FunctionTemplate> generateEcFT = v8::FunctionTemplate::New(isolate, GenerateEc, addon_data_value);
	v8::Local<v8::String> generateEcName = v8::String::NewFromUtf8(isolate, "generateEc", v8::NewStringType::kNormal).ToLocalChecked();
	generateEcFT->SetClassName(generateEcName);
	v8::Local<v8::Function> generateEcF = generateEcFT->GetFunction(context).ToLocalChecked();
	tpl->GetFunction(context).ToLocalChecked()->Set(context, generateEcName, generateEcF);

	// Nan::SetMethod(Nan::GetFunction(tpl).ToLocalChecked(), "importPkcs8", ImportPkcs8);
	v8::Local<v8::FunctionTemplate> importPkcs8FT = v8::FunctionTemplate::New(isolate, ImportPkcs8, addon_data_value);
	v8::Local<v8::String> importPkcs8Name = v8::String::NewFromUtf8(isolate, "importPkcs8", v8::NewStringType::kNormal).ToLocalChecked();
	importPkcs8FT->SetClassName(importPkcs8Name);
	v8::Local<v8::Function> importPkcs8F = importPkcs8FT->GetFunction(context).ToLocalChecked();
	tpl->GetFunction(context).ToLocalChecked()->Set(context, importPkcs8Name, importPkcs8F);

	// Nan::SetMethod(Nan::GetFunction(tpl).ToLocalChecked(), "importJwk", ImportJwk);
	v8::Local<v8::FunctionTemplate> importJwkFT = v8::FunctionTemplate::New(isolate, ImportJwk, addon_data_value);
	v8::Local<v8::String> importJwkName = v8::String::NewFromUtf8(isolate, "importJwk", v8::NewStringType::kNormal).ToLocalChecked();
	importJwkFT->SetClassName(importJwkName);
	v8::Local<v8::Function> importJwkF = importJwkFT->GetFunction(context).ToLocalChecked();
	tpl->GetFunction(context).ToLocalChecked()->Set(context, importJwkName, importJwkF);

	// Nan::SetMethod(Nan::GetFunction(tpl).ToLocalChecked(), "importSpki", ImportSpki);
	v8::Local<v8::FunctionTemplate> importSpkiFT = v8::FunctionTemplate::New(isolate, ImportSpki, addon_data_value);
	v8::Local<v8::String> importSpkiName = v8::String::NewFromUtf8(isolate, "importSpki", v8::NewStringType::kNormal).ToLocalChecked();
	importSpkiFT->SetClassName(importSpkiName);
	v8::Local<v8::Function> importSpkiF = importSpkiFT->GetFunction(context).ToLocalChecked();
	tpl->GetFunction(context).ToLocalChecked()->Set(context, importSpkiName, importSpkiF);

	// Nan::Set(target,
	//          Nan::New(WKey::ClassName).ToLocalChecked(),
	//          Nan::GetFunction(tpl).ToLocalChecked());
	target->Set(context,
		v8::String::NewFromUtf8(isolate, WKey::ClassName, v8::NewStringType::kNormal)
				.ToLocalChecked(),
		tpl->GetFunction(context).ToLocalChecked()).FromJust();
}

// NAN_METHOD(WKey::New) {
void WKey::New(const v8::FunctionCallbackInfo<v8::Value>& info) {
	LOG_FUNC();

	if (info.IsConstructCall()) {

		WKey * obj = new WKey();
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

NAN_GETTER(WKey::Type) {
	LOG_FUNC();

	WKey *wkey = WKey::Unwrap<WKey>(info.This());
    info.GetReturnValue().Set(Nan::New<v8::Number>(EVP_PKEY_base_id(wkey->data->Get())));
}

NAN_METHOD(WKey::ModulusLength) {
	LOG_FUNC();

	WKey *wkey = WKey::Unwrap<WKey>(info.This());

    if (EVP_PKEY_base_id(wkey->data->Get()) != EVP_PKEY_RSA)
		Nan::ThrowError("Key is not RSA");
	else {
		ScopedRSA rsa(EVP_PKEY_get1_RSA(wkey->data->Get()));

		int modulus_length = RSA_size(rsa.Get());
		info.GetReturnValue().Set(Nan::New<v8::Number>(modulus_length));
	}
}

NAN_METHOD(WKey::PublicExponent) {
	LOG_FUNC();

	WKey *wkey = WKey::Unwrap<WKey>(info.This());

    if (EVP_PKEY_base_id(wkey->data->Get()) != EVP_PKEY_RSA)
		Nan::ThrowError("Key is not RSA");
	else {
        ScopedRSA rsa(EVP_PKEY_get1_RSA(wkey->data->Get()));

#if NODE_MODULE_VERSION < 60 || OPENSSL_1_0_2
        
        BIGNUM *e = rsa.Get()->e;
        
#else
        
        // NODE v10
        
        const BIGNUM *e;
        RSA_get0_key(rsa.Get(), NULL, &e, NULL);

#endif

        v8::Local<v8::Object> v8Buffer = bn2buf2(e);
		info.GetReturnValue().Set(v8Buffer);
	}
}

// NAN_METHOD(WKey::GenerateRsa) {
void WKey::GenerateRsa(const v8::FunctionCallbackInfo<v8::Value>& info) {
	LOG_FUNC();

	int modulus = Nan::To<int>(info[0]).FromJust();
	int publicExponent = Nan::To<int>(info[1]).FromJust();
	Nan::Callback *callback = new Nan::Callback(info[2].As<v8::Function>());

	Nan::AsyncQueueWorker(new AsyncRsaGenerateKey(callback, modulus, publicExponent, info.Data()));
}

/*
 * namedCurve: number
 * cb: function
 */
// NAN_METHOD(WKey::GenerateEc) {
void WKey::GenerateEc(const v8::FunctionCallbackInfo<v8::Value>& info) {
	LOG_FUNC();

	int namedCurve = Nan::To<int>(info[0]).FromJust();
	Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());

	Nan::AsyncQueueWorker(new AsyncEcGenerateKey(callback, namedCurve, info.Data()));
}

/*
 * key_type: number
 * cb: function
 */
NAN_METHOD(WKey::ExportJwk) {
	LOG_FUNC();

	WKey *wkey = WKey::Unwrap<WKey>(info.This());

	int key_type = Nan::To<int>(info[0]).FromJust();

    switch (EVP_PKEY_base_id(wkey->data->Get())) {
	case EVP_PKEY_RSA:
	case EVP_PKEY_EC:
		break;
	default:
		v8::Local<v8::Value> argv[] = {
			Nan::New("Unsupported Key in use").ToLocalChecked()
		};

		if (!info[1]->IsUndefined()) {
			Nan::CallAsFunction(Nan::To<v8::Object>(info[1]).ToLocalChecked(), info.This(), 1, argv);
			return;
		}
		else {
			Nan::ThrowError(Nan::New("Unsupported Key in use").ToLocalChecked());
			return;
		}
	}

	Nan::Callback *callback = !info[1]->IsUndefined() ? new Nan::Callback(info[1].As<v8::Function>()) : NULL;

    switch (EVP_PKEY_base_id(wkey->data->Get())) {
	case EVP_PKEY_RSA: {
		if (callback)
			Nan::AsyncQueueWorker(new AsyncExportJwkRsa(callback, key_type, wkey->data));
		else {
			Handle<JwkRsa> jwk = JwkRsa::From(wkey->data, key_type);

			v8::Local<v8::Object> v8Jwk = Nan::New<v8::Object>();

			LOG_INFO("Convert RSA to JWK");
			Nan::Set(v8Jwk, Nan::New(JWK_ATTR_KTY).ToLocalChecked(), Nan::New(jwk->kty).ToLocalChecked());
			LOG_INFO("Get RSA public key");
			Nan::Set(v8Jwk, Nan::New(JWK_ATTR_N).ToLocalChecked(), bn2buf(jwk->n.Get()));
			Nan::Set(v8Jwk, Nan::New(JWK_ATTR_E).ToLocalChecked(), bn2buf(jwk->e.Get()));
			if (key_type == NODESSL_KT_PRIVATE) {
				LOG_INFO("Get RSA private key");
				Nan::Set(v8Jwk, Nan::New(JWK_ATTR_D).ToLocalChecked(), bn2buf(jwk->d.Get()));
				Nan::Set(v8Jwk, Nan::New(JWK_ATTR_P).ToLocalChecked(), bn2buf(jwk->p.Get()));
				Nan::Set(v8Jwk, Nan::New(JWK_ATTR_Q).ToLocalChecked(), bn2buf(jwk->q.Get()));
				Nan::Set(v8Jwk, Nan::New(JWK_ATTR_DP).ToLocalChecked(), bn2buf(jwk->dp.Get()));
				Nan::Set(v8Jwk, Nan::New(JWK_ATTR_DQ).ToLocalChecked(), bn2buf(jwk->dq.Get()));
				Nan::Set(v8Jwk, Nan::New(JWK_ATTR_QI).ToLocalChecked(), bn2buf(jwk->qi.Get()));
			}

			info.GetReturnValue().Set(v8Jwk);
			return;
		}
		break;
	}
	case EVP_PKEY_EC:
		if (callback)
			Nan::AsyncQueueWorker(new AsyncEcExportJwk(callback, key_type, wkey->data));
		else {
			Handle<JwkEc> jwk = JwkEc::From(wkey->data, key_type);

			v8::Local<v8::Object> v8Jwk = Nan::New<v8::Object>();

			LOG_INFO("Convert EC to JWK");
			Nan::Set(v8Jwk, Nan::New(JWK_ATTR_KTY).ToLocalChecked(), Nan::New(jwk->kty).ToLocalChecked());
			LOG_INFO("Get EC public key");
			Nan::Set(v8Jwk, Nan::New(JWK_ATTR_CRV).ToLocalChecked(), Nan::New<v8::Number>(jwk->crv));
			Nan::Set(v8Jwk, Nan::New(JWK_ATTR_X).ToLocalChecked(), bn2buf(jwk->x.Get()));
			Nan::Set(v8Jwk, Nan::New(JWK_ATTR_Y).ToLocalChecked(), bn2buf(jwk->y.Get()));
			if (key_type == NODESSL_KT_PRIVATE) {
				LOG_INFO("Get RSA private key");
				Nan::Set(v8Jwk, Nan::New(JWK_ATTR_D).ToLocalChecked(), bn2buf(jwk->d.Get()));
			}

			info.GetReturnValue().Set(v8Jwk);
			return;
		}
		break;
	}
}

NAN_METHOD(WKey::ExportSpki) {
	LOG_FUNC();

	WKey *wkey = WKey::Unwrap<WKey>(info.This());

	Nan::Callback *callback = new Nan::Callback(info[0].As<v8::Function>());

	Nan::AsyncQueueWorker(new AsyncExportSpki(callback, wkey->data));
}

NAN_METHOD(WKey::ExportPkcs8) {
	LOG_FUNC();

	WKey *wkey = WKey::Unwrap<WKey>(info.This());

	Nan::Callback *callback = new Nan::Callback(info[0].As<v8::Function>());

	Nan::AsyncQueueWorker(new AsyncExportPkcs8(callback, wkey->data));
}

// NAN_METHOD(WKey::ImportPkcs8) {
void WKey::ImportPkcs8(const v8::FunctionCallbackInfo<v8::Value>& info) {
	LOG_FUNC();

	Handle<std::string> in = v8Buffer_to_String(info[0]);

	Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());

	Nan::AsyncQueueWorker(new AsyncImportPkcs8(callback, in, info.Data()));
}

/*
 * in: buffer
 */
// NAN_METHOD(WKey::ImportSpki) {
void WKey::ImportSpki(const v8::FunctionCallbackInfo<v8::Value>& info) {
	LOG_FUNC();

	Handle<std::string> in = v8Buffer_to_String(info[0]);

	Nan::Callback *callback = new Nan::Callback(info[1].As<v8::Function>());

	Nan::AsyncQueueWorker(new AsyncImportSpki(callback, in, info.Data()));
}

/*
 * jwk: v8::Object
 * key_type: number
 */
// NAN_METHOD(WKey::ImportJwk) {
void WKey::ImportJwk(const v8::FunctionCallbackInfo<v8::Value>& info) {
	LOG_FUNC();

	int key_type = Nan::To<int>(info[1]).FromJust();

    v8::Local<v8::Object> v8Jwk = Nan::To<v8::Object>(info[0]).ToLocalChecked();
	Nan::Utf8String v8Kty(Nan::Get(v8Jwk, Nan::New(JWK_ATTR_KTY).ToLocalChecked()).ToLocalChecked());

	if (!(strcmp(*v8Kty, JWK_KTY_RSA) == 0 || strcmp(*v8Kty, JWK_KTY_EC) == 0)) {
		v8::Local<v8::Value> argv[] = {
			Nan::New("Unsupported Key in use").ToLocalChecked()
		};

		if (!info[2]->IsUndefined()) {
			Nan::CallAsFunction(Nan::To<v8::Object>(info[2]).ToLocalChecked(), info.This(), 1, argv);
			return;
		}
		else {
			Nan::ThrowError(Nan::New("Unsupported Key in use").ToLocalChecked());
			return;
		}
	}

	Nan::Callback *callback = !info[2]->IsUndefined() ? new Nan::Callback(info[2].As<v8::Function>()) : NULL;

	if (strcmp(*v8Kty, JWK_KTY_RSA) == 0) {
		Handle<JwkRsa> jwk(new JwkRsa());

		LOG_INFO("set public key");
		v8Object_get_BN(v8Jwk, n, jwk, n);
		v8Object_get_BN(v8Jwk, e, jwk, e);

		if (key_type == NODESSL_KT_PRIVATE) {
			LOG_INFO("set private key");
			v8Object_get_BN(v8Jwk, d, jwk, d);
			v8Object_get_BN(v8Jwk, p, jwk, p);
			v8Object_get_BN(v8Jwk, q, jwk, q);
			v8Object_get_BN(v8Jwk, dp, jwk, dp);
			v8Object_get_BN(v8Jwk, dq, jwk, dq);
			v8Object_get_BN(v8Jwk, qi, jwk, qi);
		}

		if (callback)
			Nan::AsyncQueueWorker(new AsyncImportJwkRsa(callback, jwk, key_type, info.Data()));
		else {
			Handle<ScopedEVP_PKEY> pkey = jwk->To(key_type);
		  AddonData* addon_data = static_cast<AddonData*>(info.Data().As<v8::External>()->Value());
			v8::Local<v8::Object> v8Key = WKey::NewInstance(addon_data);
			WKey *wkey = WKey::Unwrap<WKey>(v8Key);
			wkey->data = pkey;

			return info.GetReturnValue().Set(v8Key);
		}
	}
	else {
		Handle<JwkEc> jwk(new JwkEc());

		LOG_INFO("set public key");
		v8Object_get_BN(v8Jwk, x, jwk, x);
		v8Object_get_BN(v8Jwk, y, jwk, y);
		jwk->crv = Nan::To<int>(Nan::Get(v8Jwk, Nan::New(JWK_ATTR_CRV).ToLocalChecked()).ToLocalChecked()).FromJust();

		if (key_type == NODESSL_KT_PRIVATE) {
			LOG_INFO("set private key");
			v8Object_get_BN(v8Jwk, d, jwk, d);
		}

		if (callback)
			Nan::AsyncQueueWorker(new AsyncEcImportJwk(callback, jwk, key_type, info.Data()));
		else {
			Handle<ScopedEVP_PKEY> pkey = jwk->To(key_type);
		  AddonData* addon_data = static_cast<AddonData*>(info.Data().As<v8::External>()->Value());
			v8::Local<v8::Object> v8Key = WKey::NewInstance(addon_data);
			WKey *wkey = WKey::Unwrap<WKey>(v8Key);
			wkey->data = pkey;

			return info.GetReturnValue().Set(v8Key);
		}
	}

}

/*
 * digestName: string
 * data: Buffer
 */
NAN_METHOD(WKey::Sign) {
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
	WKey *wkey = WKey::Unwrap<WKey>(info.This());
	Handle<ScopedEVP_PKEY> pkey = wkey->data;

	LOG_INFO("Check key RSA, EC");
    switch (EVP_PKEY_base_id(pkey->Get())) {
	case EVP_PKEY_RSA:
	case EVP_PKEY_EC:
		break;
	default:
		v8::Local<v8::Value> argv[] = {
			Nan::New("Unsupported Key in use").ToLocalChecked()
		};

		Nan::CallAsFunction(Nan::To<v8::Object>(info[2]).ToLocalChecked(), info.This(), 1, argv);
		return;
	}

	Nan::Callback *callback = new Nan::Callback(info[2].As<v8::Function>());
    switch (EVP_PKEY_base_id(pkey->Get())) {
	case EVP_PKEY_RSA:
		Nan::AsyncQueueWorker(new AsyncSignRsa(callback, md, pkey, hBio));
		break;
	case EVP_PKEY_EC:
		Nan::AsyncQueueWorker(new AsyncEcdsaSign(callback, md, pkey, hBio));
		break;
	}
}

/*
* digestName: string
* data: Buffer
* signature: Buffer
* cb: function
*/
NAN_METHOD(WKey::Verify) {
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
	WKey *wkey = WKey::Unwrap<WKey>(info.This());
	Handle<ScopedEVP_PKEY> pkey = wkey->data;

	LOG_INFO("Check key RSA, EC");
	switch (EVP_PKEY_base_id(pkey->Get())) {
	case EVP_PKEY_RSA:
	case EVP_PKEY_EC:
		break;
	default:

		v8::Local<v8::Value> argv[] = {
			Nan::New("Unsupported Key in use").ToLocalChecked()
		};

		Nan::CallAsFunction(Nan::To<v8::Object>(info[2]).ToLocalChecked(), info.This(), 1, argv);
		return;
	}

	Nan::Callback *callback = new Nan::Callback(info[3].As<v8::Function>());
	switch (EVP_PKEY_base_id(pkey->Get())) {
	case EVP_PKEY_RSA:
		Nan::AsyncQueueWorker(new AsyncVerifyRsa(callback, md, pkey, data, sig));
		break;
	case EVP_PKEY_EC:
		Nan::AsyncQueueWorker(new AsyncEcdsaVerify(callback, md, pkey, data, sig));
		break;
	}
}

/*
* digestName: string
* data: Buffer
* label: Buffer
* decrypt: boolean
* cb: function
*/
NAN_METHOD(WKey::RsaOaepEncDec) {
	LOG_FUNC();

	LOG_INFO("digestName");
    Nan::Utf8String v8DigestName(Nan::To<v8::String>(info[0]).ToLocalChecked());
	const EVP_MD *md = EVP_get_digestbyname(*v8DigestName);
	if (!md) {
		Nan::ThrowError("Unknown digest name");
		return;
	}

	LOG_INFO("data");
	Handle<std::string> hData = v8Buffer_to_String(info[1]);

	LOG_INFO("label");
	Handle<std::string> hLabel(new std::string());
	if (!info[2]->IsNull()) {
		hLabel = v8Buffer_to_String(info[2]);
	}

	LOG_INFO("decrypt");
    bool decrypt = Nan::To<bool>(info[3]).FromJust();

	LOG_INFO("this->Key");
	WKey *wKey = WKey::Unwrap<WKey>(info.This());
	Handle<ScopedEVP_PKEY> hKey = wKey->data;

	Nan::Callback *callback = new Nan::Callback(info[4].As<v8::Function>());

	Nan::AsyncQueueWorker(new AsyncEncrypDecryptRsaOAEP(callback, hKey, md, hData, hLabel, decrypt));
}

/*
 * digestName: string
 * saltLength: number
 * data: Buffer
 * cb: function
 */
NAN_METHOD(WKey::RsaPssSign) {
	LOG_FUNC();

	LOG_INFO("digestName");
    Nan::Utf8String v8DigestName(Nan::To<v8::String>(info[0]).ToLocalChecked());
	const EVP_MD *md = EVP_get_digestbyname(*v8DigestName);
	if (!md) {
		Nan::ThrowError("Unknown digest name");
		return;
	}

	LOG_INFO("saltLength");
	int saltLength = Nan::To<int>(info[1]).FromJust();

	LOG_INFO("data");
	Handle<std::string> hData = v8Buffer_to_String(info[2]);

	LOG_INFO("this->Key");
	WKey *wKey = WKey::Unwrap<WKey>(info.This());
	Handle<ScopedEVP_PKEY> hKey = wKey->data;

	Nan::Callback *callback = new Nan::Callback(info[3].As<v8::Function>());

	Nan::AsyncQueueWorker(new AsyncSignRsaPSS(callback, md, hKey, saltLength, hData));
}

/*
* digestName: string
* saltLength: number
* data: Buffer
* signature: Buffer
* cb: function
*/
NAN_METHOD(WKey::RsaPssVerify) {
	LOG_FUNC();

	LOG_INFO("digestName");
    Nan::Utf8String v8DigestName(Nan::To<v8::String>(info[0]).ToLocalChecked());
	const EVP_MD *md = EVP_get_digestbyname(*v8DigestName);
	if (!md) {
		Nan::ThrowError("Unknown digest name");
		return;
	}

	LOG_INFO("saltLength");
	int saltLength = Nan::To<int>(info[1]).FromJust();

	LOG_INFO("data");
	Handle<std::string> hData = v8Buffer_to_String(info[2]);

	LOG_INFO("signature");
	Handle<std::string> hSignature= v8Buffer_to_String(info[3]);

	LOG_INFO("this->Key");
	WKey *wKey = WKey::Unwrap<WKey>(info.This());
	Handle<ScopedEVP_PKEY> hKey = wKey->data;

	Nan::Callback *callback = new Nan::Callback(info[4].As<v8::Function>());

	Nan::AsyncQueueWorker(new AsyncVerifyRsaPSS(callback, md, hKey, saltLength, hData, hSignature));
}

/*
 * publicKey: Key
 * derivedLen: number
 * cb: function
 */
NAN_METHOD(WKey::EcdhDeriveKey) {
	LOG_FUNC();

	LOG_INFO("publicKey");
    WKey *wPubKey = WKey::Unwrap<WKey>(Nan::To<v8::Object>(info[0]).ToLocalChecked());
	Handle<ScopedEVP_PKEY> hPubKey = wPubKey->data;

	LOG_INFO("derivedLen");
	int derivedLen = Nan::To<int>(info[1]).FromJust();

	LOG_INFO("this->Key");
	WKey *wPKey = WKey::Unwrap<WKey>(info.This());
	Handle<ScopedEVP_PKEY> hPKey = wPKey->data;

	Nan::Callback *callback = new Nan::Callback(info[2].As<v8::Function>());

	Nan::AsyncQueueWorker(new AsyncEcdhDeriveKey(callback, hPKey, hPubKey, derivedLen));
}

/*
* publicKey: Key
* lengthBits: number
* cb: function
*/
NAN_METHOD(WKey::EcdhDeriveBits) {
	LOG_FUNC();

	LOG_INFO("publicKey");
    WKey *wPubKey = WKey::Unwrap<WKey>(Nan::To<v8::Object>(info[0]).ToLocalChecked());
	Handle<ScopedEVP_PKEY> hPubKey = wPubKey->data;

	LOG_INFO("lengthBits");
	int lengthBits = Nan::To<int>(info[1]).FromJust();

	LOG_INFO("this->Key");
	WKey *wPKey = WKey::Unwrap<WKey>(info.This());
	Handle<ScopedEVP_PKEY> hPKey = wPKey->data;

	Nan::Callback *callback = new Nan::Callback(info[2].As<v8::Function>());

	Nan::AsyncQueueWorker(new AsyncEcdhDeriveBits(callback, hPKey, hPubKey, lengthBits));
}
