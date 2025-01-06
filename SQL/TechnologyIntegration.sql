-- Develop a RESTful API that uses SQL to retrieve and update real-time inventory data

// Required dependencies: 
// - Spring Boot
// - Spring Data JPA
// - MySQL Driver

// Step 1: SQL Schema to create database tables
/*
-- Table to store product inventory
CREATE TABLE Products (
    product_id INT AUTO_INCREMENT PRIMARY KEY,        -- Unique product ID
    product_name VARCHAR(100) NOT NULL,                -- Name of the product
    quantity INT NOT NULL,                             -- Quantity in stock
    price DECIMAL(10, 2) NOT NULL,                     -- Price of the product
    last_updated TIMESTAMP DEFAULT CURRENT_TIMESTAMP   -- Last updated timestamp
);
*/

// Step 2: Java classes and Spring Boot configuration

// Entity class mapping the Products table
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import java.math.BigDecimal;
import java.time.LocalDateTime;

@Entity
public class Product {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long productId;  // Unique product identifier
    private String productName;  // Product name
    private int quantity;  // Quantity of product in stock
    private BigDecimal price;  // Price of product
    private LocalDateTime lastUpdated;  // Timestamp of last update

    // Getters and setters for all fields
    public Long getProductId() {
        return productId;
    }

    public void setProductId(Long productId) {
        this.productId = productId;
    }

    public String getProductName() {
        return productName;
    }

    public void setProductName(String productName) {
        this.productName = productName;
    }

    public int getQuantity() {
        return quantity;
    }

    public void setQuantity(int quantity) {
        this.quantity = quantity;
    }

    public BigDecimal getPrice() {
        return price;
    }

    public void setPrice(BigDecimal price) {
        this.price = price;
    }

    public LocalDateTime getLastUpdated() {
        return lastUpdated;
    }

    public void setLastUpdated(LocalDateTime lastUpdated) {
        this.lastUpdated = lastUpdated;
    }
}

// Repository Interface to handle CRUD operations
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface ProductRepository extends JpaRepository<Product, Long> {
    // You can add custom queries here if needed
}

// Service class with business logic for product management
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import java.util.Optional;

@Service
public class ProductService {

    @Autowired
    private ProductRepository productRepository;

    // Method to get product by ID
    public Optional<Product> getProductById(Long id) {
        return productRepository.findById(id);
    }

    // Method to add or update product
    public Product saveProduct(Product product) {
        return productRepository.save(product);
    }

    // Method to delete product by ID
    public void deleteProduct(Long id) {
        productRepository.deleteById(id);
    }

    // Method to update product quantity
    public Product updateProductQuantity(Long id, int quantity) {
        Optional<Product> productOptional = productRepository.findById(id);
        if (productOptional.isPresent()) {
            Product product = productOptional.get();
            product.setQuantity(quantity);  // Update the quantity
            return productRepository.save(product);
        }
        return null;  // Return null if product not found
    }
}

// Controller class to define RESTful API endpoints
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/api/products")  // Define the base URI for all product-related API calls
public class ProductController {

    @Autowired
    private ProductService productService;

    // GET endpoint to fetch product by ID
    @GetMapping("/{id}")
    public ResponseEntity<Product> getProductById(@PathVariable Long id) {
        Optional<Product> product = productService.getProductById(id);
        return product.map(ResponseEntity::ok).orElseGet(() -> ResponseEntity.notFound().build());
    }

    // POST endpoint to add or update a product
    @PostMapping
    public ResponseEntity<Product> addOrUpdateProduct(@RequestBody Product product) {
        Product savedProduct = productService.saveProduct(product);
        return ResponseEntity.ok(savedProduct);
    }

    // DELETE endpoint to remove a product by ID
    @DeleteMapping("/{id}")
    public ResponseEntity<Void> deleteProduct(@PathVariable Long id) {
        productService.deleteProduct(id);
        return ResponseEntity.noContent().build();
    }

    // PUT endpoint to update product quantity
    @PutMapping("/{id}/quantity")
    public ResponseEntity<Product> updateProductQuantity(@PathVariable Long id, @RequestBody int quantity) {
        Product updatedProduct = productService.updateProductQuantity(id, quantity);
        return updatedProduct != null ? ResponseEntity.ok(updatedProduct) : ResponseEntity.notFound().build();
    }
}

// Spring Boot Application Class
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication  // Marks this as a Spring Boot application
public class InventoryApplication {

    public static void main(String[] args) {
        SpringApplication.run(InventoryApplication.class, args);  // Run the Spring Boot application
    }
}

// application.properties for MySQL database configuration
/*
spring.datasource.url=jdbc:mysql://localhost:3306/inventory_db  -- Your MySQL database URL
spring.datasource.username=root  -- Database username
spring.datasource.password=yourpassword  -- Database password
spring.jpa.hibernate.ddl-auto=update  -- Hibernate will manage the schema (create or update)
spring.jpa.properties.hibernate.dialect=org.hibernate.dialect.MySQL8Dialect  -- Dialect for MySQL
spring.jpa.show-sql=true  -- Show SQL queries in the console
spring.jpa.format-sql=true  -- Format the SQL for readability
*/

// Final Remarks:
// After implementing the above classes and configurations, you can run the application
// using the command `mvn spring-boot:run` or by running the main method in `InventoryApplication`.
// The RESTful API will be available for interacting with inventory data via endpoints:
// 1. GET /api/products/{id} - Fetch product by ID
// 2. POST /api/products - Add or update a product
// 3. DELETE /api/products/{id} - Delete product by ID
// 4. PUT /api/products/{id}/quantity - Update the quantity of a product