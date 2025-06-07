#ifndef CORE_CHEISTRY_API_H
#define CORE_CHEISTRY_API_H
#include <boost/lexical_cast.hpp>
#include <string>

// Modified By Glysade to setup exports propertly for the RDKit
//  Build System.

#ifdef __linux
// conda-clang can set this, the linux version works just
//  fine now, so unset mac specific stuff
# ifdef TARGET_OS_MAC
#  define TARGET_OS_MAC 0
# endif
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
  #ifdef MYLIB_EXPORTS
    // building the DLL
    #define CORE_CHEMISTRY_API __declspec(dllexport)
  #else
    // using the DLL
    #define CORE_CHEMISTRY_API __declspec(dllimport)
  #endif
#elif defined(__GNUC__) && __GNUC__ >= 4
  // GCC/Clang: export with default visibility
  #define CORE_CHEMISTRY_API __attribute__((visibility("default")))
#else
  #define CORE_CHEMISTRY_API
#endif

// Even if we are a static build we define this
// it prevents missing classes from being included
// and mucking up the DLL builds
#define CORE_CHEMISTRY_API_DLL_BUILD

enum Dimensions2or3 {
  kIn2D = 2,
  kIn3D = 3
};

template<typename T>
void DeleteAndNull(T* ptr) {
  delete ptr;
  ptr = nullptr;
}


namespace cs {
  template<class T>
  std::string NumToStr(T value) {
    return boost::lexical_cast<std::string>(value);
  }
  
  inline size_t StrToNum(const std::string &value) {
    return boost::lexical_cast<size_t>(value);
  }

  inline double StrToDub(const std::string &value) {
    return boost::lexical_cast<double>(value);
  }
}

template<class T, class U> T FAST_dynamic_cast(U* obj) {
  return dynamic_cast<T>(obj);
}
#endif
