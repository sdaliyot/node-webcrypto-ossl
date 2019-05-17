#ifndef OSSL_NODE_ASYNC_RSA_H_INCLUDE
#define OSSL_NODE_ASYNC_RSA_H_INCLUDE

#include "../core/common.h"
#include "../rsa/common.h"

class AsyncRsaGenerateKey : public Nan::AsyncWorker {
public:
	AsyncRsaGenerateKey(
		Nan::Callback *callback,
		int modulusBits,
		int publicExponent,
		v8::Local<v8::Value> addon_data_value
	) : AsyncWorker(callback), modulusBits(modulusBits), publicExponent(publicExponent) {
		this->addon_data_value = addon_data_value;
		this->addon_data = static_cast<AddonData*>(addon_data_value.As<v8::External>()->Value());
	}
	~AsyncRsaGenerateKey() {}

	void Execute();
	void HandleOKCallback();

protected:
	int modulusBits;
	int publicExponent;

	AddonData* addon_data;
	v8::Local<v8::Value> addon_data_value;

	// Result
	Handle<ScopedEVP_PKEY> key;
};

class AsyncEncrypDecryptRsaOAEP : public Nan::AsyncWorker {
public:
	AsyncEncrypDecryptRsaOAEP(
		Nan::Callback *callback,
		Handle<ScopedEVP_PKEY> hKey,
		const EVP_MD *md,
		Handle<std::string> hData,
		Handle<std::string> hLabel,
		bool decrypt
		) : AsyncWorker(callback), hKey(hKey), md(md), hData(hData), hLabel(hLabel), decrypt(decrypt) {}

	void Execute();
	void HandleOKCallback();
protected:
	Handle<ScopedEVP_PKEY> hKey;
	const EVP_MD *md;
	Handle<std::string> hData;
	Handle<std::string> hLabel;
	bool decrypt;
	// Result
	Handle<std::string> hResult;
};

class AsyncExportJwkRsa : public Nan::AsyncWorker {
public:
	AsyncExportJwkRsa(
		Nan::Callback *callback,
		int key_type,
		Handle<ScopedEVP_PKEY> key)
		: AsyncWorker(callback), key_type(key_type), key(key) {}
	~AsyncExportJwkRsa() {}

	void Execute();
	void HandleOKCallback();

protected:
	int key_type;
	Handle<ScopedEVP_PKEY> key;
	// Result
	Handle<JwkRsa> jwk;
};

class AsyncExportSpki : public Nan::AsyncWorker {
public:
	AsyncExportSpki(
		Nan::Callback *callback,
		Handle<ScopedEVP_PKEY>key)
		: AsyncWorker(callback), key(key) {}
	~AsyncExportSpki() {}

	void Execute();
	void HandleOKCallback();

protected:
	Handle<ScopedEVP_PKEY> key;
	// Result
	Handle<std::string> buffer;
};

class AsyncExportPkcs8 : public Nan::AsyncWorker {
public:
	AsyncExportPkcs8(
		Nan::Callback *callback,
		Handle<ScopedEVP_PKEY>key)
		: AsyncWorker(callback), key(key) {}
	~AsyncExportPkcs8() {}

	void Execute();
	void HandleOKCallback();

protected:
	Handle<ScopedEVP_PKEY> key;
	// Result
	Handle<std::string> buffer;
};

class AsyncImportPkcs8 : public Nan::AsyncWorker {
public:
	AsyncImportPkcs8(
		Nan::Callback *callback,
		Handle<std::string> in,
		v8::Local<v8::Value> addon_data_value
	) : AsyncWorker(callback), in(in) {
		this->addon_data_value = addon_data_value;
		this->addon_data = static_cast<AddonData*>(addon_data_value.As<v8::External>()->Value());
	}
	~AsyncImportPkcs8() {}

	void Execute();
	void HandleOKCallback();

protected:
	Handle<std::string> in;

	AddonData* addon_data;
	v8::Local<v8::Value> addon_data_value;

	// Result
	Handle<ScopedEVP_PKEY> key;
};

