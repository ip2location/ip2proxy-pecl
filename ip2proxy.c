
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"

#include "php_ip2proxy.h"

/* For PHP 8 */
#ifndef TSRMLS_CC
#define TSRMLS_CC
#endif

ZEND_DECLARE_MODULE_GLOBALS(ip2proxy)

#define IP2PROXY_RECORD 0

ZEND_BEGIN_ARG_INFO_EX(ip2proxy_open, 0, 0, 1)
	ZEND_ARG_INFO(0, file_path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ip2proxy_open_mem, 0, 0, 1)
	ZEND_ARG_INFO(0, method)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ip2proxy_ip_address, 0, 0, 1)
	ZEND_ARG_INFO(0, ip_address)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ip2proxy_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry ip2proxy_functions_entry[] = {
	PHP_FE(ip2proxy_open, ip2proxy_open)
	PHP_FE(ip2proxy_open_mem, ip2proxy_open_mem)
	PHP_FE(ip2proxy_get_country_short, ip2proxy_ip_address)
	PHP_FE(ip2proxy_get_country_long, ip2proxy_ip_address)
	PHP_FE(ip2proxy_get_region, ip2proxy_ip_address)
	PHP_FE(ip2proxy_get_city, ip2proxy_ip_address)
	PHP_FE(ip2proxy_get_isp, ip2proxy_ip_address)
	PHP_FE(ip2proxy_get_domain, ip2proxy_ip_address)
	PHP_FE(ip2proxy_get_usage_type, ip2proxy_ip_address)
	PHP_FE(ip2proxy_get_asn, ip2proxy_ip_address)
	PHP_FE(ip2proxy_get_as, ip2proxy_ip_address)
	PHP_FE(ip2proxy_get_last_seen, ip2proxy_ip_address)
	PHP_FE(ip2proxy_get_threat, ip2proxy_ip_address)
	PHP_FE(ip2proxy_get_provider, ip2proxy_ip_address)
	PHP_FE(ip2proxy_get_fraud_score, ip2proxy_ip_address
	PHP_FE(ip2proxy_get_all, ip2proxy_ip_address)
	PHP_FE(ip2proxy_close, ip2proxy_void)
	PHP_FE(ip2proxy_delete_shm, ip2proxy_void)
#ifdef PHP_FE_END
	PHP_FE_END
#else
	{NULL, NULL, NULL}
#endif
};

/* the following code creates an entry for the module and registers it with Zend.*/
zend_module_entry ip2proxy_module_entry = {
	STANDARD_MODULE_HEADER,
	PHP_IP2PROXY_EXTNAME,
	ip2proxy_functions_entry,
	PHP_MINIT(ip2proxy),
	PHP_MSHUTDOWN(ip2proxy),
	NULL,
	NULL,
	PHP_MINFO(ip2proxy),
	PHP_IP2PROXY_VERSION,
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_IP2PROXY
ZEND_GET_MODULE(ip2proxy)
#endif

	/* {{{ php_ip2proxy_init_globals
	 *  */
static void php_ip2proxy_init_globals(zend_ip2proxy_globals *ip2proxy_globals)
{
	ip2proxy_globals->ip2proxy_ptr = NULL;
}
/* }}} */

#define PHP_IP2PROXY_DB_CHECK \
	if( IP2PROXY_G(ip2proxy_ptr) == NULL ) { \
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "open db file with ip2proxy_open() before using this function"); \
		RETURN_FALSE; \
	}

/* {{{ PHP_MINIT_FUNCTION
 *  */
PHP_MINIT_FUNCTION(ip2proxy)
{
	ZEND_INIT_MODULE_GLOBALS(ip2proxy, php_ip2proxy_init_globals, NULL);

	/* For memory access type constants */
	REGISTER_LONG_CONSTANT("IP2PROXY_FILE_IO", IP2PROXY_FILE_IO, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IP2PROXY_CACHE_MEMORY", IP2PROXY_CACHE_MEMORY, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IP2PROXY_SHARED_MEMORY", IP2PROXY_SHARED_MEMORY, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("IP2PROXY_RECORD", IP2PROXY_RECORD, CONST_CS | CONST_PERSISTENT);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 *  */
PHP_MSHUTDOWN_FUNCTION(ip2proxy)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 *  */
PHP_MINFO_FUNCTION(ip2proxy)
{
	char buf[32];
	unsigned long ver;

	php_info_print_table_start();
	php_info_print_table_header(2, "ip2proxy support", "enabled");
	php_info_print_table_row(2, "extension version", PHP_IP2PROXY_VERSION);
	ver = IP2Proxy_version_number();
	snprintf(buf, sizeof(buf), "%d.%d.%d", API_VERSION_MAJOR, API_VERSION_MINOR, API_VERSION_RELEASE);
	if (API_VERSION_NUMERIC == ver) {
		php_info_print_table_row(2, "library version", buf);
	} else {
		/* display both headers/runtime versions when differ */
		php_info_print_table_row(2, "library headers version", buf);
		snprintf(buf, sizeof(buf), "%lu.%lu.%lu", ver / 10000, (ver / 100) % 100, ver % 100);
		php_info_print_table_row(2, "library runtime version", buf);
	}
	php_info_print_table_end();
}
/* }}} */


/* {{{ ip2proxy_open("db_filepath")
 * Returns the success or failure */
PHP_FUNCTION(ip2proxy_open)
{
	char * file_path = NULL;
	size_t path_len;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &file_path, &path_len) == FAILURE) {
		return;
	}
	if (IP2PROXY_G(ip2proxy_ptr)) {
		IP2Proxy_close(IP2PROXY_G(ip2proxy_ptr));
	}
	IP2PROXY_G(ip2proxy_ptr) = IP2Proxy_open(file_path);
	if (IP2PROXY_G(ip2proxy_ptr)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}
/* }}} */

/* {{{ ip2proxy_open_mem(method)
 * Returns the success or failure */
PHP_FUNCTION(ip2proxy_open_mem)
{
	long method;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &method) == FAILURE) {
		return;
	}
	/*Shared memory method is not supported*/
	if( method == IP2PROXY_FILE_IO || method == IP2PROXY_CACHE_MEMORY || method == IP2PROXY_SHARED_MEMORY ){
		if(IP2Proxy_set_lookup_mode(IP2PROXY_G(ip2proxy_ptr), method) == -1){
			RETURN_FALSE;
		}
		else{
			RETURN_TRUE;
		}
	}
	else{
		RETURN_FALSE;
	}
}
/* }}} */

