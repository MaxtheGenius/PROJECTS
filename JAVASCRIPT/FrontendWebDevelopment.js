import React, { useState } from "react";
import ReactDOM from "react-dom";
import { FaShoppingCart } from "react-icons/fa";

/*
    Integrated E-Commerce Application
    A single-page React application with a dynamic shopping cart and real-time product filters.
*/

// Sample product data
const products = [
  { id: 1, name: "Smartphone", category: "Electronics", price: 499.99 },
  { id: 2, name: "Laptop", category: "Electronics", price: 999.99 },
  { id: 3, name: "Sofa", category: "Furniture", price: 299.99 },
  { id: 4, name: "T-shirt", category: "Clothing", price: 19.99 },
  { id: 5, name: "Headphones", category: "Electronics", price: 89.99 },
  { id: 6, name: "Dining Table", category: "Furniture", price: 399.99 },
];

// Main Application Component
function App() {
  const [cart, setCart] = useState([]); // Shopping cart state
  const [filter, setFilter] = useState(""); // Category filter state

  // Add product to the shopping cart
  const addToCart = (product) => setCart([...cart, product]);

  // Remove product from the shopping cart
  const removeFromCart = (productId) =>
    setCart(cart.filter((item) => item.id !== productId));

  // Filter products based on selected category
  const filteredProducts = filter
    ? products.filter((product) => product.category === filter)
    : products;

  return (
    <div style={styles.app}>
      <header style={styles.header}>
        <h1 style={styles.title}>My E-Commerce Store</h1>
        <Cart cart={cart} removeFromCart={removeFromCart} />
      </header>
      <main style={styles.main}>
        <Filters setFilter={setFilter} />
        <ProductList products={filteredProducts} addToCart={addToCart} />
      </main>
    </div>
  );
}

// Cart Component
function Cart({ cart, removeFromCart }) {
  const total = cart.reduce((sum, item) => sum + item.price, 0);

  return (
    <div style={styles.cart}>
      <h2>
        <FaShoppingCart /> Shopping Cart
      </h2>
      {cart.length === 0 ? (
        <p>Your cart is empty.</p>
      ) : (
        <ul style={styles.cartList}>
          {cart.map((item) => (
            <li key={item.id} style={styles.cartItem}>
              {item.name} - ${item.price.toFixed(2)}
              <button
                style={styles.removeButton}
                onClick={() => removeFromCart(item.id)}
              >
                Remove
              </button>
            </li>
          ))}
        </ul>
      )}
      <h3>Total: ${total.toFixed(2)}</h3>
    </div>
  );
}

// Filters Component
function Filters({ setFilter }) {
  const categories = ["All", ...new Set(products.map((p) => p.category))];

  return (
    <div style={styles.filters}>
      <h2>Filter by Category</h2>
      {categories.map((category) => (
        <button
          key={category}
          style={styles.filterButton}
          onClick={() => setFilter(category === "All" ? "" : category)}
        >
          {category}
        </button>
      ))}
    </div>
  );
}

// ProductList Component
function ProductList({ products, addToCart }) {
  return (
    <div style={styles.productList}>
      <h2>Products</h2>
      {products.map((product) => (
        <div key={product.id} style={styles.productCard}>
          <h3>{product.name}</h3>
          <p>Category: {product.category}</p>
          <p>Price: ${product.price.toFixed(2)}</p>
          <button style={styles.addButton} onClick={() => addToCart(product)}>
            Add to Cart
          </button>
        </div>
      ))}
    </div>
  );
}

// Inline Styles
const styles = {
  app: { fontFamily: "Arial, sans-serif", padding: "1rem", backgroundColor: "#f9f9f9" },
  header: { display: "flex", justifyContent: "space-between", alignItems: "center", backgroundColor: "#0073e6", padding: "1rem", color: "white" },
  title: { margin: 0 },
  main: { padding: "1rem" },
  cart: { backgroundColor: "white", padding: "1rem", borderRadius: "8px", boxShadow: "0 2px 5px rgba(0, 0, 0, 0.1)" },
  cartList: { listStyle: "none", padding: 0 },
  cartItem: { display: "flex", justifyContent: "space-between", marginBottom: "0.5rem" },
  removeButton: { backgroundColor: "red", color: "white", border: "none", padding: "0.5rem", borderRadius: "4px", cursor: "pointer" },
  filters: { marginBottom: "1rem" },
  filterButton: { marginRight: "0.5rem", backgroundColor: "#0073e6", color: "white", border: "none", padding: "0.5rem 1rem", borderRadius: "4px", cursor: "pointer" },
  productList: { display: "grid", gridTemplateColumns: "repeat(auto-fill, minmax(200px, 1fr))", gap: "1rem" },
  productCard: { backgroundColor: "white", padding: "1rem", borderRadius: "8px", boxShadow: "0 2px 5px rgba(0, 0, 0, 0.1)", textAlign: "center" },
  addButton: { backgroundColor: "#0073e6", color: "white", border: "none", padding: "0.5rem 1rem", borderRadius: "4px", cursor: "pointer" },
};

// Render the application
ReactDOM.render(<App />, document.getElementById("root"));