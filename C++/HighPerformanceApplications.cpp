#include <iostream>
#include <opencv2/opencv.hpp> // Include OpenCV library for image processing

using namespace std;
using namespace cv;

// Class to represent an Image Processor
class ImageProcessor {
private:
    Mat image; // Original image in a matrix format

public:
    // Constructor that loads an image from a given path
    ImageProcessor(const string& imagePath) {
        // Load the image from file
        image = imread(imagePath, IMREAD_COLOR);
        if (image.empty()) {
            cout << "Error: Unable to load image from path: " << imagePath << endl;
            exit(1);  // Exit if the image cannot be loaded
        }
    }

    // Method to apply a Gaussian blur filter to the image
    void applyGaussianBlur(int kernelSize = 15) {
        Mat blurredImage;
        // Apply Gaussian blur using OpenCV's GaussianBlur function
        GaussianBlur(image, blurredImage, Size(kernelSize, kernelSize), 0);
        // Update the image with the blurred one
        image = blurredImage;
        cout << "Applied Gaussian Blur with kernel size: " << kernelSize << endl;
    }

    // Method to display the current image in a window
    void displayImage(const string& windowName = "Processed Image") {
        imshow(windowName, image);  // Show the image in a window
        waitKey(1);  // Display the image for 1 millisecond to allow OpenCV to update the window
    }

    // Method to save the processed image to a file
    void saveImage(const string& outputPath) {
        imwrite(outputPath, image);  // Save the processed image to the specified path
        cout << "Processed image saved to: " << outputPath << endl;
    }
};

// Main function to simulate real-time image processing
int main() {
    // Path to the high-resolution image
    string inputImagePath = "input_image.jpg";  // Replace with the actual path of the image
    string outputImagePath = "output_image.jpg"; // Path where the processed image will be saved

    // Create an ImageProcessor object and load the image
    ImageProcessor processor(inputImagePath);

    // Apply a Gaussian blur filter to the image
    processor.applyGaussianBlur(15);  // You can change the kernel size here (e.g., 15)

    // Display the processed image in real-time
    processor.displayImage("Real-Time Image Processing");

    // Simulate a real-time application where the image is processed continuously
    // This loop will run for 10 seconds to simulate real-time processing
    auto startTime = chrono::steady_clock::now();
    while (chrono::steady_clock::now() - startTime < chrono::seconds(10)) {
        // In a real application, you could apply more filters or update the image at each frame
        processor.displayImage("Real-Time Image Processing");
        this_thread::sleep_for(chrono::milliseconds(100));  // Simulate delay in real-time display
    }

    // Save the processed image after 10 seconds
    processor.saveImage(outputImagePath);

    return 0;
}