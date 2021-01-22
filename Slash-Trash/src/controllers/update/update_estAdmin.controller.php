<?php
    require_once '../../utils/connect_database.php';

    // Check connection
    if(!$conn) 
    {  
        die("Connection failed");
    }
    else 
    {
        $Admin_Id = filter_input(INPUT_POST, 'Admin_Id');
        $Phone = filter_input(INPUT_POST, 'Phone');
        $EAEmail = filter_input(INPUT_POST, 'EAEmail');

        if($query = "UPDATE ESTABLISHMENT_ADMIN SET Phone = :Phone, EAEmail = :EAEmail WHERE Admin_Id = :Admin_Id")
        {    
            if($query==null)
            {      
                echo "Unable to update due to violation.";      
                die();    
            }    
            else
            {
                $stmt = $conn->prepare($query);  
                $stmt->execute(array(':Admin_Id' => $Admin_Id, ':Phone' => $Phone, ':EAEmail' => $EAEmail));
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