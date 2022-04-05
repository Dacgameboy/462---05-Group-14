#!/usr/bin/env bash
#
# Script file to compile all C++ source files in or under the
# current directory.  This has been used in the OpenSUSE and Ubuntu
# environments with the GCC and Clang compilers and linkers
executableFileName="${1:-project}"


### Check GCC and Clang versions on Tuffix, and upgrade if needed - Usually a one-time occurrence
###  The procedure should be removed once Tuffix is configured out-of-the-box with correct versions
CheckVersion()
{
  RequiredGccVersion=11
  RequiredClangVersion=12

  buffer=( $(lsb_release -d ) )
  Distribution="${buffer[1]}"

  buffer=( $(lsb_release -r ) )
  Release="${buffer[1]}"

  #  See Parameter Expansion section of Bash man page for "%%"'s' Remove matching suffix pattern
  #  behavior (https://linux.die.net/man/1/bash)
  #
  #  ${parameter,,}     ==> lower case
  #  ${parameter^^}     ==> upper case
  #  ${parameter%word}  ==> Remove matching suffix pattern (shortest matching pattern)
  #  ${parameter%%word} ==> Remove matching suffix pattern (longest matching pattern)

  buffer=( $(g++ --version ) )
  gccVersion="${buffer[3]%%.*}"

  # This is pretty fragile, but version 10 and 12 display the version slightly differently
  buffer=( $(clang++ --version ) )
  if   [[ ${buffer[1],,} = "version" ]]; then  clangVersion="${buffer[2]%%.*}"
  elif [[ ${buffer[2],,} = "version" ]]; then  clangVersion="${buffer[3]%%.*}"
  fi

  if [[ "${Distribution,,}" = "ubuntu"  &&  "${Release,,}" = "20.04" ]]; then
    if [[ "${gccVersion,,}" -lt "${RequiredGccVersion,,}"  ||  "${clangVersion,,}" -lt "${RequiredClangVersion,,}" ]]; then

      echo -e "\nGCC version ${RequiredGccVersion} and Clang version ${RequiredClangVersion} are required, but you're using GCC version ${gccVersion} and Clang version ${clangVersion}\n\nWould like to upgrade now?  This may require a system reboot. (yes or no)"
      read shall_I_upgrade

      if [[ "${shall_I_upgrade,,}x" = "yesx"  ||  "${shall_I_upgrade,,}x" = "yx" ]]; then
        echo -e "\nUpgrading could be a long and extensive process.\n\n ****  Make sure you have backups of all your data!\n\n Are you really sure?"
        read shall_I_upgrade
        if [[ "${shall_I_upgrade,,}x" = "yesx"  ||  "${shall_I_upgrade,,}x" = "yx" ]]; then

          echo -e "Yes.  Okay, attempting to upgrade now.  The upgrade requires super user privileges and you may be prompted for your password.\n"

          sudo /bin/bash -svx -- <<-EOF   # the "-" after the "<<" allows leading tabs (but not spaces), a quoted EOF would mean literal input, i.e., do not substitute parameters

				# Move gcc 9 and clang 10 to gcc ${RequiredGccVersion} and clang ${RequiredClangVersion} on Ubuntu 20.04 LTS

				# Someday, Ubuntu 20.04 standard packages will be updated to include the new versions, but for now ...
				add-apt-repository -y ppa:ubuntu-toolchain-r/test

				apt -y update
				# apt -y upgrade  # takes too long and sometimes doesn't work on the VCL, otherwise it's a good idea but not required

				apt install build-essential manpages-dev gdb ddd  # in case "Tuffix" is not installed (e.g. WSL)

				apt -y install gcc-${RequiredGccVersion} g++-${RequiredGccVersion}
				update-alternatives  --install /usr/bin/gcc gcc /usr/bin/gcc-${RequiredGccVersion} ${RequiredGccVersion}  --slave /usr/bin/g++         g++         /usr/bin/g++-${RequiredGccVersion}        \
				                                                                                                          --slave /usr/bin/gcc-ar      gcc-ar      /usr/bin/gcc-ar-${RequiredGccVersion}     \
				                                                                                                          --slave /usr/bin/gcc-nm      gcc-nm      /usr/bin/gcc-nm-${RequiredGccVersion}     \
				                                                                                                          --slave /usr/bin/gcc-ranlib  gcc-ranlib  /usr/bin/gcc-ranlib-${RequiredGccVersion} \
				                                                                                                          --slave /usr/bin/gcov        gcov        /usr/bin/gcov-${RequiredGccVersion}       \
				                                                                                                          --slave /usr/bin/gcov-dump   gcov-dump   /usr/bin/gcov-dump-${RequiredGccVersion}  \
				                                                                                                          --slave /usr/bin/gcov-tool   gcov-tool   /usr/bin/gcov-tool-${RequiredGccVersion}

				update-alternatives  --install /usr/bin/gcc gcc /usr/bin/gcc-9   9                                        --slave /usr/bin/g++         g++         /usr/bin/g++-9                    \
				                                                                                                          --slave /usr/bin/gcc-ar      gcc-ar      /usr/bin/gcc-ar-9                 \
				                                                                                                          --slave /usr/bin/gcc-nm      gcc-nm      /usr/bin/gcc-nm-9                 \
				                                                                                                          --slave /usr/bin/gcc-ranlib  gcc-ranlib  /usr/bin/gcc-ranlib-9             \
				                                                                                                          --slave /usr/bin/gcov        gcov        /usr/bin/gcov-9                   \
				                                                                                                          --slave /usr/bin/gcov-dump   gcov-dump   /usr/bin/gcov-dump-9              \
				                                                                                                          --slave /usr/bin/gcov-tool   gcov-tool   /usr/bin/gcov-tool-9

				apt -y install clang-${RequiredClangVersion} clang-tools-${RequiredClangVersion} clang-${RequiredClangVersion}-doc libclang-common-${RequiredClangVersion}-dev libclang-${RequiredClangVersion}-dev libclang1-${RequiredClangVersion} clang-format-${RequiredClangVersion} clang-tidy-${RequiredClangVersion} python3-clang-${RequiredClangVersion} clangd-${RequiredClangVersion}
				apt -y install lldb-${RequiredClangVersion} lld-${RequiredClangVersion}
				apt -y install libc++-${RequiredClangVersion}-dev libc++abi-${RequiredClangVersion}-dev
				apt -y autoremove

				update-alternatives  --install /usr/bin/clang clang /usr/bin/clang-${RequiredClangVersion} ${RequiredClangVersion}  --slave /usr/bin/clang++            clang++            /usr/bin/clang++-${RequiredClangVersion}              \
				                                                                                                                    --slave /usr/bin/clang-format       clang-format       /usr/bin/clang-format-${RequiredClangVersion}         \
				                                                                                                                    --slave /usr/bin/clang-format-diff  clang-format-diff  /usr/bin/clang-format-diff-${RequiredClangVersion}    \
				                                                                                                                    --slave /usr/bin/clang-tidy         clang-tidy         /usr/bin/clang-tidy-${RequiredClangVersion}           \
				                                                                                                                    --slave /usr/bin/clang-tidy-diff    clang-tidy-diff    /usr/bin/clang-tidy-diff-${RequiredClangVersion}.py

				update-alternatives  --install /usr/bin/clang clang /usr/bin/clang-10 10                                            --slave /usr/bin/clang++            clang++            /usr/bin/clang++-10              \
				                                                                                                                    --slave /usr/bin/clang-format       clang-format       /usr/bin/clang-format-10         \
				                                                                                                                    --slave /usr/bin/clang-format-diff  clang-format-diff  /usr/bin/clang-format-diff-10    \
				                                                                                                                    --slave /usr/bin/clang-tidy         clang-tidy         /usr/bin/clang-tidy-10           \
				                                                                                                                    --slave /usr/bin/clang-tidy-diff    clang-tidy-diff    /usr/bin/clang-tidy-diff-10.py
				sudo update-alternatives --auto gcc
				sudo update-alternatives --auto clang

				###################
				# To remove:
				# sudo add-apt-repository -y --remove ppa:ubuntu-toolchain-r/test
				# sudo update-alternatives --set gcc   /usr/bin/gcc-9
				# sudo update-alternatives --set clang /usr/bin/clang-10

				###################
				# To select which version interactively:
				# sudo update-alternatives --config gcc
				# sudo update-alternatives --config clang

				###################
				# References:
				# https://apt.llvm.org/
				# https://stackoverflow.com/questions/67298443/when-gcc-11-will-appear-in-ubuntu-repositories/67406788#67406788

				EOF

        exit

        fi # upgrade? 2
      fi  # upgrade? 1

      echo -e "No.  Okay, but your program may not compile, link, or execute properly\n"
    fi  # gccVersion || clangVersion
  fi  # Distribution && Release
}


