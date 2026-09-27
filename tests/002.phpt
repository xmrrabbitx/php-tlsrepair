--TEST--
test1() Basic test
--EXTENSIONS--
tlsrepair
--FILE--
<?php
$ret = test1();

var_dump($ret);
?>
--EXPECT--
The extension tlsrepair is loaded and working!
NULL
