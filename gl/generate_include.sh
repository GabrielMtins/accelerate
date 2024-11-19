#!/bin/sh

rm -f include/acc_gl.hpp
touch include/acc_gl.hpp

echo "#ifndef ACC_GL_HPP" >> include/acc_gl.hpp
echo "#define ACC_GL_HPP" >> include/acc_gl.hpp
echo >> include/acc_gl.hpp

find include/ | grep ".hpp" | grep -v "acc_gl.hpp" | sed "s/acc_gl\.hpp//g" | sed "s/include\//\#include \"/g" | sed "s/hpp/hpp\"/g" >> include/acc_gl.hpp

echo >> include/acc_gl.hpp
echo "#endif" >> include/acc_gl.hpp
