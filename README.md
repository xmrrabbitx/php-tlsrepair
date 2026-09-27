# PHP TLSRepair

A PHP extension that exposes the native **TLSRepair** C library to PHP.

It integrates with PHP's built-in cURL extension and can repair incomplete TLS certificate chains before performing a request.

## Requirements

* PHP 8.5+
* cURL extension
* [TLSRepair C library](https://github.com/xmrrabbitx/tlsrepair)

## API

### `TLSRepair::fromCurl()`

Create a `TLSRepair` object from an existing PHP cURL handle.

### `setProxy()`

Optionally configure a proxy for certificate-chain discovery.

### `prepare()`

Inspect the server's certificate chain, retrieve missing intermediate certificates, and configure the cURL handle for the final request.

### `getError()`

Return the last TLSRepair error, or `null` if there is no error.

## Example

```php
<?php

$ch = curl_init();

curl_setopt(
    $ch,
    CURLOPT_URL,
    'https://incomplete-chain.badssl.com'
);

$repair = TLSRepair::fromCurl($ch);

$repair->setProxy(
    'http://127.0.0.1:8080'
);

if (!$repair->prepare()) {
    die($repair->getError());
}

curl_exec($ch);

curl_close($ch);
```

## Build

Build the extension using the standard PHP extension build process:

```bash
git clone https://github.com/xmrrabbitx/php-tlsrepair.git
cd php-tlsrepair
../bin/phpize
./configure --with-php-config=$HOME/myphp/bin/php-config
make -jn 
sudo make install
```
N is the maximum number of jobs (parallel build tasks) that make may run simultaneously.

Then enable it in `php.ini`:

```ini
extension=php_tlsrepair.so
```

Check that it is loaded:

```bash
php -m | grep php_tlsrepair
```
