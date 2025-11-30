# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/magnus/Nextcloud/Uni/Projekt P1/projekt/P1/build/_deps/mtest-src")
  file(MAKE_DIRECTORY "/home/magnus/Nextcloud/Uni/Projekt P1/projekt/P1/build/_deps/mtest-src")
endif()
file(MAKE_DIRECTORY
  "/home/magnus/Nextcloud/Uni/Projekt P1/projekt/P1/build/_deps/mtest-build"
  "/home/magnus/Nextcloud/Uni/Projekt P1/projekt/P1/build/_deps/mtest-subbuild/mtest-populate-prefix"
  "/home/magnus/Nextcloud/Uni/Projekt P1/projekt/P1/build/_deps/mtest-subbuild/mtest-populate-prefix/tmp"
  "/home/magnus/Nextcloud/Uni/Projekt P1/projekt/P1/build/_deps/mtest-subbuild/mtest-populate-prefix/src/mtest-populate-stamp"
  "/home/magnus/Nextcloud/Uni/Projekt P1/projekt/P1/build/_deps/mtest-subbuild/mtest-populate-prefix/src"
  "/home/magnus/Nextcloud/Uni/Projekt P1/projekt/P1/build/_deps/mtest-subbuild/mtest-populate-prefix/src/mtest-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/magnus/Nextcloud/Uni/Projekt P1/projekt/P1/build/_deps/mtest-subbuild/mtest-populate-prefix/src/mtest-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/magnus/Nextcloud/Uni/Projekt P1/projekt/P1/build/_deps/mtest-subbuild/mtest-populate-prefix/src/mtest-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
