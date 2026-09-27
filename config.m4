PHP_ARG_ENABLE([tlsrepair],
  [whether to enable tlsrepair support],
  [AS_HELP_STRING([--enable-tlsrepair],
    [Enable tlsrepair support])],
  [no])

AS_VAR_IF([PHP_TLSREPAIR], [no],, [

  TLSREPAIR_DIR="$HOME/tls"

  PHP_ADD_INCLUDE([$TLSREPAIR_DIR/include])

  TLSREPAIR_SHARED_LIBADD="-L$TLSREPAIR_DIR/build -Wl,-rpath,$TLSREPAIR_DIR/build -ltlsrepair"

  PHP_SUBST([TLSREPAIR_SHARED_LIBADD])

  AC_DEFINE([HAVE_TLSREPAIR], [1],
    [Define to 1 if the PHP extension 'tlsrepair' is available.])

  PHP_NEW_EXTENSION(
    [php_tlsrepair],
    [php_tlsrepair.c],
    [$ext_shared],
    ,
    [-DZEND_ENABLE_STATIC_TSRMLS_CACHE=1]
  )
])