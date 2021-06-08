# OCL-VideoProcessingLibrary

[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]


<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project


This is a C++ library to read video data as video and audio frames. If you are looking some library to process video data into numbers for your AI library of video editing software this might be your goto library. You clone the source code and extend this into a new project, currently if supports only video and audio frame extraction for a video file.

Project classes:
* Frame - A base class which all classes inherit. (This opens a video file container and finds the index of stream you need)
* Synchronizer - This keeps and manages all the frame data along with their presentation time stamps. (This is the class whose member functions you will access to extract frames)
* VideoFrame and AudioFrame - As the name suggest the extract respective frames.



### Built With

This is the list of build tools you will need and the frameworks I have used.
* [CMake](https://cmake.org/download/)
* [ffmpeg](http://ffmpeg.org/download.html)


<!-- GETTING STARTED -->
## Getting Started

Follow these instruction to set this library up and running

### Prerequisites

First install cmake if not already present.
* CMake
  ```sh
  sudo apt install cmake
  ```

### Installation

1. Clone the repo
   ```sh
   git clone --recurse-submodules https://github.com/OutOfContext-Developers/OCL-VideoProcessingLibrary.git
   ```
2. Also before compiling this library make sure you have installed ffmpeg. To install ffmpeg, run script run.sh in FFMPEG/
   ```sh
   cd lib/FFMPEG
   chmod +x ./run.sh
   sudo ./run.sh
   ```
2. Make build directory and go inside build directory
   ```sh
   mkdir build && cd build
   ```
3. Configure
   ```sh
   cmake ..
   ```
4. Build
   ```sh
   make
   ```
5. Install
   ```sh
   sudo make install
   ```



<!-- USAGE EXAMPLES -->
## Usage

This is a library, hence you are supposed to use it in your C++ programs.

_For examples, please refer to the [Examples](https://github.com/OutOfContext-Developers/OCL-VideoProcessingLibrary/tree/main/examples)_


<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.



<!-- CONTACT -->
## Contact

Project Link: [https://github.com/OutOfContext-Developers/OCL-VideoProcessingLibrary](https://github.com/OutOfContext-Developers/OCL-VideoProcessingLibrary)



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=for-the-badge
[license-url]: https://github.com/OutOfContext-Developers/OCL-VideoProcessingLibrary/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://www.linkedin.com/in/akshat-dixit-15b284186/
