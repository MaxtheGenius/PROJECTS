-- Implement a stored procedure in SQL to automate the cleanup of obsolete logs in a database record system

-- Step 1: Create the Logs table to store log entries
CREATE TABLE Logs (
    log_id INT AUTO_INCREMENT PRIMARY KEY,             -- Unique log ID
    log_message TEXT NOT NULL,                         -- The log message
    log_timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP  -- Timestamp when the log was created
);

-- Step 2: Insert sample log data
INSERT INTO Logs (log_message) VALUES
    ('Log entry 1'),
    ('Log entry 2'),
    ('Log entry 3'),
    ('Log entry 4'),
    ('Log entry 5');

-- Step 3: Create a stored procedure to clean up obsolete logs (older than 30 days)
DELIMITER $$

CREATE PROCEDURE CleanUpOldLogs()
BEGIN
    -- Delete logs that are older than 30 days
    DELETE FROM Logs
    WHERE log_timestamp < NOW() - INTERVAL 30 DAY;
    
    -- Optional: You can output the number of rows deleted
    SELECT ROW_COUNT() AS 'Deleted Rows';
END $$

DELIMITER ;

-- Step 4: Execute the stored procedure to clean up old logs
CALL CleanUpOldLogs();

-- Step 5: Optionally, check the remaining logs after cleanup
SELECT * FROM Logs;