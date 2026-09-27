#include "php.h"
#include "php_tlsrepair.h"

#include "Zend/zend_exceptions.h"
#include "ext/standard/info.h"
#include "/home/ahmad/php-src/ext/curl/curl_private.h"

#include <tlsrepair.h>


typedef struct
{
    zend_object std;
    TLSRepair *repair;
} php_tlsrepair_object;


static zend_class_entry *tlsrepair_ce;
static zend_object_handlers tlsrepair_object_handlers;


static inline php_tlsrepair_object *
php_tlsrepair_from_obj(zend_object *object)
{
    return (php_tlsrepair_object *)object;
}


#define Z_TLSREPAIR_P(zv) \
    php_tlsrepair_from_obj(Z_OBJ_P(zv))


static zend_object *
tlsrepair_object_create(zend_class_entry *ce)
{
    php_tlsrepair_object *object;

    object = zend_object_alloc(
        sizeof(php_tlsrepair_object),
        ce
    );

    object->repair = NULL;

    zend_object_std_init(
        &object->std,
        ce
    );

    object_properties_init(
        &object->std,
        ce
    );

    object->std.handlers =
        &tlsrepair_object_handlers;

    return &object->std;
}


static void
tlsrepair_object_free(zend_object *object)
{
    php_tlsrepair_object *intern;

    intern = php_tlsrepair_from_obj(object);

    if (intern->repair != NULL) {
        tlsrepair_destroy(intern->repair);
        intern->repair = NULL;
    }

    zend_object_std_dtor(
        &intern->std
    );
}


PHP_METHOD(TLSRepair, fromCurl)
{
    zval *curl_zv;
    php_curl *curl;
    php_tlsrepair_object *intern;
    TLSRepair *repair;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(curl_zv, curl_ce)
    ZEND_PARSE_PARAMETERS_END();

    curl = Z_CURL_P(curl_zv);

    fprintf(
        stderr,
        "FROM CURL: curl=%p cp=%p\n",
        (void *)curl,
        curl != NULL ? (void *)curl->cp : NULL
    );

    if (curl == NULL || curl->cp == NULL) {
        zend_throw_exception(NULL, "Invalid cURL handle", 0);
        RETURN_THROWS();
    }

    repair = tlsrepair_from_curl(curl->cp);

    fprintf(
        stderr,
        "FROM CURL: repair=%p\n",
        (void *)repair
    );

    if (repair == NULL) {
        zend_throw_exception(
            NULL,
            "Failed to create TLSRepair from cURL handle",
            0
        );
        RETURN_THROWS();
    }

    object_init_ex(return_value, tlsrepair_ce);

    fprintf(
        stderr,
        "FROM CURL: object=%p\n",
        (void *)Z_OBJ_P(return_value)
    );

    intern = Z_TLSREPAIR_P(return_value);

    fprintf(
        stderr,
        "FROM CURL: intern=%p before repair=%p\n",
        (void *)intern,
        (void *)intern->repair
    );

    intern->repair = repair;

    fprintf(
        stderr,
        "FROM CURL: intern=%p after repair=%p\n",
        (void *)intern,
        (void *)intern->repair
    );
}

PHP_METHOD(TLSRepair, setProxy)
{
    zend_string *proxy = NULL;
    php_tlsrepair_object *intern;
    int result;


    ZEND_PARSE_PARAMETERS_START(0, 1)

        Z_PARAM_OPTIONAL

        Z_PARAM_STR_OR_NULL(proxy)

    ZEND_PARSE_PARAMETERS_END();


    intern = Z_TLSREPAIR_P(
        ZEND_THIS
    );

    fprintf(
    stderr,
    "SET PROXY: object=%p intern=%p repair=%p\n",
    (void *)Z_OBJ_P(ZEND_THIS),
    (void *)intern,
    (void *)intern->repair
);

    if (intern->repair == NULL) {

        zend_throw_error(
            NULL,
            "TLSRepair object is not initialized"
        );

        RETURN_THROWS();
    }


    result = tlsrepair_set_proxy(
        intern->repair,
        proxy != NULL
            ? ZSTR_VAL(proxy)
            : NULL
    );


    RETURN_BOOL(
        result != 0
    );
}


