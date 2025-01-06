import java.security.SecureRandom;
import java.util.Scanner;
import javax.mail.*;
import javax.mail.internet.*;
import java.util.Properties;
import java.time.LocalDateTime;
import java.time.temporal.ChronoUnit;

/*
 * TwoFactorAuthentication.java
 * This Java application implements a basic two-factor authentication (2FA) system for an enterprise web application.
 * The system sends a one-time password (OTP) to the user's email and verifies it within a time limit.
 */
public class TwoFactorAuthentication {

    // Store the generated OTP and timestamp for expiration verification
    private static String generatedOTP;
    private static LocalDateTime otpTimestamp;

    // Generate a random 6-digit OTP
    private static String generateOTP() {
        SecureRandom random = new SecureRandom();
        int otp = 100000 + random.nextInt(900000); // Generate a random 6-digit OTP
        return String.valueOf(otp);
    }

    // Send OTP to the user's email address
    private static void sendOTP(String email, String otp) {
        String host = "smtp.gmail.com"; // SMTP server for Gmail
        String from = "your_email@gmail.com"; // Sender's email
        String password = "your_email_password"; // Sender's email password (use app password if 2FA enabled)

        // Set up email properties
        Properties properties = new Properties();
        properties.put("mail.smtp.host", host);
        properties.put("mail.smtp.port", "587");
        properties.put("mail.smtp.auth", "true");
        properties.put("mail.smtp.starttls.enable", "true");

        // Get the Session object
        Session session = Session.getInstance(properties, new Authenticator() {
            protected PasswordAuthentication getPasswordAuthentication() {
                return new PasswordAuthentication(from, password);
            }
        });

        try {
            // Create the email message
            MimeMessage message = new MimeMessage(session);
            message.setFrom(new InternetAddress(from));
            message.setRecipient(Message.RecipientType.TO, new InternetAddress(email));
            message.setSubject("Your One-Time Password (OTP) for 2FA");
            message.setText("Your OTP is: " + otp + "\nThis OTP is valid for 5 minutes.");

            // Send the email
            Transport.send(message);
            System.out.println("OTP sent to: " + email);
        } catch (MessagingException e) {
            e.printStackTrace();
        }
    }

    // Verify if the entered OTP matches the generated OTP and is within the
    // validity period (5 minutes)
    private static boolean verifyOTP(String enteredOTP) {
        if (generatedOTP == null || otpTimestamp == null) {
            return false; // No OTP generated yet
        }

        // Check if the OTP has expired
        long minutesPassed = ChronoUnit.MINUTES.between(otpTimestamp, LocalDateTime.now());
        if (minutesPassed > 5) {
            System.out.println("The OTP has expired.");
            return false;
        }

        // Verify if the entered OTP matches the generated OTP
        return generatedOTP.equals(enteredOTP);
    }

    // Simulate the 2FA process
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        // Request the user's email
        System.out.println("Enter your email address for 2FA:");
        String email = scanner.nextLine();

        // Generate an OTP
        generatedOTP = generateOTP();
        otpTimestamp = LocalDateTime.now(); // Record the time when the OTP is generated

        // Send the OTP to the user's email
        sendOTP(email, generatedOTP);

        // Request the user to enter the OTP
        System.out.println("Enter the OTP sent to your email:");

        // Capture the entered OTP
        String enteredOTP = scanner.nextLine();

        // Verify the OTP
        if (verifyOTP(enteredOTP)) {
            System.out.println("Authentication successful.");
        } else {
            System.out.println("Invalid or expired OTP. Authentication failed.");
        }

        scanner.close();
    }
}