<?php
	/*
		Add serial number to database.
	*/
	
	if (!isset($_GET['serial']) || empty($_GET['serial']))
		exit ("INPUT_ERROR");

	require("config.php");
								
	$serial = $_GET['serial'];
	$serial = trim($serial);
	$serial = mysql_real_escape_string($serial);
	$serial = strip_tags($serial);
	
	$ip = "";

	if (strlen($serial) > 25)
		exit ("BAD_ENTREY");
	
	
	mysql_query("INSERT INTO UsedLicenceTable (serial, usedTime, ip) VALUES ('$serial',CURDATE(),'$ip')")
	or die(mysql_error());  
	
	echo "DONE";
	mysql_close();
?>