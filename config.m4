PHP_ARG_WITH([tlsrepair],
  [for TLSRepair support],
  [AS_HELP_STRING([--with-tlsrepair[=DIR]],
    [Include TLSRepair support])],
  [yes])

if test "$PHP_TLSREPAIR" != "no"; then

  AS_VAR_IF([PHP_TLSREPAIR], [yes], [
    for i in /usr/local /usr; do
      AS_IF(
        [test -r "$i/include/tlsrepair/tlsrepair.h"],
        [TLSREPAIR_DIR="$i"; break]
      )
    done
  ], [
    TLSREPAIR_DIR="$PHP_TLSREPAIR"
  ])

  AS_VAR_IF([TLSREPAIR_DIR],, [
    AC_MSG_FAILURE([
      Cannot find TLSRepair.
      Please install TLSRepair or use --with-tlsrepair=/path/to/prefix
    ])
  ])

  AC_MSG_CHECKING([for TLSRepair in $TLSREPAIR_DIR])

  AS_IF(
    [test -r "$TLSREPAIR_DIR/include/tlsrepair/tlsrepair.h"],
    [AC_MSG_RESULT([yes])],
    [
      AC_MSG_RESULT([no])
      AC_MSG_FAILURE([
        TLSRepair header not found:
        $TLSREPAIR_DIR/include/tlsrepair/tlsrepair.h
      ])
    ]
  )

  PHP_ADD_INCLUDE([$TLSREPAIR_DIR/include])

  PHP_CHECK_LIBRARY(
    [tlsrepair],
    [tlsrepair_global_init],
    [
      PHP_ADD_LIBRARY_WITH_PATH(
        [tlsrepair],
        [$TLSREPAIR_DIR/$PHP_LIBDIR],
        [TLSREPAIR_SHARED_LIBADD]
      )
    ],
    [
      AC_MSG_FAILURE([
        TLSRepair library not found or tlsrepair_global_init() is missing
      ])
    ],
    [-L$TLSREPAIR_DIR/$PHP_LIBDIR]
  )

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

fi