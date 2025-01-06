-- Implement a role-based access control system using SQL to manage user permissions in an enterprise application

-- Step 1: Create tables for roles, users, permissions, and role assignments

-- Table to store user roles
CREATE TABLE Roles (
    role_id INT AUTO_INCREMENT PRIMARY KEY,           -- Unique role ID
    role_name VARCHAR(100) NOT NULL                   -- Name of the role (e.g., 'Admin', 'User')
);

-- Table to store permissions
CREATE TABLE Permissions (
    permission_id INT AUTO_INCREMENT PRIMARY KEY,    -- Unique permission ID
    permission_name VARCHAR(100) NOT NULL             -- Name of the permission (e.g., 'view_dashboard', 'edit_profile')
);

-- Table to store users
CREATE TABLE Users (
    user_id INT AUTO_INCREMENT PRIMARY KEY,          -- Unique user ID
    username VARCHAR(100) NOT NULL,                   -- Username of the user
    password VARCHAR(255) NOT NULL,                   -- User password (hashed, for security)
    email VARCHAR(100) NOT NULL,                      -- Email of the user
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP    -- Account creation timestamp
);

-- Table to store the relationship between users and roles (many-to-many relationship)
CREATE TABLE UserRoles (
    user_id INT,                                     -- User ID
    role_id INT,                                     -- Role ID
    PRIMARY KEY (user_id, role_id),                   -- Composite primary key
    FOREIGN KEY (user_id) REFERENCES Users(user_id), -- Foreign key referencing Users table
    FOREIGN KEY (role_id) REFERENCES Roles(role_id)  -- Foreign key referencing Roles table
);

-- Table to store the relationship between roles and permissions (many-to-many relationship)
CREATE TABLE RolePermissions (
    role_id INT,                                     -- Role ID
    permission_id INT,                               -- Permission ID
    PRIMARY KEY (role_id, permission_id),            -- Composite primary key
    FOREIGN KEY (role_id) REFERENCES Roles(role_id), -- Foreign key referencing Roles table
    FOREIGN KEY (permission_id) REFERENCES Permissions(permission_id)  -- Foreign key referencing Permissions table
);

-- Step 2: Insert sample roles and permissions

-- Insert sample roles (e.g., 'Manager', 'Customer')
INSERT INTO Roles (role_name) VALUES ('Manager'), ('Customer');

-- Insert sample permissions (e.g., 'view_dashboard', 'edit_profile', 'delete_account')
INSERT INTO Permissions (permission_name) VALUES ('view_dashboard'), ('edit_profile'), ('delete_account');

-- Step 3: Insert sample users

-- Insert the user with username 'James' (hashed password for security)
INSERT INTO Users (username, password, email) 
VALUES ('James', 'hashed_password_1', 'james@example.com');

-- Step 4: Assign roles to the user 'James'

-- Assign 'Manager' role to 'James'
INSERT INTO UserRoles (user_id, role_id)
VALUES ((SELECT user_id FROM Users WHERE username = 'James'), 
        (SELECT role_id FROM Roles WHERE role_name = 'Manager'));

-- Step 5: Assign permissions to roles

-- Assign 'view_dashboard' permission to 'Manager' role
INSERT INTO RolePermissions (role_id, permission_id)
VALUES ((SELECT role_id FROM Roles WHERE role_name = 'Manager'),
        (SELECT permission_id FROM Permissions WHERE permission_name = 'view_dashboard'));

-- Assign 'edit_profile' permission to 'Customer' role
INSERT INTO RolePermissions (role_id, permission_id)
VALUES ((SELECT role_id FROM Roles WHERE role_name = 'Customer'),
        (SELECT permission_id FROM Permissions WHERE permission_name = 'edit_profile'));

-- Step 6: Query to check permissions for user 'James'

-- Example: Check permissions for 'James'
SELECT 
    u.username, 
    r.role_name, 
    p.permission_name
FROM 
    Users u
JOIN 
    UserRoles ur ON u.user_id = ur.user_id
JOIN 
    Roles r ON ur.role_id = r.role_id
JOIN 
    RolePermissions rp ON r.role_id = rp.role_id
JOIN 
    Permissions p ON rp.permission_id = p.permission_id
WHERE 
    u.username = 'James';  -- We check the permissions for 'James'