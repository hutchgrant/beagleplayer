beagleplayer 0.1.0 Release instructions

An Audio, Video, Radio, Playlist client for mplayer. Beagleplayer is ideal for quick easy browsing of your media library.


dependencies:

$  sudo apt-get install mplayer libqt4-dev libqt4-opengl libqt4-opengl-dev libqt4-network libqt4-sql libqt4-sql-sqlite

download and extract:

download beagletomb.v.0.1.0.tar.gz 
 $ cd ~/Downloads
 $  tar -zxvf beagleplayer*.tar.gz 
 $ sudo mkdir /opt/extras.ubuntu.com
 $ sudo cp -R beagleplayer /opt/extras.ubuntu.com/
 $ chmod 777 -R /opt/extras.ubuntu.com/beagleplayer
----
or ----
----
install git and ...
git clone https://hutchgrant@github.com/hutchgrant/beagleplayer.git
 $ sudo mkdir /opt/extras.ubuntu.com
 $ sudo cp -R beagleplayer /opt/extras.ubuntu.com/
 $ chmod 777 -R /opt/extras.ubuntu.com/beagleplayer



compile and run
 $ cd /opt/extras.ubuntu.com/beagletomb
 $ qmake 
 $ make

 link it with:
 $ ln -fs /opt/extras.ubuntu.com/beagletomb/beagletomb /usr/bin/beagletomb
 
 Finally run the binary with: 
 $ beagletomb 

On first open you will be asked to enter your server credentials. Make sure the mediatomb server initialized port and server bound ip matches the address you enter into the beagletomb preference window.

The defaults are:

user: mediatomb pass: mediatomb table: mediatomb SqlDir?: BTmedia.db


