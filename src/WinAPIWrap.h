#pragma once
#include <string>
#include <vector>
#include "common.h"
#include <mmdeviceapi.h>
#include <endpointvolume.h>

//yes this is a singleton.
//It needs to be globally accessable and should only have one

//class IWinAPIWrap
//{
// public:
//
//   virtual std::string getName(std::wstring id) = 0;
//   virtual std::string getNameDefault(ERole role, EDataFlow flow) = 0;
//   virtual std::vector<std::string> getAllNames(EDataFlow flow, DWORD state) = 0;
//
//   virtual std::wstring getID(std::string identifier, EDataFlow flow, DWORD state) = 0;
//   virtual std::wstring getDefaultID(ERole role, EDataFlow flow) = 0;
//   virtual std::vector<std::wstring> getAllIDs(EDataFlow flow, DWORD state) = 0;
//
//   virtual DWORD getState(std::wstring id) = 0;
//
//   virtual bool getMute(std::wstring id) = 0;
//   virtual bool setMute(std::wstring id, bool statusToSet) = 0;
//
//   virtual int getVolume(std::wstring id) = 0;
//   virtual bool setVolume(std::wstring id, int volumeToSet) = 0;
//
//   virtual bool registerForNotifications(IMMNotificationClient *toRegister) = 0;
//   virtual bool registerForNotifications(IAudioEndpointVolumeCallback *toRegister, std::wstring id) = 0;
//};
//
//class WinAPIWrap : public IWinAPIWrap
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
      void initTests(IMMDeviceEnumerator *pTestEnumerator) { runningTests = true; pEnum = pTestEnumerator; initCount = 0; }
      void COMInitialize() { if (runningTests) initCount++; else CoInitialize(NULL); }
   };

   std::string getName(IMMDevicePtr device);
   std::string getNameDefault(ERole role, EDataFlow flow);
   std::vector<std::string> getAllNames(EDataFlow flow, DWORD state);

   IMMDevicePtr getDevice(std::wstring id);
   IMMDevicePtr getDevice(std::string uniqueNameIdentifier, EDataFlow flow, DWORD state = DEVICE_STATE_ACTIVE);

   IAudioEndpointVolumePtr getEndpointVolume(IMMDevicePtr device);
}


//class TestWinAPIWrap : public IWinAPIWrap
//{
//   IMMNotificationClient *notifier;
//   const int DEVICE_NOT_FOUND = -1;
//   int deviceCount;
//   class Device
//   {
//   public:
//      Device(EDataFlow _flow, std::string _name = "", std::wstring _id = L"", DWORD _state = DEVICE_STATE_ACTIVE, PROPERTYKEY _properties = {}) :
//         flow(_flow), name(_name), id(_id), state(_state), properties(_properties), callback(nullptr)
//      {
//      }
//      std::wstring id;
//      std::string name;
//      EDataFlow flow;
//      DWORD state;
//      PROPERTYKEY properties;
//      IAudioEndpointVolumeCallback *callback;
//      bool isSubsetOf(DWORD _state) { return (_state & state) != 0; }
//   };
//   EDataFlow getFlow(std::wstring id);
//   std::vector<Device> &devices(EDataFlow flow) { return flow == eRender ? speakers : mics; }
//   int getIndex(std::string name, EDataFlow flow, DWORD state = DEVICE_STATEMASK_ALL);
//   int getFirstActiveIndex(EDataFlow flow);
//   int &defaultIndex(EDataFlow flow) { return flow == eRender ? defaultSpeakerIndex : defaultMicIndex; }
//   int &defaultComIndex(EDataFlow flow) { return flow == eRender ? defaultComSpeakerIndex : defaultComMicIndex; }
//
//   std::vector<Device> mics;
//   std::vector<Device> speakers;
//   int defaultComSpeakerIndex;
//   int defaultSpeakerIndex;
//   int defaultComMicIndex;
//   int defaultMicIndex;
//public:
//   //Test specific functions: These throw errors.
//   void t_addDevice(std::string name, EDataFlow flow, DWORD state);
//   void t_removeDevice(std::string name, EDataFlow flow);
//   void t_changeProperty(std::string name, EDataFlow flow, PROPERTYKEY newProperties);
//   HRESULT t_callChangeProperties(std::string name, EDataFlow flow);
//   void t_changeState(std::string name, EDataFlow flow, DWORD newState);
//   HRESULT t_callChangeState(std::string name, EDataFlow flow);
//   //Valid to call using ERole_enum_count to set default for all roles
//   void t_changeDefault(std::string name, EDataFlow flow, ERole role);
//   HRESULT t_callChangeDefault(ERole role);
//
//   void t_changeVolume(std::wstring id, int newVolume);
//   void t_callChangeVolume(std::wstring id);
//   void t_changeMute(std::wstring id, bool isMuted);
//   void t_callChangeMute(std::wstring id);
//
//
//   //These should not.
//   std::string getName(std::wstring id);
//   std::string getNameDefault(ERole role, EDataFlow flow);
//   std::vector<std::string> getAllNames(EDataFlow flow, DWORD state);
//
//   std::wstring getID(std::string identifier, EDataFlow flow, DWORD state);
//   std::wstring getDefaultID(ERole role, EDataFlow flow);
//   std::vector<std::wstring> getAllIDs(EDataFlow flow, DWORD state);
//
//   DWORD getState(std::wstring id);
//
//   bool getMute(std::wstring id);
//   bool setMute(std::wstring id, bool statusToSet);
//
//   int getVolume(std::wstring id);
//   bool setVolume(std::wstring id, int volumeToSet);
//
//   bool registerForNotifications(IMMNotificationClient *toRegister);
//   bool registerForNotifications(IAudioEndpointVolumeCallback *toRegister, std::wstring id);
//};
//
//class InjectionFramework
//{
//   static bool runningTests;
//   static IWinAPIWrap *wrapper;
//
//public:
//
//   //Should only be called when running tests
//   //If get_instance is called before this function, the "pseudo singleton" will not be reconfigured
//   //for testing.
//   static void initForTesting() { runningTests = true; }
//
//   static IWinAPIWrap& get_instance() 
//   {
//      if (wrapper == nullptr)
//      {
//         if (runningTests)
//         {
//            wrapper = new TestWinAPIWrap();
//         }
//         else
//         {
//            CoInitialize(NULL);
//            wrapper = new WinAPIWrap();
//         }
//      }
//
//      return *wrapper;
//   }
//
//   //Will throw an error if Wrapper has not been configured for tests.
//   static TestWinAPIWrap& get_instance_test()
//   {
//      if (wrapper == nullptr && runningTests)
//      {
//         wrapper = new TestWinAPIWrap();
//      }
//
//      auto toReturn = dynamic_cast<TestWinAPIWrap*>(wrapper);
//
//      if(!toReturn)
//      {
//         throw "Trying to get test instance without properly initializing for tests.";
//      }
//
//      return *toReturn;
//   }
//
//};