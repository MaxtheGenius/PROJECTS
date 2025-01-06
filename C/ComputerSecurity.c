#include <stdio.h>
#include <string.h>

// Function to encrypt a message using Caesar cipher
void encrypt(char *message, int shift)
{
    for (int i = 0; message[i] != '\0'; i++)
    {
        // Check if the character is an uppercase letter
        if (message[i] >= 'A' && message[i] <= 'Z')
        {
            message[i] = ((message[i] - 'A' + shift) % 26) + 'A';
        }
        // Check if the character is a lowercase letter
        else if (message[i] >= 'a' && message[i] <= 'z')
        {
            message[i] = ((message[i] - 'a' + shift) % 26) + 'a';
        }
        // Non-alphabetical characters remain unchanged
    }
}

// Function to decrypt a message using Caesar cipher
void decrypt(char *message, int shift)
{
    for (int i = 0; message[i] != '\0'; i++)
    {
        // Check if the character is an uppercase letter
        if (message[i] >= 'A' && message[i] <= 'Z')
        {
            message[i] = ((message[i] - 'A' - shift + 26) % 26) + 'A';
        }
        // Check if the character is a lowercase letter
        else if (message[i] >= 'a' && message[i] <= 'z')
        {
            message[i] = ((message[i] - 'a' - shift + 26) % 26) + 'a';
        }
        // Non-alphabetical characters remain unchanged
    }
}

int main()
{
    char message[100];
    int shift;

    // Get the message from the user
    printf("Enter the message to encrypt: ");
    fgets(message, sizeof(message), stdin); // Using fgets to allow spaces
    message[strcspn(message, "\n")] = '\0'; // Remove newline character if present

    // Get the shift value from the user
    printf("Enter the shift value: ");
    scanf("%d", &shift);

    // Encrypt the message
    encrypt(message, shift);
    printf("Encrypted message: %s\n", message);

    // Decrypt the message back to original
    decrypt(message, shift);
    printf("Decrypted message: %s\n", message);

    return 0;
}