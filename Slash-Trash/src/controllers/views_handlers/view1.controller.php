<?php
    require_once '../../utils/connect_database.php';

    // Check connection
    if(!$conn) 
    {  
        die("Connection failed");
    }
    else 
    {
        $cust_id = filter_input(INPUT_POST, 'Cust_Id');
        $start = filter_input(INPUT_POST, 'from_date');
        $end = filter_input(INPUT_POST, 'to_date');
        
        $query = "SELECT DISTINCT Cust_Id, CName, SUM(Pts)
                FROM TRANSACTION_INFO
                WHERE Cust_Id = \"".$cust_id."\" AND Trans_Date >= \"".$start."\" AND Trans_Date <= \"".$end."\"
                GROUP BY Cust_Id;";
        $result = $conn->query($query);
        if ($result)
        {
            $data = $result->fetch(PDO::FETCH_NUM);
            if($data)
            {
                echo "<br /><b>";
                echo "Customer, ".$data[1]."(".$data[0].") has gained ".$data[2].
                " points between ".$start." and ".$end;
                echo "</b>";
            }
            else
            {
                echo "<Br /><b>";
                echo "Customer with ID ".$cust_id." made no transaction between ".$start." and ".$end;
                echo "</b>";
            }
        }
        else
        {
            echo "Error:".$query."
            ".$conn->error;
        }
    }
    echo '<p><a href="javascript:history.go(-1)" title="return">&laquo; Return to Slash-Trash Homepage</a></p>';
?>
