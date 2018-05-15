// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME PrimaryHitsDict

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

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "PrimaryHits.hh"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *PrimaryHits_Dictionary();
   static void PrimaryHits_TClassManip(TClass*);
   static void *new_PrimaryHits(void *p = 0);
   static void *newArray_PrimaryHits(Long_t size, void *p);
   static void delete_PrimaryHits(void *p);
   static void deleteArray_PrimaryHits(void *p);
   static void destruct_PrimaryHits(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PrimaryHits*)
   {
      ::PrimaryHits *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::PrimaryHits));
      static ::ROOT::TGenericClassInfo 
         instance("PrimaryHits", "PrimaryHits.hh", 20,
                  typeid(::PrimaryHits), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &PrimaryHits_Dictionary, isa_proxy, 4,
                  sizeof(::PrimaryHits) );
      instance.SetNew(&new_PrimaryHits);
      instance.SetNewArray(&newArray_PrimaryHits);
      instance.SetDelete(&delete_PrimaryHits);
      instance.SetDeleteArray(&deleteArray_PrimaryHits);
      instance.SetDestructor(&destruct_PrimaryHits);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PrimaryHits*)
   {
      return GenerateInitInstanceLocal((::PrimaryHits*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PrimaryHits*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *PrimaryHits_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::PrimaryHits*)0x0)->GetClass();
      PrimaryHits_TClassManip(theClass);
   return theClass;
   }

   static void PrimaryHits_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_PrimaryHits(void *p) {
      return  p ? new(p) ::PrimaryHits : new ::PrimaryHits;
   }
   static void *newArray_PrimaryHits(Long_t nElements, void *p) {
      return p ? new(p) ::PrimaryHits[nElements] : new ::PrimaryHits[nElements];
   }
   // Wrapper around operator delete
   static void delete_PrimaryHits(void *p) {
      delete ((::PrimaryHits*)p);
   }
   static void deleteArray_PrimaryHits(void *p) {
      delete [] ((::PrimaryHits*)p);
   }
   static void destruct_PrimaryHits(void *p) {
      typedef ::PrimaryHits current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PrimaryHits

namespace ROOT {
   static TClass *vectorlEdoublegR_Dictionary();
   static void vectorlEdoublegR_TClassManip(TClass*);
   static void *new_vectorlEdoublegR(void *p = 0);
   static void *newArray_vectorlEdoublegR(Long_t size, void *p);
   static void delete_vectorlEdoublegR(void *p);
   static void deleteArray_vectorlEdoublegR(void *p);
   static void destruct_vectorlEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<double>*)
   {
      vector<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<double>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<double>", -2, "vector", 210,
                  typeid(vector<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEdoublegR_Dictionary, isa_proxy, 4,
                  sizeof(vector<double>) );
      instance.SetNew(&new_vectorlEdoublegR);
      instance.SetNewArray(&newArray_vectorlEdoublegR);
      instance.SetDelete(&delete_vectorlEdoublegR);
      instance.SetDeleteArray(&deleteArray_vectorlEdoublegR);
      instance.SetDestructor(&destruct_vectorlEdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<double> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<double>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<double>*)0x0)->GetClass();
      vectorlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double> : new vector<double>;
   }
   static void *newArray_vectorlEdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double>[nElements] : new vector<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEdoublegR(void *p) {
      delete ((vector<double>*)p);
   }
   static void deleteArray_vectorlEdoublegR(void *p) {
      delete [] ((vector<double>*)p);
   }
   static void destruct_vectorlEdoublegR(void *p) {
      typedef vector<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<double>

namespace ROOT {
   static TClass *vectorlETStringgR_Dictionary();
   static void vectorlETStringgR_TClassManip(TClass*);
   static void *new_vectorlETStringgR(void *p = 0);
   static void *newArray_vectorlETStringgR(Long_t size, void *p);
   static void delete_vectorlETStringgR(void *p);
   static void deleteArray_vectorlETStringgR(void *p);
   static void destruct_vectorlETStringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TString>*)
   {
      vector<TString> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TString>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TString>", -2, "vector", 210,
                  typeid(vector<TString>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETStringgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<TString>) );
      instance.SetNew(&new_vectorlETStringgR);
      instance.SetNewArray(&newArray_vectorlETStringgR);
      instance.SetDelete(&delete_vectorlETStringgR);
      instance.SetDeleteArray(&deleteArray_vectorlETStringgR);
      instance.SetDestructor(&destruct_vectorlETStringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TString> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TString>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETStringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TString>*)0x0)->GetClass();
      vectorlETStringgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETStringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETStringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TString> : new vector<TString>;
   }
   static void *newArray_vectorlETStringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TString>[nElements] : new vector<TString>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETStringgR(void *p) {
      delete ((vector<TString>*)p);
   }
   static void deleteArray_vectorlETStringgR(void *p) {
      delete [] ((vector<TString>*)p);
   }
   static void destruct_vectorlETStringgR(void *p) {
      typedef vector<TString> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TString>

namespace {
  void TriggerDictionaryInitialization_PrimaryHitsDict_Impl() {
    static const char* headers[] = {
"PrimaryHits.hh",
0
    };
    static const char* includePaths[] = {
"/my_cache/ROOT6.10.08/install/include",
"/home/zhiyu/proj/MuImag/include/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "PrimaryHitsDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
class __attribute__((annotate("$clingAutoload$TString.h")))  __attribute__((annotate("$clingAutoload$PrimaryHits.hh")))  TString;
class __attribute__((annotate("$clingAutoload$PrimaryHits.hh")))  PrimaryHits;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "PrimaryHitsDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "PrimaryHits.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"PrimaryHits", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("PrimaryHitsDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_PrimaryHitsDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_PrimaryHitsDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_PrimaryHitsDict() {
  TriggerDictionaryInitialization_PrimaryHitsDict_Impl();
}
