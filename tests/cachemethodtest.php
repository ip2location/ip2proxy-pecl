<?php
        /*Test DB access using cache method*/
        ip2proxy_open('PX12.SAMPLE.BIN');
        ip2proxy_open_mem(IP2PROXY_CACHE_MEMORY);
        echo ip2proxy_get_country_short("23.83.130.186");
        echo "\n";
        echo ip2proxy_get_country_long("23.83.130.186");
        echo "\n";
        echo ip2proxy_get_region("23.83.130.186");
        echo "\n";
        echo ip2proxy_get_city("23.83.130.186");
        echo "\n";
        echo ip2proxy_get_isp("23.83.130.186");
        echo "\n";
        echo ip2proxy_get_domain("23.83.130.186");
        echo "\n";
        echo ip2proxy_get_usage_type("23.83.130.186");
        echo "\n";
		echo ip2proxy_get_asn("23.83.130.186");
        echo "\n";
		echo ip2proxy_get_as("23.83.130.186");
        echo "\n";
		echo ip2proxy_get_last_seen("23.83.130.186");
        echo "\n";
		echo ip2proxy_get_threat("23.83.130.186");
        echo "\n";
		echo ip2proxy_get_provider("23.83.130.186");
        echo "\n";
		echo ip2proxy_get_fraud_score("23.83.130.186");
        echo "\n";
        echo ip2proxy_close();
?>

