<?php
function connectToDatabase($host, $username, $password, $database) {
    $connection = mysqli_connect($host, $username, $password, $database);
    if (mysqli_connect_errno()) {
        die("Failed to connect to MySQL: " . mysqli_connect_error());
    }
    return $connection;
}

$connection = connectToDatabase('185.214.124.4', 'u759114105_Laki', 'Chandu123', 'u759114105_Sensortable');

if (isset($_GET['sensor_columns']) && isset($_GET['start_date']) && isset($_GET['end_date'])) {
    // Sanitize the input to prevent SQL injection
    $sensor_columns = $_GET['sensor_columns'];
    $start_date = mysqli_real_escape_string($connection, $_GET['start_date']);
    $end_date = mysqli_real_escape_string($connection, $_GET['end_date']);

    // Convert sensor_columns into an array
    $sensor_columns_array = explode(',', $sensor_columns);

    // Make sure all the sensor_columns are valid
    $valid_columns = array('id', 'temperature', 'humidity', 'distance', 'ldr_value', 'flame_value', 'pir_value', 'soilMoisture_value', 'rain_value', 'heartbeat_value', 'timestamp_column');
    foreach ($sensor_columns_array as $column) {
        if (!in_array($column, $valid_columns)) {
            die("Invalid sensor column.");
        }
    }

    // Construct the SELECT clause dynamically for the query
    $select_columns = implode(',', $sensor_columns_array);

    // Assuming the 'timestamp_column' column in your database stores the timestamp value
    $query = "SELECT $select_columns FROM Sensor_table WHERE timestamp_column BETWEEN '$start_date' AND '$end_date' ORDER BY id ASC";

    $result = mysqli_query($connection, $query);

    $data = array();
    while ($row = mysqli_fetch_assoc($result)) {
        $item = array();
        foreach ($sensor_columns_array as $column) {
            $item[$column] = $row[$column];
        }
        $data[] = $item;
    }

    echo json_encode($data);
}
?>
