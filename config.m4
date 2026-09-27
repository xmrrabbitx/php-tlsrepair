PHP_ARG_WITH([tlsrepair],
  [for TLSRepair support],
  [AS_HELP_STRING([--with-tlsrepair],
    [Include TLSRepair support])],
  [yes])

if test "$PHP_TLSREPAIR" != "no"; then

  PKG_CHECK_MODULES(
    [TLSREPAIR],
    [tlsrepair >= 1.0.0],
    [],
    [
      AC_MSG_FAILURE([
        TLSRepair library not found.

        Make sure TLSRepair is installed and pkg-config can find
        tlsrepair.pc.
      ])
    ]
  )

  CFLAGS="$CFLAGS $TLSREPAIR_CFLAGS"

  TLSREPAIR_SHARED_LIBADD="$TLSREPAIR_LIBS"

  PHP_SUBST([TLSREPAIR_SHARED_LIBADD])

  AC_DEFINE(
    [HAVE_TLSREPAIR],
    [1],
    [Define to 1 if the PHP extension 'tlsrepair' is available.]
  )

  PHP_NEW_EXTENSION(
    [php_tlsrepair],
    [php_tlsrepair.c],
    [$ext_shared],
    ,
    [-DZEND_ENABLE_STATIC_TSRMLS_CACHE=1]
  )

fi