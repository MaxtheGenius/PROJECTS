# Tool to Analyze Medical Images and Detect Anomalies

import cv2  # Library for image processing
import numpy as np  # Library for numerical operations
import tensorflow as tf  # TensorFlow library for machine learning
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Conv2D, MaxPooling2D, Flatten, Dense
from tensorflow.keras.preprocessing.image import ImageDataGenerator
from tensorflow.keras.optimizers import Adam
import os
import matplotlib.pyplot as plt  # Library for visualization
from sklearn.model_selection import train_test_split

# Step 1: Load and preprocess medical images
def load_and_preprocess_images(image_dir, image_size=(224, 224)):
    """
    Load and preprocess images for training or testing.
    
    Parameters:
        image_dir (str): Directory containing the medical images.
        image_size (tuple): The target image size for the model.
        
    Returns:
        np.array: Array of processed images.
        np.array: Array of image labels (0 for normal, 1 for anomaly).
    """
    images = []
    labels = []
    
    # Loop through the image directory and load images
    for label in ['normal', 'anomaly']:  # Assuming images are in 'normal' and 'anomaly' folders
        label_dir = os.path.join(image_dir, label)
        for filename in os.listdir(label_dir):
            if filename.endswith(".png") or filename.endswith(".jpg"):
                img_path = os.path.join(label_dir, filename)
                img = cv2.imread(img_path)  # Read image
                img = cv2.resize(img, image_size)  # Resize to the target size
                img = img / 255.0  # Normalize the image to [0, 1]
                images.append(img)
                labels.append(0 if label == 'normal' else 1)  # Assign labels: 0 for normal, 1 for anomaly
    
    images = np.array(images)
    labels = np.array(labels)
    
    return images, labels

# Step 2: Define the Convolutional Neural Network (CNN) Model for Anomaly Detection
def build_cnn_model(input_shape=(224, 224, 3)):
    """
    Build and compile a CNN model to detect anomalies in medical images.
    
    Parameters:
        input_shape (tuple): Shape of the input images.
        
    Returns:
        tf.keras.Model: Compiled CNN model.
    """
    model = Sequential([
        Conv2D(32, (3, 3), activation='relu', input_shape=input_shape),
        MaxPooling2D(2, 2),
        Conv2D(64, (3, 3), activation='relu'),
        MaxPooling2D(2, 2),
        Conv2D(128, (3, 3), activation='relu'),
        MaxPooling2D(2, 2),
        Flatten(),
        Dense(128, activation='relu'),
        Dense(1, activation='sigmoid')  # Sigmoid activation for binary classification
    ])
    
    model.compile(optimizer=Adam(), loss='binary_crossentropy', metrics=['accuracy'])
    
    return model

# Step 3: Train the Model
def train_model(model, images, labels, validation_split=0.2, batch_size=32, epochs=10):
    """
    Train the CNN model on the image dataset.
    
    Parameters:
        model (tf.keras.Model): The CNN model to train.
        images (np.array): The array of images for training.
        labels (np.array): The array of labels for training.
        validation_split (float): Proportion of data to use for validation.
        batch_size (int): The batch size for training.
        epochs (int): The number of epochs to train the model.
        
    Returns:
        tf.keras.Model: The trained model.
    """
    # Split the data into training and validation sets
    X_train, X_val, y_train, y_val = train_test_split(images, labels, test_size=validation_split)
    
    # Fit the model on the training data
    model.fit(X_train, y_train, validation_data=(X_val, y_val), batch_size=batch_size, epochs=epochs)
    
    return model

# Step 4: Evaluate the Model
def evaluate_model(model, images, labels):
    """
    Evaluate the trained model on the given images and labels.
    
    Parameters:
        model (tf.keras.Model): The trained CNN model.
        images (np.array): The array of images to evaluate.
        labels (np.array): The array of labels for evaluation.
        
    Returns:
        float: The accuracy of the model on the evaluation dataset.
    """
    loss, accuracy = model.evaluate(images, labels)
    print(f"Model Accuracy: {accuracy * 100:.2f}%")
    return accuracy

# Step 5: Use the Model for Anomaly Detection
def detect_anomalies(model, image_path, image_size=(224, 224)):
    """
    Use the trained model to detect anomalies in a medical image.
    
    Parameters:
        model (tf.keras.Model): The trained CNN model.
        image_path (str): Path to the medical image for detection.
        image_size (tuple): The target image size for the model.
        
    Returns:
        str: 'Anomaly' or 'Normal' based on the prediction.
    """
    # Load and preprocess the image
    img = cv2.imread(image_path)
    img = cv2.resize(img, image_size)  # Resize to the target size
    img = img / 255.0  # Normalize the image to [0, 1]
    img = np.expand_dims(img, axis=0)  # Add batch dimension
    
    # Predict the class (normal or anomaly)
    prediction = model.predict(img)
    
    if prediction > 0.5:
        return 'Anomaly'
    else:
        return 'Normal'

# Step 6: Visualize the Results
def visualize_results(image_path, prediction):
    """
    Visualize the result of the anomaly detection.
    
    Parameters:
        image_path (str): Path to the image.
        prediction (str): The predicted result ('Anomaly' or 'Normal').
    """
    img = cv2.imread(image_path)
    plt.imshow(cv2.cvtColor(img, cv2.COLOR_BGR2RGB))  # Convert from BGR to RGB
    plt.title(f"Prediction: {prediction}")
    plt.axis('off')
    plt.show()

# Step 7: Main Execution
if __name__ == '__main__':
    # Define the image directory containing 'normal' and 'anomaly' folders
    image_dir = 'path_to_medical_images'  # Change this to your image directory
    
    # Step 1: Load and preprocess images
    images, labels = load_and_preprocess_images(image_dir)
    
    # Step 2: Build the CNN model
    model = build_cnn_model(input_shape=(224, 224, 3))
    
    # Step 3: Train the model
    model = train_model(model, images, labels, validation_split=0.2, batch_size=32, epochs=10)
    
    # Step 4: Evaluate the model
    accuracy = evaluate_model(model, images, labels)
    
    # Step 5: Use the model to detect anomalies in new medical images
    test_image_path = 'path_to_test_image.jpg'  # Change this to a test image
    prediction = detect_anomalies(model, test_image_path)
    
    # Step 6: Visualize the result
    visualize_results(test_image_path, prediction)