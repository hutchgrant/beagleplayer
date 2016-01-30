# Beagleplayer 0.1.6
An audio, video, radio, playlist, client. Beagleplayer is ideal for: quick, easy, browsing of your media library.

## Table of contents

- [Dependencies](#dependencies)
- [Included](#included)
- [Compile and Run](#compile-and-run)
- [Themes](#themes)
- [Theme Examples](#theme-examples)
- [Contributing](#contributing)
- [Creators](#creators)
- [License](#license)

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

##Compile and Run

1. Install Codecs http://www.codecguide.com/download_kl.htm

2. Install Qt community edition.

3. Install Git

4. Open Git cmdline type: 
```
cd Documents 
git clone https://github.com/hutchgrant/beagleplayer.git
```
5. Open QtCreator -> Open Project -> browse C:\User\youruser\Documents\beagleplayer, open the beagleplayer.pro file

6. Compile and run.

7. Database will be created at C:\User\youruser\beagleplayer\beagle.db

8. You need to manually copy the entire 'res' folder 
```
Copy
from: C:\User\youruser\Documents\beagleplayer\res
to: C:\User\youruser\beagleplayer\res
```
This is so beagleplayer can use the custom themes.

9. Enjoy.


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
