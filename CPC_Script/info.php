
<?php

	/*
		Check if serial number(key) is used or not.
	*/
	
	if (!isset($_GET['serial']) || empty($_GET['serial']))
		exit ("INPUT_ERROR");
		
	require("config.php");
	
	$serial = $_GET['serial'];
	$serial = trim($serial);
	$serial = mysql_real_escape_string($serial);
	$serial = strip_tags($serial);
	
	if (strlen($serial) > 25)
		exit ("BAD_ENTREY");
	
	// search in generated table
	$query="SELECT * FROM GeneratedTable";
	$result=mysql_query($query);
	$foundInGeneratedTable = False;
	
	while ($row = mysql_fetch_array($result) ) {
		$tmpSerial =  $row['serial'] ;
		
		if ( $tmpSerial == $serial )
			$foundInGeneratedTable =  True;
	}
	
	// search in used table
	$query2="SELECT * FROM UsedLicenceTable";
	$result2=mysql_query($query2);
	$foundInUsedTable = False;
	
	while ($row2 = mysql_fetch_array($result2) ) {
		$tmpSerial2 =  $row2['serial'] ;
		
		if ( $tmpSerial2 == $serial )
			$foundInUsedTable =  True;
	}
	
	if ( $foundInGeneratedTable == True && $foundInUsedTable == True)
		echo "Used" ;
	else if ( $foundInGeneratedTable == True && $foundInUsedTable == False )
		echo "OK"; 
	else if ( $foundInGeneratedTable == False && $foundInUsedTable == True ) 
		echo "NO" ;
	else if ( $foundInGeneratedTable == False && $foundInUsedTable == False )
		echo "NO";
		
	mysql_close();
?>