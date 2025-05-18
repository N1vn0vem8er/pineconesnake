# Music Player

Music player with playlists.

## Table of contents

* [General info](#general-info)
* [Features](#features)
* [Technologies](#technologies)
* [Setup](#setup)

## General info

PCS Music Player is a simple music player with playlists. It automatically searches for music files in user defined directories.

## Features

* Auto index music
* Create playlists
* Save playlists
* Sort music into categories

## Technologies

This project was created with:

* Qt version: >4
* SQLite3 version: >=3.46.1
* TagLib version: 2.0.2

## Setup

```
$ mkdir -p build
$ cd build
$ qmake ../src/pcsmusicplayer.pro
$ make
$ make install
```
