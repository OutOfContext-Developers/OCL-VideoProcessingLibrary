#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundStream.hpp>

#include <OCL-VideoProcessingLibrary/frame.hpp>
#include <OCL-VideoProcessingLibrary/vframe.hpp>
#include <OCL-VideoProcessingLibrary/aframe.hpp>

#include <cassert>
#include <queue>


int main()
{
    char const * url = "path/to/your/video/file";

    if(url == NULL) {
      std::cerr << "No File selected" << '\n';
      return 0;
    }

    // Objects to extract video and audio frames
    VideoFrame *videoframe = new VideoFrame(url);
    AudioFrame *audioframe = new AudioFrame(url);


    // Create the main window
    sf::RenderWindow window(sf::VideoMode(videoframe->width, videoframe->height), "SFML window");

    // Create a texture to display video
    sf::Texture texture;
    texture.create(videoframe->width, videoframe->height);

    // Load a sprite to display
    sf::Sprite sprite;

    // Pointer to store rgba frame information
    unsigned char* pixelData;
    // Presentation time stamp
    int64_t pts;
    // Presentation time in seconds
    double presentationTime = -1;

    // Sound type object to play sound
    sf::Sound sound;
    // SoundBuffer object to update sound samples when done playing
    sf::SoundBuffer buffer;

    // Clock and time objects used for Syncronization
    sf::Clock clock;
    clock.restart();
    sf::Time elapsed;

    // queue<int16_t*> AudioQueue;


    while (window.isOpen())
    {
        // Get elapsed time
        elapsed = clock.getElapsedTime();



        // Get sound if previous sound was done playing
        if(sound.getStatus()!=sf::Sound::Playing) {

          // Update audio frame
          audioframe->update();
          int16_t* audioData;

          // Get samples
          // AudioFrame class returns no of samples = sampleRate
          audioData = (int16_t*)audioframe->getFrame().first;

          // Load audio data into audio buffer
          buffer.loadFromSamples(audioData, audioframe->sampleRate, audioframe->channelCount, audioframe->sampleRate);

          sound.setBuffer(buffer);
          sound.play();

          // free the memory
          free(audioData);
        }

        // If time of current frame is up present next frame
        if(elapsed.asSeconds()>presentationTime) {
          // Update video frame
          videoframe->update();

          // extract pts and pixel data from frame
          auto fullData = videoframe->getFrame();
          pixelData = (unsigned char*)fullData.first;
          pts = fullData.second;

          // Update texture update sprite and free memory
          texture.update(pixelData);
          sprite.setTexture(texture);
          free(pixelData);
        }

        if(pts==-1) {
          break;
        }


        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Clear screen
        window.clear();
        // Draw the sprite
        window.draw(sprite);
        // Update the window
        window.display();

        // Calculate presentation time of current frame
        presentationTime = pts * (double)videoframe->time_base.num / (double)videoframe->time_base.den;
        // sf::sleep(sf::seconds(presentationTime - elapsed.asSeconds()));
    }

    while(sound.getStatus() == sf::Sound::Playing) {
      sf::sleep(sf::milliseconds(100));
    }
    delete(videoframe);
    delete(audioframe);
    return EXIT_SUCCESS;

    return 0;
}
