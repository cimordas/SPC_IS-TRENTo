// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME events_info_dic
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
#include "events_info_CLASS.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_events_info_CLASS(void *p = 0);
   static void *newArray_events_info_CLASS(Long_t size, void *p);
   static void delete_events_info_CLASS(void *p);
   static void deleteArray_events_info_CLASS(void *p);
   static void destruct_events_info_CLASS(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::events_info_CLASS*)
   {
      ::events_info_CLASS *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::events_info_CLASS >(0);
      static ::ROOT::TGenericClassInfo 
         instance("events_info_CLASS", ::events_info_CLASS::Class_Version(), "events_info_CLASS.h", 16,
                  typeid(::events_info_CLASS), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::events_info_CLASS::Dictionary, isa_proxy, 4,
                  sizeof(::events_info_CLASS) );
      instance.SetNew(&new_events_info_CLASS);
      instance.SetNewArray(&newArray_events_info_CLASS);
      instance.SetDelete(&delete_events_info_CLASS);
      instance.SetDeleteArray(&deleteArray_events_info_CLASS);
      instance.SetDestructor(&destruct_events_info_CLASS);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::events_info_CLASS*)
   {
      return GenerateInitInstanceLocal((::events_info_CLASS*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::events_info_CLASS*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr events_info_CLASS::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *events_info_CLASS::Class_Name()
{
   return "events_info_CLASS";
}

//______________________________________________________________________________
const char *events_info_CLASS::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::events_info_CLASS*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int events_info_CLASS::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::events_info_CLASS*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *events_info_CLASS::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::events_info_CLASS*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *events_info_CLASS::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::events_info_CLASS*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void events_info_CLASS::Streamer(TBuffer &R__b)
{
   // Stream an object of class events_info_CLASS.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(events_info_CLASS::Class(),this);
   } else {
      R__b.WriteClassBuffer(events_info_CLASS::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_events_info_CLASS(void *p) {
      return  p ? new(p) ::events_info_CLASS : new ::events_info_CLASS;
   }
   static void *newArray_events_info_CLASS(Long_t nElements, void *p) {
      return p ? new(p) ::events_info_CLASS[nElements] : new ::events_info_CLASS[nElements];
   }
   // Wrapper around operator delete
   static void delete_events_info_CLASS(void *p) {
      delete ((::events_info_CLASS*)p);
   }
   static void deleteArray_events_info_CLASS(void *p) {
      delete [] ((::events_info_CLASS*)p);
   }
   static void destruct_events_info_CLASS(void *p) {
      typedef ::events_info_CLASS current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::events_info_CLASS

namespace ROOT {
   static TClass *maplETStringcOTStringgR_Dictionary();
   static void maplETStringcOTStringgR_TClassManip(TClass*);
   static void *new_maplETStringcOTStringgR(void *p = 0);
   static void *newArray_maplETStringcOTStringgR(Long_t size, void *p);
   static void delete_maplETStringcOTStringgR(void *p);
   static void deleteArray_maplETStringcOTStringgR(void *p);
   static void destruct_maplETStringcOTStringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<TString,TString>*)
   {
      map<TString,TString> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<TString,TString>));
      static ::ROOT::TGenericClassInfo 
         instance("map<TString,TString>", -2, "map", 100,
                  typeid(map<TString,TString>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplETStringcOTStringgR_Dictionary, isa_proxy, 0,
                  sizeof(map<TString,TString>) );
      instance.SetNew(&new_maplETStringcOTStringgR);
      instance.SetNewArray(&newArray_maplETStringcOTStringgR);
      instance.SetDelete(&delete_maplETStringcOTStringgR);
      instance.SetDeleteArray(&deleteArray_maplETStringcOTStringgR);
      instance.SetDestructor(&destruct_maplETStringcOTStringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<TString,TString> >()));

      ::ROOT::AddClassAlternate("map<TString,TString>","std::map<TString, TString, std::less<TString>, std::allocator<std::pair<TString const, TString> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<TString,TString>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplETStringcOTStringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<TString,TString>*)0x0)->GetClass();
      maplETStringcOTStringgR_TClassManip(theClass);
   return theClass;
   }

   static void maplETStringcOTStringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplETStringcOTStringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,TString> : new map<TString,TString>;
   }
   static void *newArray_maplETStringcOTStringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<TString,TString>[nElements] : new map<TString,TString>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplETStringcOTStringgR(void *p) {
      delete ((map<TString,TString>*)p);
   }
   static void deleteArray_maplETStringcOTStringgR(void *p) {
      delete [] ((map<TString,TString>*)p);
   }
   static void destruct_maplETStringcOTStringgR(void *p) {
      typedef map<TString,TString> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<TString,TString>

namespace {
  void TriggerDictionaryInitialization_libevents_info_dic_Impl() {
    static const char* headers[] = {
"events_info_CLASS.h",
0
    };
    static const char* includePaths[] = {
"/home/cindy/root/include",
"/home/cindy/cernbox/MyProjects/SPC_IS-Trento/calculator/src",
"/usr/include",
"/home/cindy/root/include/",
"/home/cindy/cernbox/MyProjects/SPC_IS-Trento/calculator/build/src/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libevents_info_dic dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$events_info_CLASS.h")))  events_info_CLASS;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libevents_info_dic dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "events_info_CLASS.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"events_info_CLASS", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libevents_info_dic",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libevents_info_dic_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libevents_info_dic_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libevents_info_dic() {
  TriggerDictionaryInitialization_libevents_info_dic_Impl();
}
