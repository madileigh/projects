<?php
    $host = "localhost:3306";
	$dbusername = "root";
    $dbpassword = "";
    
	try {
        $conn = new PDO('mysql:host='.$host, $dbusername, $dbpassword);
        $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
		$sql = "CREATE DATABASE SLASHTRASH";
		// use exec() because no results are returned
		$conn->exec("DROP DATABASE IF EXISTS SLASHTRASH");	
		$conn->exec($sql);
		echo "Database created successfully<br>";

	} 
	catch(PDOException $e) 
	{
		echo $sql . "<br>" . $e->getMessage();
	}

	$conn = null;

    //create tables:
    
    $establishment = "CREATE TABLE 
		ESTABLISHMENT (
		Est_Id VARCHAR(7) NOT NULL, 
		EName VARCHAR(50) NOT NULL, 
		Est_Address VARCHAR(80) NOT NULL,
		Waste_Pts INT, 
		Type VARCHAR(30),
		PRIMARY KEY(Est_Id));";

	$establishment_admin = "CREATE TABLE 
		ESTABLISHMENT_ADMIN (
		Admin_Id VARCHAR(7) NOT NULL, 
		EAName VARCHAR(50) NOT NULL,
		Phone VARCHAR(20),
		EAEmail VARCHAR(50), 
		Est_Id VARCHAR(7) NOT NULL, 
		PRIMARY KEY(Admin_Id, Est_Id),
		FOREIGN KEY (Est_Id) REFERENCES ESTABLISHMENT(Est_Id) );";

	$customer = "CREATE TABLE 
		CUSTOMER (Cust_Id VARCHAR(7) NOT NULL, 
		CName VARCHAR(50) NOT NULL, 
		Cust_Pts INT, 
		CEmail VARCHAR(50) NOT NULL,
		Age INT,
		PRIMARY KEY(Cust_Id));";

	$visits = "CREATE TABLE 
		VISITS (Est_Id VARCHAR(7) NOT NULL, 
		Cust_Id VARCHAR(7) NOT NULL, 
		PRIMARY KEY (Est_Id, Cust_Id), 
		FOREIGN KEY (Est_Id) REFERENCES ESTABLISHMENT(Est_Id), 
		FOREIGN KEY (Cust_Id) REFERENCES CUSTOMER(Cust_Id));";

	$item = "CREATE TABLE 
		REUSABLE_ITEM (
		Item_Id VARCHAR(9) NOT NULL, 
		Category VARCHAR(20),
		Pt_Val INT, 
		IName VARCHAR(50) NOT NULL,
		Cust_Id VARCHAR(7) NOT NULL, 
		PRIMARY KEY(Item_Id, Cust_Id),
		FOREIGN KEY (Cust_Id) REFERENCES CUSTOMER(Cust_Id) );";

	$order = "CREATE TABLE 
		ORDERS (
		Order_Id VARCHAR(10) NOT NULL,
		Pts INT,
		Trans_Date DATE NOT NULL,
		Cust_Id VARCHAR(7) NOT NULL,
		Item_Id VARCHAR(9), 
		Est_Id VARCHAR(7) NOT NULL,
		PRIMARY KEY(Order_Id, Est_Id, Cust_Id),
		FOREIGN KEY (Est_Id) REFERENCES ESTABLISHMENT(Est_Id),
		FOREIGN KEY (Cust_Id) REFERENCES CUSTOMER(Cust_Id) );";

    $sql = $establishment;
    $sql .= $establishment_admin;
    $sql .= $customer;
    $sql .= $visits;
    $sql .= $item;
    $sql .= $order;
	//echo $sql;
	
	$dbname = "SLASHTRASH";

	$conn = new PDO('mysql:host='.$host.';dbname='.$dbname, $dbusername, $dbpassword);
	// check connection
	if(!$conn)
	{
		die("Connection failed");
	}
	// connection worked
	else
	{
		if ($conn->query($sql) == TRUE) 
		{
  			echo "Table SLASHTRASH created successfully";
		} 
		else 
		{
  		echo "Error creating table: " . $conn->error;
		}
	}

	//Fill the tables
	$filenames = array
		(
			"Customer",
			"Establishment",
			"Establishment_Admin",
			"Reusable_Item",
			"visits",
			"Orders"
		);
	$num_filenames = count($filenames);
	$final_query = "";

	function validateDate($date, $format = 'Y-m-d')
	{
	    $d = DateTime::createFromFormat($format, $date);
	    // The Y ( 4 digits year ) returns TRUE for any integer with any number of digits so changing the comparison from == to === fixes the issue.
	    return $d && $d->format($format) === $date;
	}

	for($i=0; $i < $num_filenames; $i++)
	{
		$firstline = TRUE;

		if (($fileread = fopen("../../Data/".$filenames[$i].".csv", "r")) == TRUE)
		{
			while (($data = fgetcsv($fileread, 1000, ",")) == TRUE) 
			{
				if($firstline == TRUE)
				{
					$firstline = FALSE;
					continue;
				}

				$num = count($data);
				$values = "";
				for ($c=0; $c < $num; $c++) 
				{
					if(is_numeric($data[$c])    /*|validateDate($data[$c])*/   )
					{
						$values .= $data[$c];
					}
					else
					{
						$values .= "\"".$data[$c]."\"";
					}
					if($c<($num-1))
					{
						$values .= ", ";
					}
				}

				$insert_query = "INSERT INTO ".strtoupper($filenames[$i])." VALUES (".$values.");";
				$final_query .= $insert_query;
				echo $insert_query."<br>";
			}
			fclose($fileread);
		}
	}

	if ($conn->query($final_query) == TRUE) {
  		echo "<br> Tables are filled";
	} else {
  		echo "Error: " . $final_query . "<br>" . $conn->error;
    }
    
    $custom_view = "CREATE VIEW TRANSACTION_INFO(Cust_Id, CName, Cust_Pts, Est_Id, EName, Order_Id, Pts, Trans_Date, Item_Id, IName)
        AS SELECT   C.Cust_Id, C.CName, C.Cust_Pts, E.Est_Id, E.EName, O.Order_Id, O.Pts, O.Trans_Date, I.Item_Id, I.IName
        FROM CUSTOMER AS C, ESTABLISHMENT AS E, ORDERS AS O, REUSABLE_ITEM AS I
        WHERE C.Cust_Id = O.Cust_Id AND I.Item_Id = O.Item_Id AND E.Est_Id = O.Est_Id;";

    if ($conn->query($custom_view) == TRUE) {
        echo "<br> TRANSACTION_INFO VIEW is successfully created.";
    } else {
        echo "Error: " . $final_query . "<br>" . $conn->error;
    }

	$conn = NULL;
?>