class AsyncImportSpki : public Nan::AsyncWorker {
public:
	AsyncImportSpki(
		Nan::Callback *callback,
		Handle<std::string> in,
		v8::Local<v8::Value> addon_data_value
	) : AsyncWorker(callback), in(in) {
		this->addon_data_value = addon_data_value;
		this->addon_data = static_cast<AddonData*>(addon_data_value.As<v8::External>()->Value());
	}
	~AsyncImportSpki() {}

	void Execute();
	void HandleOKCallback();

private:
	Handle<std::string> in;

	AddonData* addon_data;
	v8::Local<v8::Value> addon_data_value;

	// Result
	Handle<ScopedEVP_PKEY> key;
};

class AsyncImportJwkRsa : public Nan::AsyncWorker {
public:
	AsyncImportJwkRsa(
		Nan::Callback *callback, 
		Handle<JwkRsa> jwk, 
		int key_type,
		v8::Local<v8::Value> addon_data_value
	) : AsyncWorker(callback), jwk(jwk), key_type(key_type) {
		this->addon_data_value = addon_data_value;
		this->addon_data = static_cast<AddonData*>(addon_data_value.As<v8::External>()->Value());
	}
	~AsyncImportJwkRsa() {}

	void Execute();
	void HandleOKCallback();

protected:
	Handle<JwkRsa> jwk;
	int key_type;

	AddonData* addon_data;
	v8::Local<v8::Value> addon_data_value;

	//Result
	Handle<ScopedEVP_PKEY> pkey;
};

class AsyncSignRsa : public Nan::AsyncWorker {
public:
	AsyncSignRsa(
		Nan::Callback *callback,
		const EVP_MD *md,
		Handle<ScopedEVP_PKEY> pkey,
		Handle<std::string> in)
		: AsyncWorker(callback), md(md), pkey(pkey), in(in) {}
	~AsyncSignRsa() {}

	void Execute();
	void HandleOKCallback();

private:
	const EVP_MD *md;
	Handle<ScopedEVP_PKEY> pkey;
	Handle<std::string> in;
	//Result
	Handle<std::string> out;
};

class AsyncVerifyRsa : public Nan::AsyncWorker {
public:
	AsyncVerifyRsa(
		Nan::Callback *callback,
		const EVP_MD *md,
		Handle<ScopedEVP_PKEY> pkey,
		Handle<std::string> in,
		Handle<std::string> signature)
		: AsyncWorker(callback), md(md), pkey(pkey), in(in), signature(signature) {}
	~AsyncVerifyRsa() {}

	void Execute();
	void HandleOKCallback();

private:
	const EVP_MD *md;
	Handle<ScopedEVP_PKEY> pkey;
	Handle<std::string> in;
	Handle<std::string> signature;
	// Result
	bool res;
};

class AsyncSignRsaPSS : public Nan::AsyncWorker {
public:
	AsyncSignRsaPSS(
		Nan::Callback *callback,
		const EVP_MD *md,
		Handle<ScopedEVP_PKEY> pkey,
		int saltLen,
		Handle<std::string> in)
		: AsyncWorker(callback), md(md), pkey(pkey), saltLen(saltLen), in(in) {}
	~AsyncSignRsaPSS() {}

	void Execute();
	void HandleOKCallback();

private:
	const EVP_MD *md;
	Handle<ScopedEVP_PKEY> pkey;
	int saltLen;
	Handle<std::string> in;
	//Result
	Handle<std::string> out;
};

class AsyncVerifyRsaPSS : public Nan::AsyncWorker {
public:
	AsyncVerifyRsaPSS(
		Nan::Callback *callback,
		const EVP_MD *md,
		Handle<ScopedEVP_PKEY> pkey,
		int saltLen,
		Handle<std::string> in,
		Handle<std::string> signature)
		: AsyncWorker(callback), md(md), pkey(pkey), saltLen(saltLen), in(in), signature(signature) {}
	~AsyncVerifyRsaPSS() {}

	void Execute();
	void HandleOKCallback();

private:
	const EVP_MD *md;
	Handle<ScopedEVP_PKEY> pkey;
	int saltLen;
	Handle<std::string> in;
	Handle<std::string> signature;
	// Result
	bool res;
};

#include "common.h"

#endif // OSSL_NODE_ASYNC_RSA_H_INCLUDE