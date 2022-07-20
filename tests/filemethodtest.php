
<?php
        /*Test DB access using file mathod*/
        ip2proxy_open('PX11.SAMPLE.BIN');
        ip2proxy_open_mem(IP2PROXY_FILE_IO);
        echo ip2proxy_get_country_short("1.0.0.8");
        echo "\n";
        echo ip2proxy_get_country_long("1.0.0.8");
        echo "\n";
        echo ip2proxy_get_region("1.0.0.8");
        echo "\n";
        echo ip2proxy_get_city("1.0.0.8");
        echo "\n";
        echo ip2proxy_get_isp("1.0.0.8");
        echo "\n";
        echo ip2proxy_get_domain("1.0.0.8");
        echo "\n";
        echo ip2proxy_get_usage_type("1.0.0.8");
        echo "\n";
		echo ip2proxy_get_asn("1.0.0.8");
        echo "\n";
		echo ip2proxy_get_as("1.0.0.8");
        echo "\n";
		echo ip2proxy_get_last_seen("1.0.0.8");
        echo "\n";
		echo ip2proxy_get_threat("1.0.0.8");
        echo "\n";
		echo ip2proxy_get_provider("1.0.0.8");
        echo "\n";
        echo ip2proxy_close();
?>

