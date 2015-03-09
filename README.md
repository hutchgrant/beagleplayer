# Beagleplayer 0.1.4
An audio, video, radio, playlist, client, for mplayer. Beagleplayer is ideal for: quick, easy, browsing of your media library.

## Table of contents

- [Dependencies](#dependencies)
- [Included](#included)
- [Compile and Run](#compile-and-run)
- [Contributing](#contributing)
- [Creators](#creators)
- [License](#license)


##Dependencies

Recommend with Qt5
```
$ sudo apt-get install mplayer libqt5network5 libsqlite3-dev qtbase5-dev qt5-qmake libqt5opengl5-dev g++ qt5-default libqt5webkit5 libqt5webkit5-dev

with Qt4
$  sudo apt-get install mplayer libqt4-dev libqt4-opengl libqt4-opengl-dev libqt4-network libqt4-sql libqt4-sql-sqlite gcc g++ make qt4-qmake libqt4-webkit
```
* This was built and tested in Ubuntu 14.04. Debian based distributions should be able to run without issue.  Minor modifications necessary, to cache path, for other Linux based systems.

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
│   ├── qmpwidget
│   └── widgets
├── debian/
└── docs/

#themes(html,js,css) for detached player and icons + assets for player, all in ./res
#src files for Qt(c++) in ./src
```

##Compile and Run
1. download and extract beagleplayer.v.0.1.4.tar.gz 
```bash
$ wget https://github.com/hutchgrant/beagleplayer/archive/master.zip
$ cd ~/Downloads
$  tar -zxvf beagleplayer*.tar.gz 
```
2. Or install git and clone
```bash
$ sudo apt-get install git
$ git clone https://github.com/hutchgrant/beagleplayer.git
```
3. Compile
```bash
 $ cd ./beagleplayer
 $ qmake 
 $ make
```
4. Run the compiled binary with
```bash
 $ ./beagleplayer
```
5. Recommended you link it in your /usr/bin folder.
```
$ sudo ln -s ./beagleplayer /usr/bin/beagleplayer
```

##Themes

Themes can be built by creating a new folder in the ./res/themes/ directory call it yourtheme.

A theme consist of: 

* yourtheme.theme 
* yourtheme.html
* yourtheme.css
* yourtheme.png

For an example of a .theme file see the default theme
For an example of a .html and .css theme check the [default](https://github.com/hutchgrant/beagleplayer/blob/master/res/themes/default/default.theme) or [blue](https://github.com/hutchgrant/beagleplayer/blob/master/res/themes/blue/blue.html) themes.

##Contributing

Contributions are encouraged, while it must be noted, this project has a planned path and you should seek to contact the authors, before undergoing any large development.  All contributions will be placed under the same GPLv3 license, contributers must agree to that license.

The following is expected from any pull requests.

1. [Fork](http://help.github.com/fork-a-repo/) the project, clone your fork,
   and configure the remotes:

   ```bash
   # clone your fork
   git clone https://github.com/<your-username>/beagleplayer.git
   # change directory to the repository folder
   cd beagleplayer
   # set the official beagleplayer repo to a remote branch called "upstream"
   git remote add upstream https://github.com/hutchgrant/beagleplayer.git
   ```

2. If you cloned a while ago, make sure to update with the latest changes from upstream branch:

   ```bash
   git checkout master
   git pull upstream master
   ```

3. Create a new branch for your changes/fixes

   ```bash
   git checkout -b <topic-branch-name>
   ```

4. Locally merge (or rebase) the upstream development branch into your topic branch:

   ```bash
   git pull [--rebase] upstream master
   ```

6. Push your topic branch up to your fork:

   ```bash
   git push origin <topic-branch-name>
   ```

7. [Open a Pull Request](https://help.github.com/articles/using-pull-requests/)
    with a clear title and description against the `master` branch.

## Creators
**Grant Hutchinson**


## License
All code and documentation from 2011- 2015 is available under the terms of the [GNU Public License v3](http://www.gnu.org/copyleft/gpl.html)

