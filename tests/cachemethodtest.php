<?php
        /*Test DB access using cache mathod*/
        ip2proxy_open('PX2.BIN');
        ip2proxy_open_mem(IP2PROXY_CACHE_MEMORY);
        echo ip2proxy_get_country_short("8.8.8.8");
        echo "\n";
        echo ip2proxy_get_country_long("8.8.8.8");
        echo "\n";
        echo ip2proxy_get_region("8.8.8.8");
        echo "\n";
        echo ip2proxy_get_city("8.8.8.8");
        echo "\n";
        echo ip2proxy_get_isp("8.8.8.8");
        echo "\n";
        echo ip2proxy_get_domain("8.8.8.8");
        echo "\n";
        echo ip2proxy_get_usage_type("8.8.8.8");
        echo "\n";
		echo ip2proxy_get_asn("8.8.8.8");
        echo "\n";
		echo ip2proxy_get_as("8.8.8.8");
        echo "\n";
		echo ip2proxy_get_last_seen("8.8.8.8");
        echo "\n";
		echo ip2proxy_get_threat("8.8.8.8");
        echo "\n";
        echo ip2proxy_close();
?>

