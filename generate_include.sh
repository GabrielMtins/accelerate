#!/bin/sh

rm -f include/acc.hpp
touch include/acc.hpp

echo "#ifndef ACC_HPP" >> include/acc.hpp
echo "#define ACC_HPP" >> include/acc.hpp
echo >> include/acc.hpp

find include/ | grep ".hpp" | grep -v "acc.hpp" | sed "s/acc\.hpp//g" | sed "s/include\//\#include \"/g" | sed "s/hpp/hpp\"/g" >> include/acc.hpp

echo >> include/acc.hpp
echo "#endif" >> include/acc.hpp
