#!/usr/bin/env bash
# Set your current working directory to the root of your project
pwd
echo "Verifying header file closure for the following header files"
find ./ -type f \( -name "*.hpp" -o -name "*.h" \) -printf "  %P\n"

# Considered and rejected the -fsyntax_only option.  I wanted to give the liker a chance to discover undefined references
# Added the header file twice so include guards will be verified
(find ./ -type f \( -name "*.hpp" -o -name "*.h" \) -printf "#include \"%P\"\n#include \"%P\"\n"; echo "int main(){}") | \
g++ -x c++                                      \
    -Wall -Wextra -pedantic -Werror             \
    -Wdelete-non-virtual-dtor                   \
    -Wduplicated-branches                       \
    -Wduplicated-cond                           \
    -Wextra-semi                                \
    -Wfloat-equal                               \
    -Winit-self                                 \
    -Wlogical-op                                \
    -Wnoexcept                                  \
    -Wshadow                                    \
    -Wnon-virtual-dtor                          \
    -Wold-style-cast                            \
    -Wstrict-null-sentinel                      \
    -Wsuggest-override                          \
    -Wswitch-default                            \
    -Wswitch-enum                               \
    -Woverloaded-virtual                        \
    -Wuseless-cast                              \
    -g0 -O3 -DNDEBUG -pthread -std=c++20 -I./   \
    -DUSING_TOMS_SUGGESTIONS                    \
    -D__func__=__PRETTY_FUNCTION__              \
    -o /dev/null  -

#    -Wzero-as-null-pointer-constant             \


Status=$?
if [[ $Status == 0 ]]; then
    echo -e "\nSuccess:  Header file closure compiles clean\n"
else
   echo -e "\nFATAL: compile terminated with errors\n"
fi