BuildWorkaroundHeader()
{
	cat - > "${complianceHelperFile_path}" <<-"EOF"   # the "-" after the "<<" allow leading tabs (but not spaces), the quoted EOF means literal input, i.e., do not substitute parameters
		/******************************************************************************
		** Auto generated from Build.sh
		******************************************************************************/

		#pragma once

		// need to include something from the standard library so _LIBCPP_VERSION gets defined, so don't move
		// these #includes under that conditional compile
		#include <compare>
		#include <chrono>           // hh_mm_ss<>
		#include <cmath>            // abs()
		#include <iostream>         // ostream
		#include <string>
		#include <type_traits>      // is_floating_point<>, decay<>, is_convertible_v<>
		#include <utility>          // declval<>


		// C++20 Transition Workarounds
		//    _LIBCPP_VERSION is set if using LLVM's libc++ library
		//    __GLIBCXX__     is set if using GNU's libstdc++ library (set to date of release)
		//           (__GLIBC__ * 1'000 + __GLIBC_MINOR__)  gives libstdc++ version
		//    __GNUC__        is defined if using GCC, but also sometimes when using Clang
		//          (__GNUC__ * 1'000'000 + __GNUC_MINOR__ * 1'000 + __GNUC_PATCHLEVEL__) give gcc version



		#if defined( _LIBCPP_VERSION )
		  namespace std
		  {
		    #if _LIBCPP_VERSION < 14'000
		       inline strong_ordering operator<=>( const string & lhs, const string & rhs) noexcept
		       {
		         int result = lhs.compare( rhs );
		         return result == 0  ?  strong_ordering::equivalent
		              : result  < 0  ?  strong_ordering::less
		              :                 strong_ordering::greater;
		       }
		    #else
		      #pragma message ("A potentially obsolete C++20 workaround is present.  Either remove the workaround if no longer needed, or update the version number requiring it")
		    #endif


		    #if _LIBCPP_VERSION < 14'000
		       template<class T1, class T2>
		       constexpr weak_ordering  compare_weak_order_fallback( T1 && lhs, T2 && rhs ) noexcept
		       {
		         bool is_equal;
		         if constexpr( std::is_floating_point_v<std::decay_t<T1>>  &&  std::is_floating_point_v<decay_t<T2>> )   is_equal = std::abs( lhs - rhs ) < 1e-9;
		         else                                                                                                    is_equal = lhs == rhs;

		         return is_equal    ?  weak_ordering::equivalent
		              : lhs  < rhs  ?  weak_ordering::less
		              :                weak_ordering::greater;
		       }
		    #else
		      #pragma message ("A potentially obsolete C++20 workaround is present.  Either remove the workaround if no longer needed, or update the version number requiring it")
		    #endif


		    #if _LIBCPP_VERSION < 13'000
		      template<typename _From, typename _To>
		      concept convertible_to = is_convertible_v<_From, _To> && requires
		      {
		        static_cast<_To>( declval<_From>() );
		      };
		    #elif _LIBCPP_VERSION > 13'000  // resolved in version 13, but we're running a mix of version 12 and 13 for a while
		      #pragma message ("A potentially obsolete C++20 workaround is present.  Either remove the workaround if no longer needed, or update the version number requiring it")
		    #endif
		  } // namespace std
		#endif // defined( _LIBCPP_VERSION )




		#if( ( defined( _LIBCPP_VERSION ) && (_LIBCPP_VERSION                      <  14'000 )) || \
		     ( defined( __GLIBCXX__     ) && ( __GLIBC__ * 1'000 + __GLIBC_MINOR__ <=  2'040 )) )     // roughly a little beyond gcc 11.2.1 (run "ldd --version" and "gcc --version" at the command line)
		  namespace std::chrono                                                                       // Check lib version and not gcc version, clang may use gnu's libstdc++ library
		  {
		    template<class Duration>
		    inline std::ostream & operator<<( std::ostream & os, const std::chrono::hh_mm_ss<Duration> & t )
		    {
		      return os << t.hours     ().count() << ':'
		                << t.minutes   ().count() << ':'
		                << t.seconds   ().count() << "."
		                << t.subseconds().count();
		    }
		  }    // namespace std::chrono
		#else
		  #pragma message( "A potentially obsolete C++20 workaround is present.  Either remove the workaround if no longer needed, or update the version number requiring it" )
		#endif    // ( _LIBCPP_VERSION || __GLIBCXX__ )
	EOF
}




