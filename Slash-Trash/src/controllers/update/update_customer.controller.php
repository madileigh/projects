<?php
    require_once '../../utils/connect_database.php';

    // Check connection
    if(!$conn) 
    {  
        die("Connection failed");
    }
    else 
    {
        $Cust_Id = filter_input(INPUT_POST, 'Cust_Id');
        $Cust_Pts = filter_input(INPUT_POST, 'Cust_Pts');

        if($query = "UPDATE CUSTOMER SET Cust_Pts = :Cust_Pts WHERE Cust_Id = :Cust_Id")
        {    
            if($query==null)
            {      
                echo "Unable to update due to violation.";      
                die();    
            }    
            else
            {
                $stmt = $conn->prepare($query);  
                $stmt->execute(array(':Cust_Id' => $Cust_Id, ':Cust_Pts' => $Cust_Pts));
                $rows = $stmt->fetchALL(PDO::FETCH_ASSOC);
                if($stmt)
                    echo 'Record updated successfully.';
                else
                {
                    echo "Unable to update due to violation. Please check your input and the table."; 
                }
            }
        }
    }
    echo '<p><a href="javascript:history.go(-1)" title="return">&laquo; Return to Slash-Trash Homepage</a></p>';    
?>