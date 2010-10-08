/*
* Interface to sandbox/seatbelt.
*
* This file is covered by the Ruby license. See LICENSE for more details.
*
* Copyright (C) 2010, Apple Inc. All rights reserved.
*/

#include <ruby.h>

/* TODO: Not use private APIs. */
#define __APPLE_API_PRIVATE
#include "sandbox.h"
#undef __APPLE_API_PRIVATE

static VALUE rb_mArtemis;

static VALUE rb_cSandbox;

typedef struct {
	const char * profile;
	uint64_t flags;
} rb_sandbox_t;

static void rb_sandbox_s_free(void * instance) {
	free(((rb_sandbox_t *)instance)->profile);
	free(instance);
}

static VALUE rb_sandbox_s_alloc(VALUE klass) {
	rb_sandbox_t * sb = ALLOC(rb_sandbox_t);
	
	sb->profile = NULL;
	sb->flags = 0;
	
	return Data_Wrap_Struct(klass, NULL, rb_sandbox_s_free, sb);
}

static inline VALUE sandbox(const char * name, uint64_t flags) {
	VALUE obj = rb_sandbox_s_alloc(rb_cSandbox);
	
	rb_sandbox_t * box;
	
	Data_Get_Struct(obj, rb_sandbox_t, box);
	
	box->profile = strcpy(malloc(strlen(name)), name);
	box->flags = flags;
	
	return rb_obj_freeze(obj);
}

static inline VALUE predefined_sandbox(const char * name) {
	return sandbox(name, SANDBOX_NAMED);
}

static VALUE rb_sandbox_s_no_internet(VALUE klass) {
	return predefined_sandbox(kSBXProfileNoInternet);
}

static VALUE rb_sandbox_s_no_network(VALUE klass) {
	return predefined_sandbox(kSBXProfileNoNetwork);
}

static VALUE rb_sandbox_s_no_writes(VALUE klass) {
	return predefined_sandbox(kSBXProfileNoWrite);
}

static VALUE rb_sandbox_s_temporary_writes(VALUE klass) {
	return predefined_sandbox(kSBXProfileNoWriteExceptTemporary);
}

static VALUE rb_sandbox_s_pure_computation(VALUE klass) {
	return predefined_sandbox(kSBXProfilePureComputation);
}

static VALUE rb_sandbox_s_from_file(VALUE klass, VALUE filename) {
	return sandbox(StringValuePtr(filename), SANDBOX_NAMED_EXTERNAL);
}

static VALUE rb_sandbox_apply(VALUE self)
{
	rb_sandbox_t * box;
	
	Data_Get_Struct(self, rb_sandbox_t, box);
	
	char *error = NULL;
	
	if (sandbox_init(box->profile, box->flags, &error) == -1) {
	    rb_raise(rb_eSecurityError, "Couldn't apply sandbox: `%s`", error);
	}
	
	return Qnil;
}

void Init_artemis_sandbox() {
	rb_mArtemis = rb_define_module("Artemis");
	
	rb_cSandbox = rb_define_class_under (rb_mArtemis, "Sandbox", rb_cData);
	
	rb_define_alloc_func(rb_cSandbox, rb_sandbox_s_alloc);
	
	rb_define_singleton_method(rb_cSandbox, "no_internet",      rb_sandbox_s_no_internet, 0);
	rb_define_singleton_method(rb_cSandbox, "no_network",       rb_sandbox_s_no_network, 0);
	rb_define_singleton_method(rb_cSandbox, "no_writes",        rb_sandbox_s_no_writes, 0);
	rb_define_singleton_method(rb_cSandbox, "temporary_writes", rb_sandbox_s_temporary_writes, 0);
	rb_define_singleton_method(rb_cSandbox, "pure_computation", rb_sandbox_s_pure_computation, 0);
	
	rb_define_singleton_method(rb_cSandbox, "from_file", rb_sandbox_s_from_file, 1);
	
	rb_define_method(rb_cSandbox, "apply!", rb_sandbox_apply, 0);
}
