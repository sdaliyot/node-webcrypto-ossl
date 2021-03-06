#ifndef OSSL_NODE_ASYNC_PBKDF2_H_INCLUDE
#define OSSL_NODE_ASYNC_PBKDF2_H_INCLUDE

#include "../core/common.h"
#include "../pbkdf2/common.h"

class AsyncPbkdf2Import : public Nan::AsyncWorker {
public:
	AsyncPbkdf2Import(
		Nan::Callback *callback,
		Handle<std::string> hInput,
		v8::Local<v8::Value> addon_data_value
	) : AsyncWorker(callback), hInput(hInput) {
		this->addon_data_value = addon_data_value;
		this->addon_data = static_cast<AddonData*>(addon_data_value.As<v8::External>()->Value());
	}
	~AsyncPbkdf2Import() {}

	void Execute();
	void HandleOKCallback();

protected:
	Handle<std::string> hInput;

	AddonData* addon_data;
	v8::Local<v8::Value> addon_data_value;

	// Result
	Handle<ScopedPbkdf2> hKey;
};

class AsyncPbkdf2DeriveBits : public Nan::AsyncWorker {
public:
	AsyncPbkdf2DeriveBits(
		Nan::Callback *callback,
		Handle<ScopedPbkdf2> pkey,
		const EVP_MD *md,
		Handle<std::string> salt,
		int iterations,
		int bits_length)
		: AsyncWorker(callback), pkey(pkey), md(md), salt(salt), iterations(iterations), bits_length(bits_length) {}
	~AsyncPbkdf2DeriveBits() {}

	void Execute();
	void HandleOKCallback();

private:
	Handle<ScopedPbkdf2> pkey;
	const EVP_MD *md;
	Handle<std::string> salt;
	int iterations;
	int bits_length;
	// Result
	Handle<std::string> res;
};

#endif // OSSL_NODE_ASYNC_PBKDF2_H_INCLUDE