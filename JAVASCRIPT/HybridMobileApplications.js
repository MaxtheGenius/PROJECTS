import React, { useState, useEffect } from "react";
import { View, Text, StyleSheet, Button, FlatList, Alert } from "react-native";
import { Accelerometer } from "expo-sensors";
import AsyncStorage from "@react-native-async-storage/async-storage";
import axios from "axios";

// Mock backend API URL
const API_URL = "https://example.com/api/fitness"; // Replace with actual API

// Main Application Component
export default function App() {
  const [stepCount, setStepCount] = useState(0);
  const [isTracking, setIsTracking] = useState(false);
  const [accelerometerData, setAccelerometerData] = useState({});
  const [activityLogs, setActivityLogs] = useState([]);

  // Accelerometer subscription
  useEffect(() => {
    let subscription;
    if (isTracking) {
      subscription = Accelerometer.addListener((data) => {
        setAccelerometerData(data);
        if (isSignificantMotion(data)) {
          setStepCount((prev) => prev + 1);
        }
      });
      Accelerometer.setUpdateInterval(100); // Update every 100ms
    }
    return () => subscription && subscription.remove();
  }, [isTracking]);

  // Check for significant motion
  const isSignificantMotion = (data) => {
    const { x, y, z } = data;
    const magnitude = Math.sqrt(x * x + y * y + z * z);
    return magnitude > 1.2; // Motion threshold
  };

  // Start/Stop tracking
  const toggleTracking = () => {
    setIsTracking(!isTracking);
    if (isTracking) saveSession();
  };

  // Save session locally
  const saveSession = async () => {
    const session = {
      steps: stepCount,
      timestamp: new Date().toISOString(),
    };
    try {
      const storedLogs = await AsyncStorage.getItem("activityLogs");
      const logs = storedLogs ? JSON.parse(storedLogs) : [];
      logs.push(session);
      await AsyncStorage.setItem("activityLogs", JSON.stringify(logs));
      setActivityLogs(logs);
      Alert.alert("Session Saved", `You took ${stepCount} steps.`);
    } catch (error) {
      console.error("Error saving session:", error);
    }
  };

  // Sync data with backend API
  const syncWithBackend = async () => {
    try {
      const storedLogs = await AsyncStorage.getItem("activityLogs");
      const logs = storedLogs ? JSON.parse(storedLogs) : [];
      const response = await axios.post(`${API_URL}/sync`, { logs });
      if (response.status === 200) {
        Alert.alert("Sync Successful", "All data has been synced with the backend.");
        await AsyncStorage.removeItem("activityLogs"); // Clear synced logs
        setActivityLogs([]);
      }
    } catch (error) {
      console.error("Error syncing with backend:", error);
      Alert.alert("Sync Failed", "Unable to sync data. Try again later.");
    }
  };

  // Load activity logs on component mount
  useEffect(() => {
    const loadLogs = async () => {
      try {
        const storedLogs = await AsyncStorage.getItem("activityLogs");
        setActivityLogs(storedLogs ? JSON.parse(storedLogs) : []);
      } catch (error) {
        console.error("Error loading logs:", error);
      }
    };
    loadLogs();
  }, []);

  // Render activity logs
  const renderLog = ({ item }) => (
    <View style={styles.logItem}>
      <Text>Steps: {item.steps}</Text>
      <Text>Date: {new Date(item.timestamp).toLocaleString()}</Text>
    </View>
  );

  return (
    <View style={styles.container}>
      <Text style={styles.title}>Fitness Tracker</Text>
      <Text>Step Count: {stepCount}</Text>
      <Button title={isTracking ? "Stop Tracking" : "Start Tracking"} onPress={toggleTracking} />
      <Button title="Sync Data with Backend" onPress={syncWithBackend} />
      <Text style={styles.subtitle}>Activity Logs</Text>
      <FlatList
        data={activityLogs}
        renderItem={renderLog}
        keyExtractor={(item, index) => index.toString()}
      />
    </View>
  );
}

// Stylesheet
const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: "center",
    alignItems: "center",
    padding: 20,
    backgroundColor: "#f5f5f5",
  },
  title: {
    fontSize: 24,
    fontWeight: "bold",
    marginBottom: 20,
  },
  subtitle: {
    fontSize: 18,
    fontWeight: "bold",
    marginTop: 20,
  },
  logItem: {
    backgroundColor: "#fff",
    padding: 15,
    marginVertical: 5,
    borderRadius: 8,
    width: "100%",
  },
});