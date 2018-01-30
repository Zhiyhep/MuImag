#!/bin/bash
#CreateDict.sh source header linkdef

echo `pwd`
echo "usage: CreateDict.sh source header linkdef"
MyClass_src=$1
MyClass_head=$2
linkdef=$3
classname=${MyClass_src%.*}
classname=${classname##*/}
MyClass_dict=${classname}"Dict.cc"
MyClass_dict_so="lib"${classname}".so"
MyClass_dict_o=${classname}"Dict.o"

`root-config --cxx --cflags` -fPIC -c $MyClass_src
rootcint -f $MyClass_dict -c -p $MyClass_head $linkdef
`root-config --cxx --cflags` -fPIC -c $MyClass_dict
`root-config --cxx --cflags` -fPIC -shared -o $MyClass_dict_so $MyClass_dict_o
