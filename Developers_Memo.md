# Developers Memo #
This page is a memo for developer who aims to compile GPSBook.

## GPSBook ##
> GPSBook has been developed using QT framework and qtcreator IDE.

> For ubuntu user qtcreator and qt framework can be installed using the command
```
  sudo apt-get install qt4-default qt-sdk qtcreator
```

## Plugins ##
To compile the plugins, it is required to install the following libraries:

### libcurl and libzzip for displayStatistics and displaySRTM ###
```
  sudo apt-get install curl libcurl4-openssl-dev
```

```
  sudo apt-get install libzzip-dev
```

### qwt 6.0 for displayGraphic2D ###
```
  sudo apt-get install libqwt6 libqwt-dev
```

### libQGLViewer 2.3.6 for displayGraphic3D ###
```
  sudo apt-get install libqglviewer-qt4-dev
```

### libqscintilla2 for displyRawEditor ###
```
  sudo apt-get install libqscintilla2-dev
```