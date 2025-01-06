// Complete fitness tracking application in a single file
package com.example.fitnessapp;

import android.app.Activity;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.RelativeLayout.LayoutParams;
import android.graphics.Color;

public class MobileDevelopment extends Activity implements SensorEventListener {

    // Sensor-related variables
    private SensorManager sensorManager;
    private Sensor stepCounterSensor;
    private TextView stepCounterView;
    private boolean isSensorAvailable = false;
    private int stepCount = 0;

    // Entry point of the activity
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Create the layout programmatically
        RelativeLayout layout = new RelativeLayout(this);
        layout.setBackgroundColor(Color.parseColor("#F0F0F0"));

        // Create and configure the TextView for step counter
        stepCounterView = new TextView(this);
        stepCounterView.setTextSize(24);
        stepCounterView.setTextColor(Color.BLACK);
        stepCounterView.setText("Steps: 0");
        stepCounterView.setLayoutParams(new LayoutParams(
                LayoutParams.WRAP_CONTENT,
                LayoutParams.WRAP_CONTENT));

        // Center the TextView in the parent layout
        RelativeLayout.LayoutParams params = (RelativeLayout.LayoutParams) stepCounterView.getLayoutParams();
        params.addRule(RelativeLayout.CENTER_IN_PARENT);
        stepCounterView.setLayoutParams(params);
        layout.addView(stepCounterView);

        // Set the layout as the content view
        setContentView(layout);

        // Initialize the SensorManager and check for the step counter sensor
        sensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        if (sensorManager != null) {
            stepCounterSensor = sensorManager.getDefaultSensor(Sensor.TYPE_STEP_COUNTER);
            if (stepCounterSensor != null) {
                isSensorAvailable = true;
            } else {
                stepCounterView.setText("Step Counter Sensor not available!");
            }
        }
    }

    // Resume activity and register the sensor listener
    @Override
    protected void onResume() {
        super.onResume();
        if (isSensorAvailable) {
            sensorManager.registerListener(this, stepCounterSensor, SensorManager.SENSOR_DELAY_UI);
        }
    }

    // Pause activity and unregister the sensor listener
    @Override
    protected void onPause() {
        super.onPause();
        if (isSensorAvailable) {
            sensorManager.unregisterListener(this);
        }
    }

    // Handle sensor data updates
    @Override
    public void onSensorChanged(SensorEvent event) {
        if (event.sensor.getType() == Sensor.TYPE_STEP_COUNTER) {
            // Initialize the step count on the first event
            if (stepCount == 0) {
                stepCount = (int) event.values[0];
            }
            // Calculate and display the current step count
            int currentStepCount = (int) event.values[0] - stepCount;
            stepCounterView.setText("Steps: " + currentStepCount);
        }
    }

    // Handle changes in sensor accuracy (not used here)
    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
        // No action needed for accuracy changes
    }
}