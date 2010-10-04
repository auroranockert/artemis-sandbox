#include <ruby.h>

/* Yes, I know people will love me for this */
#define __APPLE_API_PRIVATE
#include "sandbox.h"
#undef __APPLE_API_PRIVATE

static VALUE rb_mArtemis;

static VALUE rb_cSandbox;
static VALUE rb_cSandboxError;

static ID sandbox_no_internet;
static ID sandbox_no_network;
static ID sandbox_no_filesystem_write;
static ID sandbox_no_filesystem_write_outside_tmp;
static ID sandbox_pure_computation;

VALUE sandbox_predefined(VALUE self, VALUE profile) {
	char * error;
	
	const char * sandbox_profile;
	
	ID id_profile = rb_to_id(profile);
	
	if (sandbox_no_internet == id_profile) {
		sandbox_profile = kSBXProfileNoInternet;
	} else if (sandbox_no_network == id_profile) {
		sandbox_profile = kSBXProfileNoNetwork;
	} else if (sandbox_no_filesystem_write == id_profile) {
		sandbox_profile = kSBXProfileNoWrite;
	} else if (sandbox_no_filesystem_write_outside_tmp == id_profile) {
		sandbox_profile = kSBXProfileNoWriteExceptTemporary;
	} else if (sandbox_pure_computation == id_profile) {
		sandbox_profile = kSBXProfilePureComputation;
	} else {
		rb_raise(rb_cSandboxError, "invalid profile '%s'", StringValuePtr(profile));
	}
	
	if (sandbox_init(sandbox_profile, SANDBOX_NAMED, &error)) {
		rb_raise(rb_cSandboxError, "invalid profile '%s', error (%s)", StringValuePtr(profile), error);
		
		sandbox_free_error(error);
	}
	
	return Qnil;
}

VALUE sandbox_config(VALUE self, VALUE profile) {
	char * error;
	
	if (sandbox_init(StringValuePtr(profile), SANDBOX_NAMED_EXTERNAL, &error)) {
		rb_raise(rb_cSandboxError, "invalid profile '%s', error (%s)", StringValuePtr(profile), error);
		
		sandbox_free_error(error);
	}
	
	return Qnil;
}

void Init_artemis_sandbox() {
	rb_mArtemis = rb_define_module("Artemis");
	
	rb_cSandbox = rb_define_class_under (rb_mArtemis, "Sandbox", rb_cObject);
	
	rb_cSandboxError = rb_const_get(rb_mArtemis, rb_intern("SandboxError"));
	
	rb_define_singleton_method(rb_cSandbox, "predefined", sandbox_predefined, 1);
	rb_define_singleton_method(rb_cSandbox, "config",     sandbox_config,     1);
	
	sandbox_no_internet                     = rb_intern("no_internet");
	sandbox_no_network                      = rb_intern("no_network");
	sandbox_no_filesystem_write             = rb_intern("no_filesystem_write");
	sandbox_no_filesystem_write_outside_tmp = rb_intern("no_filesystem_write_outside_tmp");
	sandbox_pure_computation                = rb_intern("pure_computation");
}