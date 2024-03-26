document.addEventListener('DOMContentLoaded', function() {
    const sensorChart = document.getElementById('sensorChart');
    let lineChart;
    let sensorColumn = 'temperature'; // Default sensor column
    let latestSensorData = []; // Variable to store the latest sensor data
    let sensorId; // Default sensorId

    function fetchSensorData() {
        let url = `db.php?sensor_column=${sensorColumn}`;

        fetch(url)
            .then(response => response.json())
            .then(data => {
                if (data.length > 0) {
                    // Get the latest sensor ID from the fetched data
                    sensorId = data[data.length - 1].id;

                    // Now, fetch data using the latest sensor ID
                    url = `db.php?sensor_column=${sensorColumn}&sensor_id=${sensorId}`;

                    return fetch(url);
                } else {
                    console.error('No data available.');
                }
            })
            .then(response => response.json())
            .then(data => {
                if (data.length > 0) {
                    // Update the latestSensorData with the fetched data
                    latestSensorData = data;

                    // Render the chart with the updated data
                    renderSensorChart(sensorColumn, data);
                }
            })
            .catch(error => {
                console.error('Error fetching data:', error);
            });
    }


    function renderSensorChart(sensorColumn, data) {
        // Destroy the previous chart if it exists
        if (lineChart) {
            lineChart.destroy();
        }

        // Use the 'id' column values for the x-axis (h-axis)
        const xValues = data.map(entry => entry.id);

        // Use the selected sensor column data for the y-axis (v-axis)
        const yValues = data.map(entry => entry[sensorColumn]);

        lineChart = new Chart(sensorChart, {
            type: 'line',
            data: {
                labels: xValues, // X-axis labels
                datasets: [{
                    label: sensorColumn,
                    data: yValues, // Y-axis values
                    borderColor: 'rgba(75, 192, 192, 1)',
                    fill: false
                }]
            },
            options: {
                responsive: true,
                maintainAspectRatio: false,
                scales: {
                    x: {
                        display: true,
                        title: {
                            display: true,
                            text: 'ID Column Value'
                        }
                    },
                    y: {
                        display: true,
                        title: {
                            display: true,
                            text: sensorColumn
                        }
                    }
                }
            }
        });
    }

    // Event listener for radio buttons
    const radioButtons = document.querySelectorAll('input[name="sensor"]');
    radioButtons.forEach(button => {
        button.addEventListener('change', function() {
            if (this.checked) {
                sensorColumn = this.value;
                fetchSensorData();
            }
        });
    });

    // Fetch and render data on page load
    fetchSensorData();

    // Fetch live data every 3 seconds
    setInterval(() => {
        fetchSensorData();
    }, 3000);

    // Default to temperature column, change to the desired default column if needed

});

window.addEventListener('resize', function() {
    if (lineChart) {
        lineChart.resize();
    }
});
