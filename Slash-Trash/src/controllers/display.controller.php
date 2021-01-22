<?php
    require_once '../utils/connect_database.php';

    // Check connection
    if(!$conn) 
    {  
        die("Connection failed");
    }
    else 
    {
        $table_name = filter_input(INPUT_POST, 'table');
        $table_name = strtoupper($table_name);
        $rows = array();

        echo "<br>".$table_name."<br>";
        switch ($table_name)
        {
            case "CUSTOMER":
                $rows = array(
                    0 => "Cust_Id",
                    1 => "CName",
                    2 => "Cust_Pts",
                    3 => "CEmail",
                    4 => "Age"
                );
                break;
            case 'ESTABLISHMENT_ADMIN':
                $rows = array(
                    0 => "Admin_Id",
                    1 => "EAName",
                    2 => "Phone",
                    3 => "EAEmail",
                    4 => "Est_Id"
                );
                break;  
            case 'ESTABLISHMENT':
                $rows = array(
                    0 => "Est_Id",
                    1 => "EName",
                    2 => "Est_Address",
                    3 => "Waste_Pts",
                    4 => "Type"
                );
                break; 
            case 'ORDERS':
                $rows = array(
                    0 => "Order_Id",
                    1 => "Pts",
                    2 => "Trans_Date",
                    3 => "Cust_Id",
                    4 => "Item_Id",
                    5 => "Est_Id"
                );
                break;
            case 'REUSABLE_ITEM':
                $rows = array(
                    0 => "Item_Id",
                    1 => "Category",
                    2 => "Pt_Val",
                    3 => "IName",
                    4 => "Cust_Id"
                );
                break;
            case 'VISITS':
                $rows = array(
                    0 => "Est_Id",
                    1 => "Cust_Id"
                );
                break;
        }
        $stmt = $conn->query("SELECT * FROM $table_name");
        if ($stmt)
        {$data = $stmt->fetchAll();
        if($data)
        {
            echo '<table border="1">';
            echo '<tr>';
            // attribute/column names
            foreach ($rows as $rowName)
            {
                echo '<td>' . $rowName . '</td>';
            }
            // table data / records / tuples
            foreach ($data as $row) 
            {
                echo '<tr>';
                foreach ($rows as $elem)
                {
                    echo '<td>' . $row[$elem] . '</td>';
                }
                echo '</tr>';
            }
            echo '</table>';
        }
        else
        {
            echo "No Records Available";
            die(); 
        }}
    }    
    echo '<p><a href="javascript:history.go(-1)" title="return">&laquo; Return to Slash-Trash Homepage</a></p>';
?>