# IP2Proxy PECL Extension
This PECL extension allows you to  detect proxy servers with country, region, city, ISP, domain, usage type, ASN, security threat, proxy type and provider information using IP2Proxy binary database.

To compile it, you will need to have the IP2Proxy C library installed.



# Installation

### Install IP2Proxy C Library (on Linux/Unix)
1. Download the latest IP2Proxy C libray  from https://github.com/ip2location/ip2proxy-c

2. Decompress the archive.

3. Navigate to working directory and start the compilation.

   ```bash
   cd ip2proxy-c-master
   autoreconf -i -v --force
   ./configure
   make
   make install
   ```


### Install PECL extension (on Linux/Unix)
1. Decompress the archive.

2. Navigate to working directory. Run the following command:

   ```bash
   cd ip2proxy-pecl-master
   phpize
   ./configure
   make
   make install
   ```



3. A successful install will create **modules/ip2proxy.so** and put it into the PHP extensions directory.

4. Open the php.ini file, and add an extension=ip2proxy.so line.

5. Restart apache server.

6. php -m (You should notice the ip2proxy module in the list)



For details installation instructions for PHP PECL extensions, please see:
https://www.php.net/manual/en/install.pecl.php



### Install Using PIE (PHP Installer for Extensions)

```
pie install ip2location/ip2proxy-pie
```



# Sample BIN Databases

* Download free IP2Proxy LITE databases at [https://lite.ip2location.com](https://lite.ip2location.com/ip2proxy-lite)

* Download commerce IP2Proxy databases at [https://www.ip2location.com](https://www.ip2location.com/database/ip2proxy)



# IPv4 BIN vs IPv6 BIN
* Use the IPv4 BIN file if you just need to query IPv4 addresses.
* Use the IPv6 BIN file if you need to query BOTH IPv4 and IPv6 addresses.

# Support
Email: support@ip2location.com.
URL: [http://www.ip2location.com](http://www.ip2location.com)
