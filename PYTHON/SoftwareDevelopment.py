# Flask Web Application

# Import required libraries for Flask and SQLAlchemy
from flask import Flask, request, jsonify
from flask_sqlalchemy import SQLAlchemy

# Initialize the Flask application
app = Flask(__name__)

# Configure the database URI (SQLite for simplicity)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///contacts.db'

# Initialize the SQLAlchemy object
db = SQLAlchemy(app)

# Define the 'Contact' model (table) in the database
class Contact(db.Model):
    id = db.Column(db.Integer, primary_key=True)      # Unique ID for each contact
    name = db.Column(db.String(100), nullable=False)   # Name of the contact
    email = db.Column(db.String(100), unique=True, nullable=False)  # Email (must be unique)
    phone = db.Column(db.String(15), nullable=True)    # Phone number (optional)

    def __repr__(self):
        return f"<Contact {self.name}>"

# Route to add a new contact (POST request)
@app.route('/contacts', methods=['POST'])
def add_contact():
    # Get data from the incoming request
    data = request.get_json()
    
    # Create a new contact object with the provided data
    new_contact = Contact(name=data['name'], email=data['email'], phone=data.get('phone'))
    
    # Add the new contact to the database and commit the transaction
    db.session.add(new_contact)
    db.session.commit()
    
    # Return a success message in JSON format
    return jsonify({"message": "Contact added successfully!"}), 201

# Route to retrieve all contacts (GET request)
@app.route('/contacts', methods=['GET'])
def get_contacts():
    # Retrieve all contacts from the database
    contacts = Contact.query.all()
    
    # Create a list of contacts as dictionaries
    contacts_list = [{"id": contact.id, "name": contact.name, "email": contact.email, "phone": contact.phone} for contact in contacts]
    
    # Return the contacts in JSON format
    return jsonify(contacts_list), 200

# Route to update a contact's information (PUT request)
@app.route('/contacts/<int:id>', methods=['PUT'])
def update_contact(id):
    # Get data from the incoming request
    data = request.get_json()
    
    # Find the contact by ID
    contact = Contact.query.get(id)
    
    # If the contact is not found, return a 404 error
    if not contact:
        return jsonify({"message": "Contact not found!"}), 404
    
    # Update the contact's details
    contact.name = data['name']
    contact.email = data['email']
    contact.phone = data.get('phone')
    
    # Commit the changes to the database
    db.session.commit()
    
    # Return a success message
    return jsonify({"message": "Contact updated successfully!"})

# Route to delete a contact (DELETE request)
@app.route('/contacts/<int:id>', methods=['DELETE'])
def delete_contact(id):
    # Find the contact by ID
    contact = Contact.query.get(id)
    
    # If the contact is not found, return a 404 error
    if not contact:
        return jsonify({"message": "Contact not found!"}), 404
    
    # Delete the contact from the database
    db.session.delete(contact)
    db.session.commit()
    
    # Return a success message
    return jsonify({"message": "Contact deleted successfully!"})

# Start the Flask web application
if __name__ == '__main__':
    # Create the database tables if they don't already exist
    db.create_all()
    
    # Run the application in debug mode (for development)
    app.run(debug=True)

# test_app.py - Unit Test Script

# Import required libraries for testing
import unittest
from app import app, db, Contact

class TestContactManager(unittest.TestCase):

    # Set up a test client and initialize the database
    def setUp(self):
        self.app = app.test_client()  # Create a test client for the Flask application
        self.app.testing = True  # Set the application in testing mode
        db.create_all()  # Create the database tables for testing

    # Clean up after each test (remove the session and drop the database)
    def tearDown(self):
        db.session.remove()  # Remove any sessions
        db.drop_all()  # Drop all the tables in the database

    # Test adding a new contact
    def test_add_contact(self):
        response = self.app.post('/contacts', json={
            'name': 'John Doe',
            'email': 'john.doe@example.com',
            'phone': '1234567890'
        })
        self.assertEqual(response.status_code, 201)  # Assert that the response is a success (201)
        self.assertIn(b'Contact added successfully!', response.data)  # Assert success message in the response

    # Test retrieving all contacts
    def test_get_contacts(self):
        self.app.post('/contacts', json={'name': 'Jane Doe', 'email': 'jane.doe@example.com', 'phone': '0987654321'})
        response = self.app.get('/contacts')
        self.assertEqual(response.status_code, 200)  # Assert success (200)
        self.assertIn(b'Jane Doe', response.data)  # Assert that the contact is in the response

    # Test updating an existing contact
    def test_update_contact(self):
        self.app.post('/contacts', json={'name': 'Mike Smith', 'email': 'mike.smith@example.com', 'phone': '1231231234'})
        response = self.app.put('/contacts/1', json={'name': 'Michael Smith', 'email': 'michael.smith@example.com', 'phone': '1231231235'})
        self.assertEqual(response.status_code, 200)  # Assert success (200)
        self.assertIn(b'Contact updated successfully!', response.data)  # Assert success message in the response

    # Test deleting an existing contact
    def test_delete_contact(self):
        self.app.post('/contacts', json={'name': 'David Lee', 'email': 'david.lee@example.com', 'phone': '1112223333'})
        response = self.app.delete('/contacts/1')
        self.assertEqual(response.status_code, 200)  # Assert success (200)
        self.assertIn(b'Contact deleted successfully!', response.data)  # Assert success message in the response

if __name__ == '__main__':
    unittest.main()  # Run the tests

# ci_script.py - Continuous Integration & Deployment Script

import subprocess
import sys

# Function to run unit tests
def run_tests():
    """Run the unit tests."""
    print("Running unit tests...")
    result = subprocess.run([sys.executable, "-m", "unittest", "test_app.py"], capture_output=True, text=True)
    if result.returncode != 0:
        print("Tests failed.")
        print(result.stderr)
        sys.exit(1)
    print("Tests passed successfully.")

# Function to deploy the application
def deploy_app():
    """Deploy the web application."""
    print("Deploying the web application...")
    subprocess.run(["flask", "run"], check=True)  # Simulate deployment command
    print("Deployment completed successfully.")

# Main function for Continuous Integration process
def continuous_integration():
    """CI pipeline to automate testing and deployment."""
    run_tests()  # Run the unit tests first
    deploy_app()  # Deploy the application if tests pass

# Main execution
if __name__ == "__main__":
    continuous_integration()  # Start the CI pipeline

# .github/workflows/ci.yml - GitHub Actions Workflow Configuration
name: CI/CD Pipeline

on:
  push:
    branches:
      - main  # Trigger on push to main branch

jobs:
  build:
    runs-on: ubuntu-latest  # Run on the latest Ubuntu

    steps:
      - name: Checkout code
        uses: actions/checkout@v2  # Checkout the code from the repository

      - name: Set up Python
        uses: actions/setup-python@v2  # Set up Python environment
        with:
          python-version: '3.x'  # Use Python 3.x

      - name: Install dependencies
        run: |
          pip install -r requirements.txt  # Install required dependencies

      - name: Run Tests and Deploy
        run: |
          python ci_script.py  # Run the CI script to test and deploy