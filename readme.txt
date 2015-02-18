beagleplayer 0.1.3 Release instructions

An Audio, Video, Radio, Playlist client for mplayer. Beagleplayer is ideal for quick easy browsing of your media library.


dependencies:

Recommend with Qt5
$ sudo apt-get install mplayer libqt5network5 libsqlite3-dev qtbase5-dev qt5-qmake libqt5opengl5-dev

with Qt4
$  sudo apt-get install mplayer libqt4-dev libqt4-opengl libqt4-opengl-dev libqt4-network libqt4-sql libqt4-sql-sqlite gcc g++ make qt4-qmake

----------------------
download and extract:
----------------------
download beagleplayer.v.0.1.3.tar.gz 
 $ cd ~/Downloads
 $  tar -zxvf beagleplayer*.tar.gz 
----
or ----
----
install git and ...
$ git clone https://github.com/hutchgrant/beagleplayer.git

-----------------
compile and run:
-----------------
 $ cd ./beagleplayer
 $ qmake 
 $ make
 $ sudo make install

 link it with:
 $ sudo ln -fs /opt/extras.ubuntu.com/beagleplayer/beagleplayer /usr/bin/beagleplayer
 
 Finally run the binary with: 
 $ beagleplayer 

Cache default path is ~/.cache/beagleplayer2
Move cache location by editing ~/.cache/beagleplayer/cache_locate
Reset/remove cache by deleting contents of ~/.cache/beagleplayer2 cache default path.


