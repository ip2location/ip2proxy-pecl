--TEST--
DB access using file method
--SKIPIF--
<?php
if(!extension_loaded('ip2proxy')) die('skip missing extension');
?>
--FILE--
<?php
/*Test DB access using file mathod*/
var_dump(ip2proxy_open(__DIR__ . '/PX12.SAMPLE.BIN'));
var_dump(ip2proxy_open_mem(IP2PROXY_CACHE_MEMORY));
var_dump(ip2proxy_get_country_short('23.83.130.186'));
var_dump(ip2proxy_get_country_long('23.83.130.186'));
var_dump(ip2proxy_close());
?>
Done
--EXPECTF--
bool(true)
bool(true)
string(2) "US"
string(24) "United States of America"
NULL
Done
