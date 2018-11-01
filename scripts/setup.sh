#!/bin/bash

# exit script on any error
set -e
set -o xtrace

build=$(pwd)/..
cd $build

function github_clone {
	if [ -d $2 ]
	then
		cd $2
		git pull
	else
		git clone https://github.com/$1/$2.git
	fi
	cd $build
}

# update or clone dependent projects
github_clone bebuch big

github_clone bebuch bitmap
