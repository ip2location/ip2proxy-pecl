#ifndef PHP_IP2PROXY_H
#define PHP_IP2PROXY_H

/* The IP2Proxy.h checks for this but it's defined by config.w32 */
#if defined(PHP_WIN32) && defined(HAVE_IP2PROXY_H)
#undef HAVE_IP2PROXY_H
#endif

#include <IP2Proxy.h>
#include <php.h>

#define PHP_IP2PROXY_EXTNAME "ip2proxy"
#define PHP_IP2PROXY_VERSION "3.0.2"

PHP_MINIT_FUNCTION(ip2proxy);
PHP_MSHUTDOWN_FUNCTION(ip2proxy);
PHP_MINFO_FUNCTION(ip2proxy);

extern zend_module_entry ip2proxy_module_entry;
#define phpext_ip2proxy_ptr &ip2proxy_module_entry

PHP_FUNCTION(ip2proxy_open);
PHP_FUNCTION(ip2proxy_open_mem);
PHP_FUNCTION(ip2proxy_get_country_short);
PHP_FUNCTION(ip2proxy_get_country_long);
PHP_FUNCTION(ip2proxy_get_region);
PHP_FUNCTION(ip2proxy_get_city);
PHP_FUNCTION(ip2proxy_get_isp);
PHP_FUNCTION(ip2proxy_get_domain);
PHP_FUNCTION(ip2proxy_get_usage_type);
PHP_FUNCTION(ip2proxy_get_asn);
PHP_FUNCTION(ip2proxy_get_as);
PHP_FUNCTION(ip2proxy_get_last_seen);
PHP_FUNCTION(ip2proxy_get_threat);
PHP_FUNCTION(ip2proxy_get_provider);
PHP_FUNCTION(ip2proxy_get_all);
PHP_FUNCTION(ip2proxy_close);
PHP_FUNCTION(ip2proxy_delete_shm);

ZEND_BEGIN_MODULE_GLOBALS(ip2proxy)
        IP2Proxy *ip2proxy_ptr;
ZEND_END_MODULE_GLOBALS(ip2proxy)

#ifdef ZTS
#define IP2PROXY_G(v) TSRMG(ip2proxy_globals_id, zend_ip2proxy_globals *, v)
#else
#define IP2PROXY_G(v) (ip2proxy_globals.v)
#endif


#endif 