/* {{{ ip2proxy_get_country_short("ip_address")
 * Returns ip address's country in short */
PHP_FUNCTION(ip2proxy_get_country_short)
{
	char *ip_address;
	size_t ip_len;
	IP2ProxyRecord *record = NULL;

	PHP_IP2PROXY_DB_CHECK;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ip_address, &ip_len) == FAILURE) {
		return;
	}
	record = IP2Proxy_get_country_short(IP2PROXY_G(ip2proxy_ptr), ip_address);
#if PHP_MAJOR_VERSION >= 7
	RETVAL_STRING(record->country_short);
#else
	RETVAL_STRING(record->country_short, 1);
#endif
	IP2Proxy_free_record(record);
}
/* }}} */

/* {{{ ip2proxy_get_country_long("ip_address")
 * Returns ip address's country in long */
PHP_FUNCTION(ip2proxy_get_country_long)
{
	char *ip_address;
	size_t ip_len;
	IP2ProxyRecord *record = NULL;

	PHP_IP2PROXY_DB_CHECK;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ip_address, &ip_len) == FAILURE) {
		return;
	}
	record = IP2Proxy_get_country_long(IP2PROXY_G(ip2proxy_ptr), ip_address);
#if PHP_MAJOR_VERSION >= 7
	RETVAL_STRING(record->country_long);
#else
	RETVAL_STRING(record->country_long, 1);
#endif
	IP2Proxy_free_record(record);
}
/* }}} */

/* {{{ ip2proxy_get_region("ip_address")
 * Returns ip address's region*/
PHP_FUNCTION(ip2proxy_get_region)
{
	char *ip_address;
	size_t ip_len;
	IP2ProxyRecord *record = NULL;

	PHP_IP2PROXY_DB_CHECK;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ip_address, &ip_len) == FAILURE) {
		return;
	}
	record = IP2Proxy_get_region(IP2PROXY_G(ip2proxy_ptr), ip_address);
#if PHP_MAJOR_VERSION >= 7
	RETVAL_STRING(record->region);
#else
	RETVAL_STRING(record->region, 1);
#endif
	IP2Proxy_free_record(record);
}
/* }}} */

/* {{{ ip2proxy_get_city("ip_address")
 * Returns ip address's city*/
PHP_FUNCTION(ip2proxy_get_city)
{
	char *ip_address;
	size_t ip_len;
	IP2ProxyRecord *record = NULL;

	PHP_IP2PROXY_DB_CHECK;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ip_address, &ip_len) == FAILURE) {
		return;
	}
	record = IP2Proxy_get_city(IP2PROXY_G(ip2proxy_ptr), ip_address);
#if PHP_MAJOR_VERSION >= 7
	RETVAL_STRING(record->city);
#else
	RETVAL_STRING(record->city, 1);
#endif
	IP2Proxy_free_record(record);
}
/* }}} */

/* {{{ ip2proxy_get_("ip_address")
 * Returns ip address's isp information */
