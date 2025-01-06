-- Design a SQL query to identify patterns in patient diagnoses correlated with demographic factors

-- Step 1: Create necessary tables for patient demographics and diagnoses

-- Table to store patient demographics (age, gender, etc.)
CREATE TABLE Patients (
    patient_id INT AUTO_INCREMENT PRIMARY KEY,       -- Unique patient ID
    first_name VARCHAR(100) NOT NULL,                 -- Patient's first name
    last_name VARCHAR(100) NOT NULL,                  -- Patient's last name
    gender VARCHAR(10),                               -- Patient's gender (e.g., Male, Female)
    date_of_birth DATE NOT NULL,                      -- Patient's date of birth
    address VARCHAR(255)                              -- Patient's address
);

-- Table to store diagnosis information
CREATE TABLE Diagnoses (
    diagnosis_id INT AUTO_INCREMENT PRIMARY KEY,     -- Unique diagnosis ID
    patient_id INT,                                   -- Foreign key to Patients table
    diagnosis_date DATE NOT NULL,                     -- Date of diagnosis
    diagnosis_code VARCHAR(50) NOT NULL,              -- ICD code for diagnosis
    diagnosis_description VARCHAR(255) NOT NULL,      -- Diagnosis description
    FOREIGN KEY (patient_id) REFERENCES Patients(patient_id)  -- Foreign key constraint
);

-- Step 2: Insert sample patient data
INSERT INTO Patients (first_name, last_name, gender, date_of_birth, address)
VALUES 
    ('John', 'Doe', 'Male', '1980-06-15', '123 Main St, Cityville'),
    ('Jane', 'Smith', 'Female', '1995-03-22', '456 Oak St, Townsville'),
    ('Mike', 'Johnson', 'Male', '1978-12-11', '789 Pine St, Villageburg'),
    ('Emily', 'Davis', 'Female', '2000-08-05', '101 Maple St, Citytown');

-- Step 3: Insert sample diagnoses data
INSERT INTO Diagnoses (patient_id, diagnosis_date, diagnosis_code, diagnosis_description)
VALUES 
    (1, '2023-01-15', 'I10', 'Hypertension'),
    (1, '2023-03-22', 'E11', 'Type 2 Diabetes'),
    (2, '2023-02-20', 'J45', 'Asthma'),
    (2, '2023-04-18', 'I10', 'Hypertension'),
    (3, '2023-01-10', 'E11', 'Type 2 Diabetes'),
    (3, '2023-05-30', 'J44', 'Chronic Obstructive Pulmonary Disease'),
    (4, '2023-03-12', 'I10', 'Hypertension'),
    (4, '2023-06-05', 'J45', 'Asthma');

-- Step 4: Complex query to identify patterns in patient diagnoses correlated with demographic factors

SELECT 
    p.gender,                                       -- Patient's gender
    COUNT(d.diagnosis_id) AS total_diagnoses,         -- Total number of diagnoses for each gender
    GROUP_CONCAT(DISTINCT d.diagnosis_description) AS diagnosis_types, -- List of unique diagnoses for each gender
    AVG(DATEDIFF(CURDATE(), p.date_of_birth) / 365) AS average_age, -- Average age of patients for each gender
    COUNT(DISTINCT p.patient_id) AS total_patients    -- Total number of patients per gender
FROM 
    Patients p
JOIN 
    Diagnoses d ON p.patient_id = d.patient_id        -- Join Diagnoses and Patients on patient_id
GROUP BY 
    p.gender                                         -- Group by gender to find patterns
ORDER BY 
    total_diagnoses DESC;                            -- Order by total diagnoses (descending)