/*
    Cross-Platform Audio Editing Application in C++
    This program uses Qt for the GUI and allows basic audio processing such as loading,
    playing, trimming, and adjusting the volume of audio files.
*/

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QVBoxLayout>
#include <QSlider>
#include <QFileInfo>
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QAudioFormat>
#include <QBuffer>

using namespace std;

class AudioEditor : public QWidget {
    Q_OBJECT

private:
    QPushButton* loadButton;           // Button to load an audio file
    QPushButton* playButton;           // Button to play audio
    QPushButton* stopButton;           // Button to stop audio
    QSlider* volumeSlider;             // Slider to control the volume
    QLabel* statusLabel;               // Label to show the current status (e.g., playing)
    QMediaPlayer* mediaPlayer;         // Media player for playing audio
    QString currentFilePath;          // Path to the loaded audio file

public:
    // Constructor to set up the UI and initialize components
    AudioEditor(QWidget* parent = nullptr) : QWidget(parent) {
        // Set the title of the window
        setWindowTitle("Audio Editor");

        // Create the buttons and slider
        loadButton = new QPushButton("Load Audio", this);
        playButton = new QPushButton("Play", this);
        stopButton = new QPushButton("Stop", this);
        volumeSlider = new QSlider(Qt::Horizontal, this);
        statusLabel = new QLabel("Status: No audio loaded", this);

        // Set the volume slider range (0-100)
        volumeSlider->setRange(0, 100);
        volumeSlider->setValue(50); // Default volume is 50%

        // Initialize the media player for playing audio
        mediaPlayer = new QMediaPlayer(this);

        // Set up the layout for the interface
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(loadButton);
        layout->addWidget(playButton);
        layout->addWidget(stopButton);
        layout->addWidget(volumeSlider);
        layout->addWidget(statusLabel);

        // Connect the buttons and slider to their respective functions
        connect(loadButton, &QPushButton::clicked, this, &AudioEditor::loadAudio);
        connect(playButton, &QPushButton::clicked, this, &AudioEditor::playAudio);
        connect(stopButton, &QPushButton::clicked, this, &AudioEditor::stopAudio);
        connect(volumeSlider, &QSlider::valueChanged, this, &AudioEditor::adjustVolume);

        // Set up the window size
        setFixedSize(300, 250);
    }

public slots:
    // Slot to load an audio file
    void loadAudio() {
        currentFilePath = QFileDialog::getOpenFileName(this, "Open Audio File", "", "Audio Files (*.mp3 *.wav *.ogg)");
        
        if (currentFilePath.isEmpty()) {
            statusLabel->setText("Status: No audio loaded");
            return;
        }

        mediaPlayer->setSource(QUrl::fromLocalFile(currentFilePath));
        statusLabel->setText("Status: Audio loaded");

        // Optionally, load and process the waveform here (or display waveform if desired)
        qDebug() << "Audio loaded from: " << currentFilePath;
    }

    // Slot to play the loaded audio
    void playAudio() {
        if (currentFilePath.isEmpty()) {
            statusLabel->setText("Status: No audio loaded");
            return;
        }

        mediaPlayer->play();
        statusLabel->setText("Status: Playing audio");

        qDebug() << "Playing audio from: " << currentFilePath;
    }

    // Slot to stop the audio playback
    void stopAudio() {
        if (mediaPlayer->state() == QMediaPlayer::PlayingState) {
            mediaPlayer->stop();
            statusLabel->setText("Status: Audio stopped");
        }
    }

    // Slot to adjust the volume of the audio
    void adjustVolume(int value) {
        mediaPlayer->setVolume(value);
    }

    // Destructor to clean up
    ~AudioEditor() {
        delete mediaPlayer;
        delete loadButton;
        delete playButton;
        delete stopButton;
        delete volumeSlider;
        delete statusLabel;
    }
};

// Main function to run the application
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Create an instance of AudioEditor and show the GUI
    AudioEditor editor;
    editor.show();

    return app.exec();
}