CheckVersion



### Vendors are releasing version of their C++ compilers and libraries with ever more C++20 compliant features, but
### they're not fully compliant yet.  And Linux vendors are slow to make these new versions available.  As a result,
### we see in practice various degrees of compliance which we can compensate for by providing the missing pieces in a
### header file that is added to each translation unit.  We search for such a header file first in the current working
### directory, then in the same directory as this build script, and if not found in either of those locations we create
### create it on the fly.  This search order allows such a (potentially updated) header file to be provided with each
### project.
complianceHelperFile_filename="Compliance_Workarounds.hpp"

complianceHelperFile_path="./${complianceHelperFile_filename}"
if [[ ! -f "${complianceHelperFile_path}" ]]; then                                   # Is the helper file in the current directory?
  complianceHelperFile_path="${0%/*}/${complianceHelperFile_filename}"
  if [[ ! -f "${complianceHelperFile_path}" ]]; then                                 # Is the helper file in the same directory as this script (Build.sh)
    complianceHelperFile_path="$( mktemp -p /tmp ${complianceHelperFile_filename}.XXXXXXXX || exit 3 )"
    trap 'rm -f "${complianceHelperFile_path}"' EXIT                                 # clean up after my self
    BuildWorkaroundHeader
  fi
fi



# Find and display all the C++ source files to be compiled ...
# temporarily ignore spaces when globing words into file names
temp=$IFS
  IFS=$'\n'
  sourceFiles=( $(find ./ -path ./.\* -prune -o -name "*.cpp" -print) )              # create array of source files skipping hidden folders (folders that start with a dot)
