<?php

// This script automates generating and sending weekly reports via email to company departments.

// Include PHPMailer for email functionality
use PHPMailer\PHPMailer\PHPMailer;
use PHPMailer\PHPMailer\Exception;

// Autoload PHPMailer classes (ensure PHPMailer is installed via Composer)
require 'vendor/autoload.php';

// Function to generate weekly reports for each department
function generateWeeklyReports() {
    $departments = ['Sales', 'Human Resources', 'IT Support', 'Finance'];
    $reports = [];

    foreach ($departments as $department) {
        $reportContent = "Weekly Report for $department Department\n";
        $reportContent .= "=================================\n";
        $reportContent .= "This is a summary of the activities and metrics for the week.\n";
        $reportContent .= "Generated on: " . date('Y-m-d H:i:s') . "\n\n";

        switch ($department) {
            case 'Sales':
                $reportContent .= "- Total Revenue: $" . rand(50000, 150000) . "\n";
                $reportContent .= "- New Clients Acquired: " . rand(10, 50) . "\n";
                $reportContent .= "- Total Deals Closed: " . rand(20, 70) . "\n";
                break;
            case 'Human Resources':
                $reportContent .= "- New Hires: " . rand(5, 15) . "\n";
                $reportContent .= "- Resignations: " . rand(1, 5) . "\n";
                $reportContent .= "- Employee Training Hours: " . rand(20, 100) . " hours\n";
                break;
            case 'IT Support':
                $reportContent .= "- Support Tickets Resolved: " . rand(100, 300) . "\n";
                $reportContent .= "- System Downtime: " . rand(0, 5) . " hours\n";
                $reportContent .= "- Software Updates Completed: " . rand(5, 20) . "\n";
                break;
            case 'Finance':
                $reportContent .= "- Budget Utilization: " . rand(70, 100) . "%\n";
                $reportContent .= "- Expense Reports Processed: " . rand(50, 200) . "\n";
                $reportContent .= "- Profit Margin: " . rand(15, 40) . "%\n";
                break;
        }

        $reports[$department] = $reportContent;
    }

    return $reports;
}

// Function to send email using PHPMailer
function sendEmail($recipient, $subject, $body) {
    $mail = new PHPMailer(true);

    try {
        // SMTP configuration
        $mail->isSMTP();
        $mail->Host = 'smtp.gmail.com'; // Example: Gmail's SMTP server
        $mail->SMTPAuth = true;
        $mail->Username = 'company.reports@gmail.com'; // Example email address
        $mail->Password = 'securepassword123';         // Example password
        $mail->SMTPSecure = PHPMailer::ENCRYPTION_STARTTLS;
        $mail->Port = 587;

        // Sender and recipient settings
        $mail->setFrom('company.reports@gmail.com', 'Company Reports');
        $mail->addAddress($recipient);

        // Email content
        $mail->isHTML(false);
        $mail->Subject = $subject;
        $mail->Body = $body;

        $mail->send();
        echo "Email sent to $recipient successfully.\n";
    } catch (Exception $e) {
        echo "Failed to send email to $recipient. Error: " . $mail->ErrorInfo . "\n";
    }
}

// Main function to orchestrate the process
function automateWeeklyReports() {
    $reports = generateWeeklyReports();

    // Define recipient email addresses for each department
    $departmentEmails = [
        'Sales' => 'sales.department@company.com',
        'Human Resources' => 'hr.department@company.com',
        'IT Support' => 'it.support@company.com',
        'Finance' => 'finance.department@company.com'
    ];

    foreach ($reports as $department => $report) {
        if (isset($departmentEmails[$department])) {
            $recipient = $departmentEmails[$department];
            $subject = "Weekly Report: $department Department";
            sendEmail($recipient, $subject, $report);
        }
    }
}

// Execute the automation process
automateWeeklyReports();
?>