PHP_FUNCTION(ip2proxy_get_isp)
{
	char *ip_address;
	size_t ip_len;
	IP2ProxyRecord *record = NULL;

	PHP_IP2PROXY_DB_CHECK;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ip_address, &ip_len) == FAILURE) {
		return;
	}
	record = IP2Proxy_get_isp(IP2PROXY_G(ip2proxy_ptr), ip_address);
#if PHP_MAJOR_VERSION >= 7
	RETVAL_STRING(record->isp);
#else
	RETVAL_STRING(record->isp, 1);
#endif
	IP2Proxy_free_record(record);
}
/* }}} */

/* {{{ ip2proxy_get_domain("ip_address")
 * Returns ip address's domain information */
PHP_FUNCTION(ip2proxy_get_domain)
{
	char *ip_address;
	size_t ip_len;
	IP2ProxyRecord *record = NULL;

	PHP_IP2PROXY_DB_CHECK;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ip_address, &ip_len) == FAILURE) {
		return;
	}
	record = IP2Proxy_get_domain(IP2PROXY_G(ip2proxy_ptr), ip_address);
#if PHP_MAJOR_VERSION >= 7
	RETVAL_STRING(record->domain);
#else
	RETVAL_STRING(record->domain, 1);
#endif
	IP2Proxy_free_record(record);
}
/* }}} */

/* {{{ ip2proxy_get_usage_type("ip_address")
 * Returns ip address's internet connection usage_type information */
PHP_FUNCTION(ip2proxy_get_usage_type)
{
	char *ip_address, *ret;
	size_t ip_len;
	IP2ProxyRecord *record = NULL;

	PHP_IP2PROXY_DB_CHECK;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ip_address, &ip_len) == FAILURE) {
		return;
	}
	record = IP2Proxy_get_usage_type(IP2PROXY_G(ip2proxy_ptr), ip_address);
	ret = record->usage_type;

#if PHP_MAJOR_VERSION >= 7
	RETVAL_STRING(ret);
#else
	RETVAL_STRING(ret, 1);
#endif
	IP2Proxy_free_record(record);
}
/* }}} */

/* {{{ ip2proxy_get_usage_type("ip_address")
 * Returns ip address's ASN information */
PHP_FUNCTION(ip2proxy_get_asn)
{
	char *ip_address, *ret;
	size_t ip_len;
	IP2ProxyRecord *record = NULL;

	PHP_IP2PROXY_DB_CHECK;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ip_address, &ip_len) == FAILURE) {
		return;
	}
	record = IP2Proxy_get_asn(IP2PROXY_G(ip2proxy_ptr), ip_address);
	ret = record->asn;

#if PHP_MAJOR_VERSION >= 7
	RETVAL_STRING(ret);
#else
	RETVAL_STRING(ret, 1);
#endif
	IP2Proxy_free_record(record);
}
/* }}} */

/* {{{ ip2proxy_get_as("ip_address")
 * Returns ip address's AS information */
PHP_FUNCTION(ip2proxy_get_as)
{
	char *ip_address, *ret;
	size_t ip_len;
	IP2ProxyRecord *record = NULL;

	PHP_IP2PROXY_DB_CHECK;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ip_address, &ip_len) == FAILURE) {
		return;
	}
	record = IP2Proxy_get_as(IP2PROXY_G(ip2proxy_ptr), ip_address);
	ret = record->as_;

#if PHP_MAJOR_VERSION >= 7
	RETVAL_STRING(ret);
#else
	RETVAL_STRING(ret, 1);
#endif
	IP2Proxy_free_record(record);
}
/* }}} */

/* {{{ ip2proxy_get_last_seen("ip_address")
 * Returns ip address's proxy last seen information */
PHP_FUNCTION(ip2proxy_get_last_seen)
{
	char *ip_address, *ret;
	size_t ip_len;
	IP2ProxyRecord *record = NULL;

	PHP_IP2PROXY_DB_CHECK;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ip_address, &ip_len) == FAILURE) {
		return;
	}
	record = IP2Proxy_get_last_seen(IP2PROXY_G(ip2proxy_ptr), ip_address);
	ret = record->last_seen;

#if PHP_MAJOR_VERSION >= 7
	RETVAL_STRING(ret);
#else
	RETVAL_STRING(ret, 1);
#endif
	IP2Proxy_free_record(record);
}
/* }}} */

/* {{{ ip2proxy_get_threat("ip_address")
 * Returns ip address's security threat information */
PHP_FUNCTION(ip2proxy_get_threat)
{
	char *ip_address, *ret;
	size_t ip_len;
	IP2ProxyRecord *record = NULL;

	PHP_IP2PROXY_DB_CHECK;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ip_address, &ip_len) == FAILURE) {
		return;
	}
	record = IP2Proxy_get_threat(IP2PROXY_G(ip2proxy_ptr), ip_address);
	ret = record->threat;

