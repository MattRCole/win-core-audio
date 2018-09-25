#pragma once
#include <string>
#include <vector>
#include "common.h"
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <tuple>

namespace WinAPIWrap
{
   class MyUnknownPtr {
      void release()
      {
         if (pUnknown)
         {
            pUnknown->Release();
            pUnknown = nullptr;
         }
      }
      void addRef()
      {
         if (pUnknown)
         {
            pUnknown->AddRef();
         }
      }

   protected:
      IUnknown * pUnknown;
      MyUnknownPtr() : pUnknown(nullptr) {}
      MyUnknownPtr(const MyUnknownPtr& other) : pUnknown(other.pUnknown) { addRef(); }
      MyUnknownPtr(IUnknown *raw) : pUnknown(nullptr) { }
      MyUnknownPtr &operator=(const MyUnknownPtr &rhs)
      {
         release();
         pUnknown = rhs.pUnknown;
         return *this;
      }
      MyUnknownPtr &operator=(IUnknown *rawRhs)
      {
         release();
         pUnknown = rawRhs;
         return *this;
      }
   public:
      IUnknown * * getPointerToPointerToNull() { release();  return &pUnknown; }
      ~MyUnknownPtr() { release(); }
   };

   class IMMDeviceEnumeratorPtr : public MyUnknownPtr {
   private:
      IMMDeviceEnumerator * getEnumerator() { return dynamic_cast<IMMDeviceEnumerator *>(pUnknown); }
      MockCom::MockDeviceEnumerator dummyEnum;
   public:
      IMMDeviceEnumeratorPtr() : MyUnknownPtr() {}
      IMMDeviceEnumeratorPtr(const IMMDeviceEnumeratorPtr &other) : MyUnknownPtr(other) {}
      IMMDeviceEnumeratorPtr(IMMDeviceEnumerator *raw) : MyUnknownPtr(raw) {}
      IMMDeviceEnumeratorPtr& operator=(const IMMDeviceEnumeratorPtr &rhs) { MyUnknownPtr::operator=(rhs); return *this; }
      IMMDeviceEnumeratorPtr& operator=(IMMDeviceEnumerator *rawRhs) { MyUnknownPtr::operator=(rawRhs); return *this; }

      IMMDeviceEnumerator * operator->() { auto pEnum = getEnumerator(); if (pEnum) return pEnum; else return &dummyEnum; }
   };

   class IMMDeviceCollectionPtr : public MyUnknownPtr {
      IMMDeviceCollection *getCollection() { return dynamic_cast<IMMDeviceCollection *>(pUnknown); }
      MockCom::MockDeviceCollection dummyCollection;
   public:
      IMMDeviceCollectionPtr() : MyUnknownPtr() {}
      IMMDeviceCollectionPtr(const IMMDeviceCollectionPtr &other) : MyUnknownPtr(other) {}
      IMMDeviceCollectionPtr(IMMDeviceCollection *raw) : MyUnknownPtr(raw) {}
      IMMDeviceCollectionPtr& operator=(const IMMDeviceCollectionPtr &rhs) { MyUnknownPtr::operator=(rhs); return *this; }
      IMMDeviceCollectionPtr& operator=(IMMDeviceCollection *rawRhs) { MyUnknownPtr::operator=(rawRhs); return *this; }

      IMMDeviceCollection * operator->() { auto pCollection = getCollection(); if (pCollection) return pCollection; else return &dummyCollection; }
   };

   class IMMDevicePtr : public MyUnknownPtr {
      IMMDevice *getDevice() { return dynamic_cast<IMMDevice *>(pUnknown); }
      MockCom::MockDevice dummyDevice;
   public:
      IMMDevicePtr() : MyUnknownPtr() {}
      IMMDevicePtr(const IMMDevicePtr &other) : MyUnknownPtr(other) {}
      IMMDevicePtr(IMMDevice *raw) : MyUnknownPtr(raw) {}
      IMMDevicePtr& operator=(const IMMDevicePtr &rhs) { MyUnknownPtr::operator=(rhs); return *this; }
      IMMDevicePtr& operator=(IMMDevice *rawRhs) { MyUnknownPtr::operator=(rawRhs); return *this; }

      IMMDevice * operator->() { auto pDevice = getDevice(); if (pDevice) return pDevice; else return &dummyDevice; }
   };

   class IPropertyStorePtr : public MyUnknownPtr{
   private:
      IPropertyStore *getPropertyStore() { return dynamic_cast<IPropertyStore *>(pUnknown); }
      MockCom::MockPropertyStore dummyPropertyStore;
   public:
      IPropertyStorePtr() : MyUnknownPtr() {}
      IPropertyStorePtr(const IPropertyStorePtr &other) : MyUnknownPtr(other) {}
      IPropertyStorePtr(IPropertyStore *raw) : MyUnknownPtr(raw) {}
      IPropertyStorePtr& operator=(const IPropertyStorePtr &rhs) { MyUnknownPtr::operator=(rhs); return *this; }
      IPropertyStorePtr& operator=(IPropertyStore *rawRhs) { MyUnknownPtr::operator=(rawRhs); return *this; }

      IPropertyStore * operator->() { auto pStore = getPropertyStore(); if (pStore) return pStore; else return &dummyPropertyStore; }
   };

   class IAudioEndpointVolumePtr : public MyUnknownPtr {
      IAudioEndpointVolume *getEndpoint() { return dynamic_cast<IAudioEndpointVolume *>(pUnknown); }
      MockCom::MockAudioEndpointVolume dummyEndpoint;
   public:
      IAudioEndpointVolumePtr() : MyUnknownPtr() {}
      IAudioEndpointVolumePtr(const IAudioEndpointVolumePtr &other) : MyUnknownPtr(other) {}
      IAudioEndpointVolumePtr(IAudioEndpointVolume *raw) : MyUnknownPtr(raw) {}
      IAudioEndpointVolumePtr& operator=(const IAudioEndpointVolumePtr &rhs) { MyUnknownPtr::operator=(rhs); return *this; }
      IAudioEndpointVolumePtr& operator=(IAudioEndpointVolume *rawRhs) { MyUnknownPtr::operator=(rawRhs); return *this; }

      IAudioEndpointVolume * operator->() { auto pEndpoint = getEndpoint(); if (pEndpoint) return pEndpoint; else return &dummyEndpoint; }
   };

   class InjectionFramework {
      static bool runningTests;
      static IMMDeviceEnumerator *pEnum;

   public:
      static int initCount;
      static IMMDeviceEnumerator * getEnumerator(DWORD dwClsContext = CLSCTX_ALL);
      static void initTests(IMMDeviceEnumerator *pTestEnumerator) { runningTests = true; pEnum = pTestEnumerator; initCount = 0; }
      static void ComInitialize() { if (runningTests) initCount++; else CoInitialize(NULL); }
   };

   std::string getName(IMMDevicePtr device);
   std::string getNameDefault(ERole role, EDataFlow flow);
   std::vector<std::string> getAllNames(EDataFlow flow, DWORD state);

   IMMDevicePtr getDevice(std::wstring id);
   IMMDevicePtr getDevice(std::string uniqueNameIdentifier, EDataFlow flow, DWORD state = DEVICE_STATE_ACTIVE);

   std::wstring getId(IMMDevicePtr device);
   std::vector<std::wstring> getAllIds(EDataFlow flow, DWORD state);


   std::vector<std::tuple<std::string, std::wstring>>getDeviceNamesAndIds(EDataFlow flow, DWORD state);

   IAudioEndpointVolumePtr getEndpointVolume(IMMDevicePtr device);
}
