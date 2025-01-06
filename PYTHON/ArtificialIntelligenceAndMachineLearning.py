import tensorflow as tf
from tensorflow.keras import layers, models
from tensorflow.keras.preprocessing.image import ImageDataGenerator
import os
import numpy as np
import matplotlib.pyplot as plt

# Step 1: Load and preprocess the data
def load_and_preprocess_data(train_dir, val_dir, image_size=(150, 150), batch_size=32):
    """
    Load and preprocess training and validation data using ImageDataGenerator.
    """
    train_datagen = ImageDataGenerator(
        rescale=1./255,  # Rescale pixel values to [0, 1]
        rotation_range=20,
        width_shift_range=0.2,
        height_shift_range=0.2,
        shear_range=0.2,
        zoom_range=0.2,
        horizontal_flip=True,
        fill_mode='nearest'
    )
    
    val_datagen = ImageDataGenerator(rescale=1./255)
    
    # Load training and validation data
    train_generator = train_datagen.flow_from_directory(
        train_dir,
        target_size=image_size,
        batch_size=batch_size,
        class_mode='categorical'  # Multi-class classification
    )
    
    validation_generator = val_datagen.flow_from_directory(
        val_dir,
        target_size=image_size,
        batch_size=batch_size,
        class_mode='categorical'
    )
    
    return train_generator, validation_generator

# Step 2: Build the deep learning model (CNN)
def build_model(input_shape=(150, 150, 3)):
    """
    Build a CNN model to classify product images.
    """
    model = models.Sequential([
        layers.Conv2D(32, (3, 3), activation='relu', input_shape=input_shape),
        layers.MaxPooling2D(2, 2),
        
        layers.Conv2D(64, (3, 3), activation='relu'),
        layers.MaxPooling2D(2, 2),
        
        layers.Conv2D(128, (3, 3), activation='relu'),
        layers.MaxPooling2D(2, 2),
        
        layers.Flatten(),
        layers.Dense(128, activation='relu'),
        layers.Dense(64, activation='relu'),
        layers.Dense(10, activation='softmax')  # 10 classes, change based on your dataset
    ])
    
    # Compile the model
    model.compile(
        optimizer='adam',
        loss='categorical_crossentropy',  # Multi-class classification
        metrics=['accuracy']
    )
    
    model.summary()
    
    return model

# Step 3: Train the model
def train_model(model, train_generator, validation_generator, epochs=10):
    """
    Train the model using the training and validation data generators.
    """
    history = model.fit(
        train_generator,
        epochs=epochs,
        validation_data=validation_generator
    )
    
    return history

# Step 4: Plot training and validation accuracy/loss
def plot_training_history(history):
    """
    Plot the training and validation accuracy and loss.
    """
    # Plot accuracy
    plt.figure(figsize=(10, 5))
    plt.subplot(1, 2, 1)
    plt.plot(history.history['accuracy'], label='Training Accuracy')
    plt.plot(history.history['val_accuracy'], label='Validation Accuracy')
    plt.title('Accuracy')
    plt.xlabel('Epochs')
    plt.ylabel('Accuracy')
    plt.legend()
    
    # Plot loss
    plt.subplot(1, 2, 2)
    plt.plot(history.history['loss'], label='Training Loss')
    plt.plot(history.history['val_loss'], label='Validation Loss')
    plt.title('Loss')
    plt.xlabel('Epochs')
    plt.ylabel('Loss')
    plt.legend()
    
    plt.tight_layout()
    plt.show()

# Step 5: Save the trained model
def save_model(model, model_name='product_classifier_model.h5'):
    """
    Save the trained model to a file.
    """
    model.save(model_name)
    print(f"Model saved as {model_name}")

# Step 6: Make predictions on new images
def predict_image(model, image_path, image_size=(150, 150)):
    """
    Predict the class of a new product image using the trained model.
    """
    img = tf.keras.preprocessing.image.load_img(image_path, target_size=image_size)
    img_array = tf.keras.preprocessing.image.img_to_array(img)
    img_array = np.expand_dims(img_array, axis=0)  # Add batch dimension
    
    # Normalize the image
    img_array = img_array / 255.0
    
    prediction = model.predict(img_array)
    predicted_class = np.argmax(prediction, axis=1)
    
    print(f"Predicted Class: {predicted_class}")
    
    return predicted_class

# Step 7: Main function to execute the process
def main():
    # Define directories for training and validation data
    train_dir = 'data/train'  # Replace with your actual training data directory
    val_dir = 'data/val'      # Replace with your actual validation data directory
    
    # Load and preprocess the data
    train_generator, validation_generator = load_and_preprocess_data(train_dir, val_dir)
    
    # Build the model
    model = build_model(input_shape=(150, 150, 3))
    
    # Train the model
    history = train_model(model, train_generator, validation_generator, epochs=10)
    
    # Plot training history
    plot_training_history(history)
    
    # Save the model
    save_model(model)
    
    # Make predictions on a new image (for example, test with a new image)
    test_image_path = 'data/test_image.jpg'  # Replace with your image path
    predict_image(model, test_image_path)

if __name__ == "__main__":
    main()