#if PHP_MAJOR_VERSION >= 7
	RETVAL_STRING(ret);
#else
	RETVAL_STRING(ret, 1);
#endif
	IP2Proxy_free_record(record);
}
/* }}} */

/* {{{ ip2proxy_get_provider("ip_address")
 * Returns ip address's VPN provider name */
PHP_FUNCTION(ip2proxy_get_provider)
{
	char *ip_address, *ret;
	size_t ip_len;
	IP2ProxyRecord *record = NULL;

	PHP_IP2PROXY_DB_CHECK;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ip_address, &ip_len) == FAILURE) {
		return;
	}
	record = IP2Proxy_get_provider(IP2PROXY_G(ip2proxy_ptr), ip_address);
	ret = record->provider;

#if PHP_MAJOR_VERSION >= 7
	RETVAL_STRING(ret);
#else
	RETVAL_STRING(ret, 1);
#endif
	IP2Proxy_free_record(record);
}
/* }}} */

/* {{{ ip2proxy_get_fraud_score("ip_address")
 * Returns ip address's fraud score */
PHP_FUNCTION(ip2proxy_get_fraud_score)
{
	char *ip_address, *ret;
	size_t ip_len;
	IP2ProxyRecord *record = NULL;

	PHP_IP2PROXY_DB_CHECK;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ip_address, &ip_len) == FAILURE) {
		return;
	}
	record = IP2Proxy_get_fraud_score(IP2PROXY_G(ip2proxy_ptr), ip_address);
	ret = record->provider;

#if PHP_MAJOR_VERSION >= 7
	RETVAL_STRING(ret);
#else
	RETVAL_STRING(ret, 1);
#endif
	IP2Proxy_free_record(record);
}
/* }}} */

/* {{{ ip2proxy_get_all("ip_address")
 *  * Returns the record information */
PHP_FUNCTION(ip2proxy_get_all)
{
	char *ip_address;
	size_t ip_len;
	IP2ProxyRecord *record = NULL;

	PHP_IP2PROXY_DB_CHECK;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ip_address, &ip_len) == FAILURE) {
		return;
	}
	record = IP2Proxy_get_all(IP2PROXY_G(ip2proxy_ptr), ip_address);
	array_init(return_value);
#if PHP_MAJOR_VERSION >= 7
	add_assoc_string(return_value, "country_short", record->country_short);
	add_assoc_string(return_value, "country_long", record->country_long);
	add_assoc_string(return_value, "region", record->region);
	add_assoc_string(return_value, "city", record->city);
	add_assoc_string(return_value, "isp", record->isp);
	add_assoc_string(return_value, "domain", record->domain);
	add_assoc_string(return_value, "usage_type", record->usage_type);
	add_assoc_string(return_value, "asn", record->asn);
	add_assoc_string(return_value, "as", record->as_);
	add_assoc_string(return_value, "last_seen", record->last_seen);
	add_assoc_string(return_value, "threat", record->threat);
	add_assoc_string(return_value, "provider", record->provider);
	add_assoc_string(return_value, "fraud_score", record->fraud_score);
#else
	add_assoc_string(return_value, "country_short", record->country_short, 1);
	add_assoc_string(return_value, "country_long", record->country_long, 1);
	add_assoc_string(return_value, "region", record->region, 1);
	add_assoc_string(return_value, "city", record->city, 1);
	add_assoc_string(return_value, "isp", record->isp, 1);
	add_assoc_string(return_value, "domain", record->domain, 1);
	add_assoc_string(return_value, "usage_type", record->usage_type, 1);
	add_assoc_string(return_value, "asn", record->asn, 1);
	add_assoc_string(return_value, "as", record->as_, 1);
	add_assoc_string(return_value, "last_seen", record->last_seen, 1);
	add_assoc_string(return_value, "threat", record->threat, 1);
	add_assoc_string(return_value, "provider", record->provider, 1);
	add_assoc_string(return_value, "fraud_score", record->fraud_score, 1);
#endif
	IP2Proxy_free_record(record);
}
/* }}} */


/* {{{ ip2proxy_close()
 * Returns the record information */
PHP_FUNCTION(ip2proxy_close)
{
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") != SUCCESS) {
		return;
	}
	if (IP2PROXY_G(ip2proxy_ptr)) {
		IP2Proxy_close(IP2PROXY_G(ip2proxy_ptr));
	}
	IP2PROXY_G(ip2proxy_ptr) = NULL;
}
/* }}} */

/* {{{ ip2proxy_delete_shm()
 * Returns nothing */
PHP_FUNCTION(ip2proxy_delete_shm)
{
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") != SUCCESS) {
		return;
	}

	IP2Proxy_delete_shm();
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
