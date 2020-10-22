dnl config.m4 for extension ip2proxy 

dnl Base file contributed by Guruswamy Basavaiah

PHP_ARG_WITH(ip2proxy, for ip2proxy support,
dnl Make sure that the comment is aligned:
[  --with-ip2proxy             Include ip2proxy support])

if test "$PHP_IP2PROXY" != "no"; then

  # --with-ip2proxy -> check with-path
  SEARCH_PATH="/usr/local /usr /opt/local"
  SEARCH_FOR="/include/IP2Proxy.h"
  AC_MSG_CHECKING([for ip2proxy files in default path])
  if test -r $PHP_IP2PROXY/$SEARCH_FOR; then
    IP2PROXY_DIR=$PHP_IP2PROXY
  else # search default path list
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        IP2PROXY_DIR=$i
      fi
    done
  fi

  if test -z "$IP2PROXY_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the IP2Proxy C library])
  else
    AC_MSG_RESULT([found in $IP2PROXY_DIR])
  fi

  # --with-ip2proxy -> add include path
  PHP_ADD_INCLUDE($IP2PROXY_DIR/include)

  # odd PHP4 fix
  if test "x$PHP_LIBDIR" = "x"; then
    PHP_LIBDIR=lib
  fi 

  # --with-ip2proxy -> check for lib and symbol presence
  LIBNAME=IP2Proxy # you may want to change this
  LIBSYMBOLOLD=IP2Proxy_open_mem
  LIBSYMBOLNEW=IP2Proxy_set_lookup_mode

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOLNEW,
  [
    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $IP2PROXY_DIR/$PHP_LIBDIR, IP2PROXY_SHARED_LIBADD)
    AC_DEFINE(HAVE_IP2PROXYLIB,1,[ ])
  ],[
    PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOLOLD,
    [
      PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $IP2PROXY_DIR/$PHP_LIBDIR, IP2PROXY_SHARED_LIBADD)
      AC_DEFINE(HAVE_IP2PROXYLIB,1,[ ])
    ],[
      AC_MSG_ERROR([wrong ip2proxy, lib version >= 6.x.x is required or library not found])
    ],[
      -L$IP2PROXY_DIR/$PHP_LIBDIR
    ])
  ],[
    -L$IP2PROXY_DIR/$PHP_LIBDIR
  ])

  PHP_SUBST(IP2PROXY_SHARED_LIBADD)

  PHP_NEW_EXTENSION(ip2proxy, ip2proxy.c, $ext_shared)
fi

