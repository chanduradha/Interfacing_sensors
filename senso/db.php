<?php
function connectToDatabase($host, $username, $password, $database) {
    $connection = mysqli_connect($host, $username, $password, $database);
    if (mysqli_connect_errno()) {
        die("Failed to connect to MySQL: " . mysqli_connect_error());
    }
    return $connection;
}

$connection = connectToDatabase('185.214.124.4', 'u759114105_Laki', 'Chandu123', 'u759114105_Sensortable');

if (isset($_GET['sensor_column'])) {
    // Sanitize the input to prevent SQL injection
    $sensor_column = mysqli_real_escape_string($connection, $_GET['sensor_column']);

    // Make sure the sensor_column is one of the valid columns
    $valid_columns = array('temperature', 'humidity', 'distance', 'ldr_value', 'flame_value', 'pir_value', 'soilMoisture_value', 'rain_value', 'heartbeat_value');
    if (!in_array($sensor_column, $valid_columns)) {
        die("Invalid sensor column.");
    }

    // Check if a timestamp parameter is provided
    if (isset($_GET['timestamp'])) {
        // Sanitize the timestamp parameter (you should further validate the timestamp format to prevent SQL injection)
        $timestamp = mysqli_real_escape_string($connection, $_GET['timestamp']);

        // Assuming the 'timestamp' column in your database stores the timestamp value
        // You can modify this query to retrieve data based on the timestamp
        $query = "SELECT id, $sensor_column FROM Sensor_table WHERE timestamp_column >= '$timestamp' ORDER BY id DESC";
    } else {
        // No timestamp provided, fetch all data for the specified sensor column
        $query = "SELECT id, $sensor_column FROM Sensor_table ORDER BY id DESC";
    }

    $result = mysqli_query($connection, $query);

    $data = array();
    while ($row = mysqli_fetch_assoc($result)) {
        $data[] = array(
            'id' => $row['id'],
            $sensor_column => $row[$sensor_column]
        );
    }

    echo json_encode($data);
}
?>
