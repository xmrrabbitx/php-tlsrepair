--TEST--
Check if tlsrepair is loaded
--EXTENSIONS--
tlsrepair
--FILE--
<?php
echo 'The extension "tlsrepair" is available';
?>
--EXPECT--
The extension "tlsrepair" is available