PHP_METHOD(TLSRepair, prepare)
{
    php_tlsrepair_object *intern;
    int result;


    ZEND_PARSE_PARAMETERS_NONE();


    intern = Z_TLSREPAIR_P(
        ZEND_THIS
    );


    if (intern->repair == NULL) {

        zend_throw_error(
            NULL,
            "TLSRepair object is not initialized"
        );

        RETURN_THROWS();
    }


    result = tlsrepair_prepare(
        intern->repair
    );


    RETURN_BOOL(
        result != 0
    );
}


PHP_METHOD(TLSRepair, getError)
{
    php_tlsrepair_object *intern;
    const char *error;


    ZEND_PARSE_PARAMETERS_NONE();


    intern = Z_TLSREPAIR_P(
        ZEND_THIS
    );


    if (intern->repair == NULL) {
        RETURN_NULL();
    }


    error = tlsrepair_error(
        intern->repair
    );


    if (error == NULL || *error == '\0') {
        RETURN_NULL();
    }


    RETURN_STRING(
        error
    );
}


ZEND_BEGIN_ARG_INFO_EX(
    arginfo_tlsrepair_from_curl,
    0,
    0,
    1
)

    ZEND_ARG_OBJ_INFO(
        0,
        curl,
        CurlHandle,
        0
    )

ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_INFO_EX(
    arginfo_tlsrepair_set_proxy,
    0,
    0,
    0
)

    ZEND_ARG_TYPE_INFO(
        0,
        proxy,
        IS_STRING,
        1
    )

ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_INFO_EX(
    arginfo_tlsrepair_prepare,
    0,
    0,
    0
)

ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_INFO_EX(
    arginfo_tlsrepair_get_error,
    0,
    0,
    0
)

ZEND_END_ARG_INFO()


static const zend_function_entry tlsrepair_methods[] = {

    PHP_ME(
        TLSRepair,
        fromCurl,
        arginfo_tlsrepair_from_curl,
        ZEND_ACC_PUBLIC | ZEND_ACC_STATIC
    )

    PHP_ME(
        TLSRepair,
        setProxy,
        arginfo_tlsrepair_set_proxy,
        ZEND_ACC_PUBLIC
    )

    PHP_ME(
        TLSRepair,
        prepare,
        arginfo_tlsrepair_prepare,
        ZEND_ACC_PUBLIC
    )

    PHP_ME(
        TLSRepair,
        getError,
        arginfo_tlsrepair_get_error,
        ZEND_ACC_PUBLIC
    )

    PHP_FE_END
};


PHP_MINIT_FUNCTION(tlsrepair)
{
    zend_class_entry ce;


    if (!tlsrepair_global_init()) {

        php_error_docref(
            NULL,
            E_WARNING,
            "Failed to initialize TLSRepair library"
        );

        return FAILURE;
    }


    memcpy(
        &tlsrepair_object_handlers,
        zend_get_std_object_handlers(),
        sizeof(zend_object_handlers)
    );


    tlsrepair_object_handlers.free_obj =
        tlsrepair_object_free;


    tlsrepair_object_handlers.clone_obj =
        NULL;


    INIT_CLASS_ENTRY(
        ce,
        "TLSRepair",
        tlsrepair_methods
    );


    tlsrepair_ce =
        zend_register_internal_class(
            &ce
        );


    tlsrepair_ce->create_object =
        tlsrepair_object_create;


    return SUCCESS;
}


PHP_MSHUTDOWN_FUNCTION(tlsrepair)
{
    tlsrepair_global_cleanup();

    return SUCCESS;
}


PHP_MINFO_FUNCTION(tlsrepair)
{
    php_info_print_table_start();

    php_info_print_table_header(
        2,
        "TLSRepair support",
        "enabled"
    );

    php_info_print_table_row(
        2,
        "Version",
        PHP_TLSREPAIR_VERSION
    );

    php_info_print_table_end();
}


zend_module_entry tlsrepair_module_entry = {

    STANDARD_MODULE_HEADER,

    "tlsrepair",

    NULL,

    PHP_MINIT(tlsrepair),

    PHP_MSHUTDOWN(tlsrepair),

    NULL,

    NULL,

    PHP_MINFO(tlsrepair),

    PHP_TLSREPAIR_VERSION,

    STANDARD_MODULE_PROPERTIES
};


#ifdef ZEND_COMPILE_DL_EXT

# ifdef ZTS

ZEND_TSRMLS_CACHE_DEFINE()

# endif

ZEND_GET_MODULE(tlsrepair)

#endif