// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME root_tree_dic
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "event_CLASS.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_event_CLASS(void *p = 0);
   static void *newArray_event_CLASS(Long_t size, void *p);
   static void delete_event_CLASS(void *p);
   static void deleteArray_event_CLASS(void *p);
   static void destruct_event_CLASS(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::event_CLASS*)
   {
      ::event_CLASS *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::event_CLASS >(0);
      static ::ROOT::TGenericClassInfo 
         instance("event_CLASS", ::event_CLASS::Class_Version(), "event_CLASS.h", 11,
                  typeid(::event_CLASS), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::event_CLASS::Dictionary, isa_proxy, 4,
                  sizeof(::event_CLASS) );
      instance.SetNew(&new_event_CLASS);
      instance.SetNewArray(&newArray_event_CLASS);
      instance.SetDelete(&delete_event_CLASS);
      instance.SetDeleteArray(&deleteArray_event_CLASS);
      instance.SetDestructor(&destruct_event_CLASS);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::event_CLASS*)
   {
      return GenerateInitInstanceLocal((::event_CLASS*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::event_CLASS*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr event_CLASS::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *event_CLASS::Class_Name()
{
   return "event_CLASS";
}

//______________________________________________________________________________
const char *event_CLASS::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::event_CLASS*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int event_CLASS::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::event_CLASS*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *event_CLASS::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::event_CLASS*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *event_CLASS::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::event_CLASS*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void event_CLASS::Streamer(TBuffer &R__b)
{
   // Stream an object of class event_CLASS.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(event_CLASS::Class(),this);
   } else {
      R__b.WriteClassBuffer(event_CLASS::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_event_CLASS(void *p) {
      return  p ? new(p) ::event_CLASS : new ::event_CLASS;
   }
   static void *newArray_event_CLASS(Long_t nElements, void *p) {
      return p ? new(p) ::event_CLASS[nElements] : new ::event_CLASS[nElements];
   }
   // Wrapper around operator delete
   static void delete_event_CLASS(void *p) {
      delete ((::event_CLASS*)p);
   }
   static void deleteArray_event_CLASS(void *p) {
      delete [] ((::event_CLASS*)p);
   }
   static void destruct_event_CLASS(void *p) {
      typedef ::event_CLASS current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::event_CLASS

namespace {
  void TriggerDictionaryInitialization_libroot_tree_dic_Impl() {
    static const char* headers[] = {
"event_CLASS.h",
0
    };
    static const char* includePaths[] = {
"/home/cindy/root/include",
"/home/cindy/cernbox/MyProjects/SPC_IS-Trento/root_trento/src",
"/usr/include",
"/home/cindy/root/include/",
"/home/cindy/cernbox/MyProjects/SPC_IS-Trento/root_trento/build/src/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libroot_tree_dic dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$event_CLASS.h")))  event_CLASS;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libroot_tree_dic dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "event_CLASS.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"event_CLASS", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libroot_tree_dic",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libroot_tree_dic_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libroot_tree_dic_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libroot_tree_dic() {
  TriggerDictionaryInitialization_libroot_tree_dic_Impl();
}
