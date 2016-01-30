# Beagleplayer 0.1.6
An audio, video, radio, playlist, client. Beagleplayer is ideal for: quick, easy, browsing of your media library.

## Table of contents

- [Dependencies](#dependencies)
- [Included](#included)
- [Compile and Run](#compile-and-run)
- [PPA Installation](#ppa-install)
- [Themes](#themes)
- [Theme Examples](#theme-examples)
- [Contributing](#contributing)
- [Creators](#creators)
- [License](#license)

##Dependencies

```bash
# Recommended with Qt5
$ sudo apt-get install qt5-default libqt5webkit5-dev
```
This was built/tested in Ubuntu 14.04 aswell as a beta version available for [Windows 10](https://github.com/hutchgrant/beagleplayer/blob/master/README-windows.md). Debian based distributions should be able to run without issue.  Minor modifications necessary, to cache path, for other Linux based systems.

##Included
```
beagleplayer/
├── res/
│   ├── html
│   │   ├── bootstrap/jquery/jqueryui
│   ├── themes
│   │   ├── default
│   ├── BPres.qrc
│   └──  *_icons.png
└── src/
│   ├── cache
│   ├── control
│   ├── html5applicationviewer
│   ├── object
│   └── widgets
├── debian/
└── docs/

#themes(html,js,css) for detached player and icons + assets for player, all in ./res
#src files for Qt(c++) in ./src
```

##Compile and Run
- download and extract beagleplayer.v.0.1.5.tar.gz 
```
wget https://github.com/hutchgrant/beagleplayer/archive/master.zip
cd ~/Downloads
tar -zxvf beagleplayer*.tar.gz 
```
- Or install git and clone
```
sudo apt-get install git
git clone https://github.com/hutchgrant/beagleplayer.git
```
- Compile
```
cd ./beagleplayer
qmake 
make
sudo make install
```
- Run the compiled binary with
```
beagleplayer
```

##PPA Install

If you're using Ubuntu 14.04(Trusty) or Ubuntu 14.10(Utopic) you can install the launchpad ppa with:
```
sudo add-apt-repository ppa:hutchgrant/beagleplayer && sudo apt-get update && sudo apt-get install beagleplayer
```

##Themes

Themes can be built by creating a new folder in your ~/.cache/res/themes/ directory call it yourtheme.

A theme consist of: 

* yourtheme.theme 
* yourtheme.html
* yourtheme.css
* yourtheme.png

###Theme Examples

- For an example of a .theme file see the default themes [./res/themes/default](https://github.com/hutchgrant/beagleplayer/blob/master/res/themes/default/default.theme)

- For an example of a .html and .css theme check the [default](https://github.com/hutchgrant/beagleplayer/blob/master/res/themes/default/default.theme), [blue](https://github.com/hutchgrant/beagleplayer/blob/master/res/themes/blue/blue.html), [grey](https://github.com/hutchgrant/beagleplayer/blob/master/res/themes/blue/grey.html)themes.

##Contributing

see [Contributing](https://github.com/hutchgrant/beagleplayer/blob/master/CONTRIBUTING.md)

## Creators
**Grant Hutchinson**

## License
All code and documentation from 2011- 2015 is available under the terms of the [GNU Public License v3](http://www.gnu.org/copyleft/gpl.html)

