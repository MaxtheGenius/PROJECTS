// Import the necessary Apache Spark libraries
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.mllib.recommendation.ALS;
import org.apache.spark.mllib.recommendation.MatrixFactorizationModel;
import org.apache.spark.mllib.recommendation.Rating;
import org.apache.spark.SparkConf;

import java.util.Arrays;
import java.util.List;

/*
 * ProductRecommendation
 * Implements a simple product recommendation system using Apache Spark's MLlib library.
 * The system trains a model based on user-product ratings and provides recommendations.
 */
public class ProductRecommendation {

    // Main method - Entry point for the program
    public static void main(String[] args) {
        // Step 1: Configure Spark
        SparkConf conf = new SparkConf()
            .setAppName("ProductRecommendationSystem") // Name of the Spark application
            .setMaster("local[*]"); // Run locally using all available cores
        JavaSparkContext sc = new JavaSparkContext(conf); // Create the Spark context

        // Step 2: Simulate the dataset (UserID, ProductID, Rating)
        // This dataset represents user ratings for products
        List<String> rawData = Arrays.asList(
            "1,101,5.0", // User 1 rates Product 101 with 5.0
            "1,102,3.0", // User 1 rates Product 102 with 3.0
            "1,103,1.0", // User 1 rates Product 103 with 1.0
            "2,101,4.0", // User 2 rates Product 101 with 4.0
            "2,102,2.0", // User 2 rates Product 102 with 2.0
            "3,103,5.0", // User 3 rates Product 103 with 5.0
            "3,104,3.0"  // User 3 rates Product 104 with 3.0
        );

        // Step 3: Load the dataset into a JavaRDD
        JavaRDD<String> data = sc.parallelize(rawData); // Convert the list to an RDD

        // Step 4: Parse the dataset into Rating objects
        JavaRDD<Rating> ratings = data.map(line -> {
            String[] parts = line.split(","); // Split each line into components
            int userId = Integer.parseInt(parts[0]); // Extract the User ID
            int productId = Integer.parseInt(parts[1]); // Extract the Product ID
            double rating = Double.parseDouble(parts[2]); // Extract the Rating
            return new Rating(userId, productId, rating); // Create a Rating object
        });

        // Step 5: Train the ALS model
        int rank = 10; // Number of latent factors in the model
        int iterations = 10; // Number of iterations for training
        double lambda = 0.01; // Regularization parameter
        // Train the model using ALS (Alternating Least Squares)
        MatrixFactorizationModel model = ALS.train(JavaRDD.toRDD(ratings), rank, iterations, lambda);

        // Step 6: Generate product recommendations for a specific user
        int userId = 1; // Specify the user ID for recommendations
        int numRecommendations = 5; // Number of recommendations to generate
        Rating[] recommendations = model.recommendProducts(userId, numRecommendations);

        // Step 7: Display the recommendations
        System.out.println("Product Recommendations for User ID " + userId + ":");
        for (Rating r : recommendations) {
            System.out.println("Product ID: " + r.product() + " | Predicted Rating: " + r.rating());
        }

        // Step 8: Stop the Spark context
        sc.stop(); // Clean up resources used by the Spark context
    }
}