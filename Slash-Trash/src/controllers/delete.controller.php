<?php
    require_once '../utils/connect_database.php';

    // Check connection
    if(!$conn) 
    {  
        die("Connection failed");
    }
    else 
    {
        $table = filter_input(INPUT_POST, 'table');
        $input_id = filter_input(INPUT_POST, 'input_id');
        $table_id = "";
        $table = strtoupper($table);

        switch ($table) 
        {
            case 'CUSTOMER':
                $table_id = "Cust_Id";
                break;
            case 'ESTABLISHMENT_ADMIN':
                $table_id = "Admin_Id";
                break;
            case 'ESTABLISHMENT':
                $table_id = "Est_Id";
                break;   
            case 'ORDERS':
                $table_id = "Order_Id";
                break;
            case 'REUSABLE_ITEM':
                $table_id = "Item_Id";
                break;
        }

        $query = "DELETE FROM $table
                  WHERE $table_id = '$input_id'";
            
        echo "<br>".$query."<br>";

        if($conn->query($query))
        {
            echo "Deleted successfully";
        }
        else
        {
            echo "Error:".$query."
            ".$conn->error;
        }
    }
    echo '<p><a href="javascript:history.go(-1)" title="return">&laquo; Return to Slash-Trash Homepage</a></p>';
?>