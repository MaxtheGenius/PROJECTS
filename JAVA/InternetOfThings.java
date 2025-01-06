// Import necessary libraries for data collection, threading, and analysis
import java.util.*;
import java.util.concurrent.*;
import java.util.stream.Collectors;

/*
 * SmartHomeSensors
 * A Java application that simulates real-time data collection and analysis from
 * temperature and humidity sensors distributed across a smart home.
 */
public class SmartHomeSensors {

    // SensorData class to store readings from temperature and humidity sensors
    static class SensorData {
        private final String sensorId; // Unique ID of the sensor
        private final double temperature; // Temperature reading
        private final double humidity; // Humidity reading

        public SensorData(String sensorId, double temperature, double humidity) {
            this.sensorId = sensorId;
            this.temperature = temperature;
            this.humidity = humidity;
        }

        public String getSensorId() {
            return sensorId;
        }

        public double getTemperature() {
            return temperature;
        }

        public double getHumidity() {
            return humidity;
        }

        @Override
        public String toString() {
            return "Sensor ID: " + sensorId +
                    " | Temperature: " + temperature + "°C" +
                    " | Humidity: " + humidity + "%";
        }
    }

    // Method to simulate data collection from a sensor
    private static SensorData collectData(String sensorId) {
        Random random = new Random();
        double temperature = 15 + random.nextDouble() * 10; // Random temperature between 15°C and 25°C
        double humidity = 30 + random.nextDouble() * 50; // Random humidity between 30% and 80%
        return new SensorData(sensorId, temperature, humidity);
    }

    // Main method - Entry point for the application
    public static void main(String[] args) {
        // Define the sensors in the smart home
        List<String> sensorIds = Arrays.asList("LivingRoom", "Bedroom", "Kitchen", "Bathroom", "Garage");

        // Executor to simulate real-time data collection
        ScheduledExecutorService executor = Executors.newScheduledThreadPool(sensorIds.size());
        List<SensorData> dataBuffer = Collections.synchronizedList(new ArrayList<>()); // Thread-safe buffer for
                                                                                       // collected data

        // Schedule periodic data collection for each sensor
        for (String sensorId : sensorIds) {
            executor.scheduleAtFixedRate(() -> {
                SensorData data = collectData(sensorId); // Collect data from the sensor
                synchronized (dataBuffer) {
                    dataBuffer.add(data); // Add collected data to the buffer
                }
                System.out.println("Collected Data: " + data); // Display collected data
            }, 0, 2, TimeUnit.SECONDS); // Collect data every 2 seconds
        }

        // Analysis task to compute statistics every 10 seconds
        executor.scheduleAtFixedRate(() -> {
            synchronized (dataBuffer) {
                if (!dataBuffer.isEmpty()) {
                    // Calculate statistics
                    double avgTemperature = dataBuffer.stream().mapToDouble(SensorData::getTemperature).average()
                            .orElse(0.0);
                    double avgHumidity = dataBuffer.stream().mapToDouble(SensorData::getHumidity).average().orElse(0.0);
                    double maxTemperature = dataBuffer.stream().mapToDouble(SensorData::getTemperature).max()
                            .orElse(0.0);
                    double minTemperature = dataBuffer.stream().mapToDouble(SensorData::getTemperature).min()
                            .orElse(0.0);

                    System.out.println("\n=== Data Analysis ===");
                    System.out.println("Average Temperature: " + avgTemperature + "°C");
                    System.out.println("Average Humidity: " + avgHumidity + "%");
                    System.out.println("Max Temperature: " + maxTemperature + "°C");
                    System.out.println("Min Temperature: " + minTemperature + "°C");
                    System.out.println("=====================\n");

                    dataBuffer.clear(); // Clear the buffer for the next cycle
                }
            }
        }, 10, 10, TimeUnit.SECONDS); // Analyze data every 10 seconds

        // Stop the simulation after 1 minute
        executor.schedule(() -> {
            executor.shutdown();
            System.out.println("Simulation Ended.");
        }, 1, TimeUnit.MINUTES); // Stop after 1 minute
    }
}