#include "javascript.hpp"

#include <cstdio>
#include <string>

using namespace std;

static duk_ret_t native_print (duk_context *ctx) {
  int nargs = duk_get_top(ctx);

  for (int i = 0; i < nargs; i++) {
    if (i < nargs - 1) {
      fprintf(stderr, "%s ", duk_to_string(ctx, i));
    } else {
      fprintf(stderr, "%s\n", duk_to_string(ctx, i));
    }
  }

  return 0;
}

static duk_ret_t native_read_file (duk_context *ctx) {
  int nargs = duk_get_top(ctx);

  if (nargs != 1) {
    return duk_error(ctx, DUK_ERR_TYPE_ERROR, "readfile() requires a filename");
  }

  const char *filename = duk_to_string(ctx, 0);

  FILE *in = fopen(filename, "r");
  size_t size;

  if (in == nullptr) {
    return duk_error(ctx, DUK_ERR_ERROR, "readfile() unable to open file %s", filename);
  }

  fseek(in, 0, SEEK_END);
  size = ftell(in);

  if (size == 0) {
    fclose(in);
    return 0;
  }

  char *buf = (char *) duk_alloc(ctx, size + 1);

  fseek(in, 0, SEEK_SET);

  size_t len = fread(buf, sizeof(char), size + 1, in);
  buf[len] = '\0';

  duk_push_string(ctx, buf);

  return 1;  /* no return value (= undefined) */
}

// read the bootstrap file, get it compiled, and into the global context
static void bootstrap (duk_context *ctx, const char *basedir) {
  std::string filename = std::string(basedir) + std::string("/bootstrap.js");

  FILE *in = fopen(filename.c_str(), "r");
  size_t size;

  if (in == nullptr) {
    fprintf(stderr, "ERROR: Unable to load '%s' to bootstrap\n", filename.c_str());
    return;
  }

  fseek(in, 0, SEEK_END);
  size = ftell(in);

  if (size == 0) {
    fclose(in);
    return;
  }

  char *buf = (char *) duk_alloc(ctx, size + 1);

  fseek(in, 0, SEEK_SET);

  size_t len = fread(buf, sizeof(char), size + 1, in);
  buf[len] = '\0';

  int ret = duk_peval_string(ctx, buf);

  if (ret != 0) {
    fprintf(stderr, "ERROR: bootstrap failed: %s\n", duk_safe_to_string(ctx, -1));
  }
}

duk_context *createJavascriptContext (const char *basedir) {
  duk_context *ctx = duk_create_heap_default();


  // enough functionality to bootstrap
  const duk_function_list_entry my_module_funcs[] = {
    { "readfile", native_read_file, 1 },
    { "print", native_print, DUK_VARARGS },
    { NULL, NULL, 0 }
  };

  // set up the global ProtoTools module
  duk_push_global_object(ctx);
  duk_push_object(ctx);  /* -> [ ... global obj ] */

  // functions
  duk_put_function_list(ctx, -1, my_module_funcs);

  // base directory for everything
  duk_push_string(ctx, "basedir");
  duk_push_string(ctx, basedir);
  duk_put_prop(ctx, -3);

  // name the module
  duk_put_prop_string(ctx, -2, "ProtoTools");  /* -> [ ... global ] */
  duk_pop(ctx);

  // bootstrap
  bootstrap(ctx, basedir);

  //duk_module_duktape_init(ctx);

  return ctx;
}
