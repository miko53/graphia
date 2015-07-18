echo %1
IF [%1]==[] ECHO Value Missing

IF "%1"=="all" (
  mkdir build-mingw32-debug
  cd build-mingw32-debug
  cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug ../.
  cd ..
  mkdir build-mingw32-release
  cd build-mingw32-release
  cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ../.
  cd ..
  goto end
  )

IF "%1"=="debug" (
:debug
  mkdir build-mingw32-debug
  cd build-mingw32-debug
  cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug ../.
  cd ..
  )
  
IF "%1"=="release" (
:release
  mkdir build-mingw32-release
  cd build-mingw32-release
  cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ../.
  cd ..
)  
:end