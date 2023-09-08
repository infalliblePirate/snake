#include <termios.h>
#include <unistd.h>

// Store the original terminal settings for restoration later
struct termios originalSettings;

// Enable non-canonical input mode (shouldn't press enter to pass data)
void enableRawInputMode() {
    // Get the current terminal settings and store them as originalSettings
    tcgetattr(STDIN_FILENO, &originalSettings);

    // Create a new settings struct based on the original settings
    struct termios newSettings = originalSettings;

    // Modify the new settings for non-canonical input
    newSettings.c_lflag &= ~(ICANON | ECHO); // Disable line buffering and echoing (data isn't stored in memory and input is not repeated)
    newSettings.c_cc[VTIME] = 0; // Set timeout between characters to 0 (no timeout)
    newSettings.c_cc[VMIN] = 1;  // Set minimum characters to read to 1

    // Apply the new settings immediately
    tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);
}

// Restore the original terminal settings
void restoreOriginalSettings() {
    // Restore the original terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &originalSettings);
}
