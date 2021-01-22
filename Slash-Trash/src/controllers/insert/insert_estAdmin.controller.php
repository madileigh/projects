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
        $EAName = filter_input(INPUT_POST, 'EAName');
        $Est_Id = filter_input(INPUT_POST, 'Est_Id');
        $EAEmail = filter_input(INPUT_POST, 'EAEmail');
        $Phone = filter_input(INPUT_POST, 'Phone');

        if($query = "INSERT INTO ESTABLISHMENT_ADMIN(Admin_Id, EAName, Phone, EAEmail, Est_Id) VALUES (:Admin_Id, :EAName, :Phone, :EAEmail, :Est_Id)")
        {
            if($query==null)
            {
                echo "Unable to insert due to violation.";
                die();
            }
            else
            {
                $stmt = $conn->prepare($query);
                $stmt->execute(array(':Admin_Id' => $Admin_Id, ':EAName' => $EAName, ':Phone' => $Phone, ':EAEmail' => $EAEmail, ':Est_Id' => $Est_Id));
                $rows = $stmt->fetchALL(PDO::FETCH_ASSOC);
                if($stmt)
                    echo 'New record inserted successfully.';
                else
                {
                    echo "Unable to insert due to violation. Please check your input and the table.";
                }
            }
        }
    }
    echo '<p><a href="javascript:history.go(-1)" title="return">&laquo; Return to Slash-Trash Homepage</a></p>';    
?>
