# SinhalaOombi

This repository contains the original source of the Sinhala Oombi game for Windows. 

![screen](https://cloud.githubusercontent.com/assets/3455428/22137643/43899984-df02-11e6-97cb-df2378f87724.png)

## History
This was written back in 2007/8 while I was waiting for the university admission. I was still learning C/C++ and then I found this 
classic book on Windows Programming, "Programming Windows" by Charles Petzold. http://www.charlespetzold.com/pw5/index.html 
The book has been a great inspiration and "SinhalaOombi" has started even before finishing it. 

## Compiling 
The original source in the master branch was compiled using Microsoft Visual Studio 2001. 
Development branch uses CMake build system. To generate sources and build, use following commands.

```
  cd <SourceDir>
  mkdir build
  cd build
  cmake ..
  cmake --build .
```

[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/1823/badge)](https://bestpractices.coreinfrastructure.org/projects/1823)
