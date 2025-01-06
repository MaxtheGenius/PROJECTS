import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
import datetime
import requests
import pandas as pd
import matplotlib.pyplot as plt
from io import BytesIO

# Step 1: Fetch website performance data (e.g., from an API or scraping)
def fetch_performance_data():
    """
    Fetch website performance data (simulated API call for demo purposes).
    Ideally, you would use an API like Google Analytics or scrape data from the website.
    """
    # Simulated data for this example
    data = {
        'Date': ['2023-12-01', '2023-12-02', '2023-12-03', '2023-12-04', '2023-12-05'],
        'Page Views': [500, 600, 450, 700, 650],
        'Bounce Rate (%)': [50, 48, 55, 45, 49],
        'Avg. Session Duration (min)': [3.5, 4.0, 3.0, 3.8, 3.6],
    }
    
    df = pd.DataFrame(data)
    df['Date'] = pd.to_datetime(df['Date'])
    
    return df

# Step 2: Generate a performance report (weekly summary)
def generate_report(df):
    """
    Generate a weekly report from the performance data.
    """
    # Weekly report generation (group by week)
    df.set_index('Date', inplace=True)
    weekly_report = df.resample('W').sum()

    # Plot performance data
    fig, ax = plt.subplots(figsize=(10, 6))
    df['Page Views'].plot(kind='line', ax=ax, color='blue', label='Page Views')
    df['Bounce Rate (%)'].plot(kind='line', ax=ax, color='red', label='Bounce Rate (%)')
    ax.set_title('Website Performance - Weekly Overview')
    ax.set_ylabel('Page Views / Bounce Rate (%)')
    ax.set_xlabel('Date')
    ax.legend()

    # Save plot as image
    img = BytesIO()
    plt.savefig(img, format='png')
    img.seek(0)
    
    return weekly_report, img

# Step 3: Send the performance report via email
def send_email(subject, body, to_emails, attachment=None):
    """
    Send an email with the performance report.
    """
    # Email server settings
    smtp_server = "smtp.gmail.com"
    smtp_port = 587
    sender_email = "youremail@gmail.com"  # Change to your email
    sender_password = "yourpassword"  # Change to your email password

    # Create message container
    msg = MIMEMultipart()
    msg['From'] = sender_email
    msg['To'] = ", ".join(to_emails)
    msg['Subject'] = subject

    # Attach the body with the message
    msg.attach(MIMEText(body, 'plain'))

    # Attach the report image
    if attachment:
        from email.mime.image import MIMEImage
        image = MIMEImage(attachment.read())
        image.add_header('Content-ID', '<report_image>')
        msg.attach(image)
    
    # Connect to the email server and send the email
    try:
        with smtplib.SMTP(smtp_server, smtp_port) as server:
            server.starttls()
            server.login(sender_email, sender_password)
            server.sendmail(sender_email, to_emails, msg.as_string())
            print(f"Email sent to: {', '.join(to_emails)}")
    except Exception as e:
        print(f"Error sending email: {str(e)}")

# Step 4: Main function to automate the entire process
def main():
    # Fetch website performance data
    df = fetch_performance_data()

    # Generate the weekly report
    weekly_report, img = generate_report(df)

    # Prepare the email body
    current_date = datetime.datetime.now().strftime('%Y-%m-%d')
    subject = f"Weekly Website Performance Report - {current_date}"
    body = f"Dear Team,\n\nPlease find the attached weekly performance report for the website.\n\nBest Regards,\nWebsite Analytics Team"

    # List of team email addresses
    to_emails = ["team1@example.com", "team2@example.com", "team3@example.com"]

    # Send the email with the report image as an attachment
    send_email(subject, body, to_emails, attachment=img)

if __name__ == "__main__":
    main()