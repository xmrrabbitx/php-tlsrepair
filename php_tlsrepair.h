#ifndef PHP_TLSREPAIR_H
#define PHP_TLSREPAIR_H

extern zend_module_entry tlsrepair_module_entry;

#define phpext_tlsrepair_ptr &tlsrepair_module_entry

#define PHP_TLSREPAIR_VERSION "0.1.0"

#ifdef PHP_WIN32
# define PHP_TLSREPAIR_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
# define PHP_TLSREPAIR_API __attribute__((visibility("default")))
#else
# define PHP_TLSREPAIR_API
#endif

#endif