IFS=$temp

echo "compiling in \"$PWD\" ..."
for fileName in "${sourceFiles[@]}"; do
  echo "  $fileName"
done
echo ""


#define options
GccOptions="  -Wall -Wextra -pedantic        \
              -Wdelete-non-virtual-dtor      \
              -Wduplicated-branches          \
              -Wduplicated-cond              \
              -Wextra-semi                   \
              -Wfloat-equal                  \
              -Winit-self                    \
              -Wlogical-op                   \
              -Wnoexcept                     \
              -Wshadow                       \
              -Wnon-virtual-dtor             \
              -Wold-style-cast               \
              -Wstrict-null-sentinel         \
              -Wsuggest-override             \
              -Wswitch-default               \
              -Wswitch-enum                  \
              -Woverloaded-virtual           \
              -Wuseless-cast                 "

#             -Wzero-as-null-pointer-constant"


ClangOptions=" -stdlib=libc++ -Weverything        \
               -Wno-comma                         \
               -Wno-unused-template               \
               -Wno-sign-conversion               \
               -Wno-exit-time-destructors         \
               -Wno-global-constructors           \
               -Wno-missing-prototypes            \
               -Wno-weak-vtables                  \
               -Wno-padded                        \
               -Wno-double-promotion              \
               -Wno-c++98-compat-pedantic         \
               -Wno-c++11-compat-pedantic         \
               -Wno-c++14-compat-pedantic         \
               -Wno-c++17-compat-pedantic         \
               -Wno-c++20-compat-pedantic         \
               -fdiagnostics-show-category=name   \
                                                  \
               -Wno-zero-as-null-pointer-constant \
               -Wno-ctad-maybe-unsupported        "

CommonOptions="-g0 -O3 -DNDEBUG -pthread -std=c++20 -I./ -DUSING_TOMS_SUGGESTIONS -D__func__=__PRETTY_FUNCTION__"




ClangCommand="clang++ $CommonOptions $ClangOptions"
echo $ClangCommand -include \"${complianceHelperFile_path}\"
clang++ --version
if $ClangCommand -include "${complianceHelperFile_path}" -o "${executableFileName}_clang++"  "${sourceFiles[@]}"; then
  echo -e "\nSuccessfully created  \"${executableFileName}_clang++\""
else
  exit 1
fi

echo ""

GccCommand="g++ $CommonOptions $GccOptions"
echo $GccCommand -include \"${complianceHelperFile_path}\"
g++ --version
if $GccCommand -include "${complianceHelperFile_path}" -o "${executableFileName}_g++"  "${sourceFiles[@]}"; then
   echo -e "\nSuccessfully created  \"${executableFileName}_g++\""
else
   exit 1
fi
