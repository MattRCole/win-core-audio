#include "WinAPIWrap.h"
#include "common.h"
#include <string>
#include <sstream>
#include <mmdeviceapi.h>
#include <vector>
#include <functiondiscoverykeys_devpkey.h>
#include <endpointvolume.h>
#include <locale>
//#include <comdef.h>
//#include <comip.h>

//_COM_SMARTPTR_TYPEDEF(IMMDeviceEnumerator, __uuidof(IMMDeviceEnumerator));   //defines IMMDeviceEnumeratorPtr
//_COM_SMARTPTR_TYPEDEF(IMMDevice, __uuidof(IMMDevice));                       //defines IMMDevicePtr
//_COM_SMARTPTR_TYPEDEF(IAudioEndpointVolume, __uuidof(IAudioEndpointVolume)); //defines IAudioEndpointVolumePtr

//#define WinAPISafeRelease(...) \
//   if (__VA_ARGS__ != NULL)    \
//   {                           \
//      __VA_ARGS__->Release();  \
//      __VA_ARGS__ = nullptr;   \
//   }

////Static member variable declaration
//bool Wrapper::runningTests = false;
//IWinAPIWrap * Wrapper::wrapper = nullptr;
//
//namespace
//{
//   IMMDeviceEnumerator *_getEnumerator()
//   {
//      IMMDeviceEnumerator *pEnum = nullptr;
//      HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void **)&pEnum);
//      return SUCCEEDED(hr) ? pEnum : nullptr;
//   }
//
//   IMMDevice *_getDevice(std::wstring id)
//   {
//      IMMDeviceEnumerator *pEnum = _getEnumerator();
//      IMMDevice *pDevice = nullptr;
//      if (pEnum == nullptr || id.size() == 0)
//      {
//         return nullptr;
//      }
//
//      HRESULT hr = pEnum->GetDevice(id.c_str(), &pDevice);
//
//      WinAPISafeRelease(pEnum);
//
//      return SUCCEEDED(hr) ? pDevice : nullptr;
//   }
//
//   IMMDevice *_getDefault(ERole role, EDataFlow flow)
//   {
//      IMMDeviceEnumerator *pEnum = _getEnumerator();
//      IMMDevice *pDevice = nullptr;
//      if (pEnum == nullptr)
//      {
//         return nullptr;
//      }
//
//      HRESULT hr = pEnum->GetDefaultAudioEndpoint(flow, role, &pDevice);
//
//      WinAPISafeRelease(pEnum);
//
//      return SUCCEEDED(hr) ? pDevice : nullptr;
//   }
//
//   std::string _getName(IMMDevice *pDevice)
//   {
//      if (pDevice == nullptr)
//      {
//         return "";
//      }
//
//      IPropertyStore *pStore;
//      PROPVARIANT varient;
//      PropVariantInit(&varient);
//
//      HRESULT hr = pDevice->OpenPropertyStore(STGM_READ, &pStore);
//
//      if (!SUCCEEDED(hr))
//      {
//         WinAPISafeRelease(pDevice);
//         return "";
//      }
//
//      hr = pStore->GetValue(PKEY_Device_FriendlyName, &varient);
//
//      if (!SUCCEEDED(hr))
//      {
//         WinAPISafeRelease(pStore, pDevice);
//         return "";
//      }
//
//      conversion::string toReturn = std::string("");
//
//      if (varient.pwszVal != NULL)
//      {
//         toReturn = varient.pwszVal;
//      }
//
//      PropVariantClear(&varient);
//      WinAPISafeRelease(pStore, pDevice);
//
//      return toReturn;
//   }
//
//   std::wstring _getId(IMMDevice *pDevice)
//   {
//      if (pDevice == nullptr)
//      {
//         return L"";
//      }
//
//      LPWSTR pId;
//
//      HRESULT hr = pDevice->GetId(&pId);
//
//      return SUCCEEDED(hr) ? std::wstring(pId) : L"";
//   }
//
//   DWORD _getState(IMMDevice *pDevice)
//   {
//      if (pDevice == nullptr)
//      {
//         return -1;
//      }
//
//      DWORD state;
//
//      HRESULT hr = pDevice->GetState(&state);
//
//      return SUCCEEDED(hr) ? state : -1;
//   }
//
//   std::vector<IMMDevice *> _getVecOfDevices(EDataFlow flow, DWORD state)
//   {
//      IMMDeviceEnumerator *pEnum = _getEnumerator();
//      IMMDeviceCollection *pCollection = nullptr;
//      IMMDevice *pDevice = nullptr;
//      UINT count;
//      std::vector<IMMDevice *> emptyVec;
//      auto toReturn = emptyVec;
//
//      if (pEnum == nullptr)
//      {
//         return emptyVec;
//      }
//
//      HRESULT hr = pEnum->EnumAudioEndpoints(flow, state, &pCollection);
//
//      if (!SUCCEEDED(hr))
//      {
//         WinAPISafeRelease(pEnum);
//         return emptyVec;
//      }
//
//      hr = pCollection->GetCount(&count);
//
//      if (!SUCCEEDED(hr))
//      {
//         WinAPISafeRelease(pEnum);
//         return emptyVec;
//      }
//
//      for (UINT i = 0; i < count; i++)
//      {
//         hr = pCollection->Item(i, &pDevice);
//         if (SUCCEEDED(hr))
//         {
//            toReturn.push_back(pDevice);
//         }
//      }
//
//      WinAPISafeRelease(pCollection, pEnum);
//
//      return toReturn;
//   }
//
//   bool _getBool(BOOL toConvert)
//   {
//      switch (toConvert)
//      {
//      case TRUE:
//         return true;
//      case FALSE:
//         return false;
//      default:
//         return false;
//      }
//   }
//
//   BOOL _getBool(bool toConvert)
//   {
//      switch (toConvert)
//      {
//      case true:
//         return TRUE;
//      case false:
//         return FALSE;
//      default:
//         return FALSE;
//      }
//   }
//
//   IAudioEndpointVolume *_getEndpoint(IMMDevice *pDevice)
//   {
//      IAudioEndpointVolume *pEndpoint;
//      if (pDevice == nullptr)
//      {
//         return nullptr;
//      }
//
//      HRESULT hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void **)&pEndpoint);
//
//      return SUCCEEDED(hr) ? pEndpoint : nullptr;
//   }
//
//   bool _setMute(IMMDevice *pDevice, bool statusToSet)
//   {
//      IAudioEndpointVolume *pEndpoint = _getEndpoint(pDevice);
//      if (pEndpoint == nullptr)
//      {
//         return false;
//      }
//
//      HRESULT hr = pEndpoint->SetMute(_getBool(statusToSet), NULL);
//
//      return SUCCEEDED(hr);
//   }
//
//   int _convertVolume(float rawFormat)
//   {
//      return (int)(rawFormat * (float)100);
//   }
//
//   float _convertVolume(int processedFormat)
//   {
//      return ((float)processedFormat / (float)100);
//   }
//
//   int _getVolume(IMMDevice *pDevice)
//   {
//      IAudioEndpointVolume *pEndpoint = _getEndpoint(pDevice);
//
//      if (pEndpoint == nullptr)
//      {
//         return -1;
//      }
//
//      float level;
//
//      HRESULT hr = pEndpoint->GetMasterVolumeLevelScalar(&level);
//
//      WinAPISafeRelease(pEndpoint);
//
//      return SUCCEEDED(hr) ? _convertVolume(level) : -1;
//   }
//
//   bool _setVolume(IMMDevice *pDevice, int toSet)
//   {
//      IAudioEndpointVolume *pEndpoint = _getEndpoint(pDevice);
//
//      if (pEndpoint == nullptr)
//      {
//         return false;
//      }
//
//      HRESULT hr = pEndpoint->SetMasterVolumeLevelScalar(_convertVolume(toSet), NULL);
//
//      WinAPISafeRelease(pEndpoint);
//
//      return SUCCEEDED(hr);
//   }
//} // namespace

namespace WinAPIWrap
{
   std::string toLower(std::string makeLower) 
   {
      for (auto &c : makeLower)
      {
         std::tolower(c, std::locale());
      }
      return makeLower;
   }

   std::vector<IMMDevicePtr> getVecOfDevices(EDataFlow flow, DWORD state)
   {
      std::vector<IMMDevicePtr> toReturn;
      IMMDeviceEnumeratorPtr enumerator = InjectionFramework::getEnumerator();
      IMMDeviceCollectionPtr collection;
      enumerator->EnumAudioEndpoints(flow, state, (IMMDeviceCollection **)collection.getPointerToPointerToNull());

      UINT count = 0;
      HRESULT hr = collection->GetCount(&count);

      if (!SUCCEEDED(hr))
      {
         count = 0;
      }

      for (UINT i = 0; i < count; i++)
      {
         IMMDevicePtr device;
         hr = collection->Item(i, (IMMDevice**)device.getPointerToPointerToNull());
         if (SUCCEEDED(hr))
         {
            toReturn.push_back(device);
         }
      }

      return toReturn;
   }
   IMMDeviceEnumerator *InjectionFramework::pEnum = nullptr;
   bool InjectionFramework::runningTests = false;

   std::string getName(IMMDevicePtr device)
   {
      IPropertyStorePtr propertyStore;
      std::wstring toReturn = L"";

      device->OpenPropertyStore(STGM_READ, (IPropertyStore **)propertyStore.getPointerToPointerToNull());

      PROPVARIANT variant;
      PropVariantInit(&variant);

      propertyStore->GetValue(PKEY_Device_FriendlyName, &variant);

      if (variant.pwszVal)
      {
         toReturn = variant.pwszVal;
      }

      PropVariantClear(&variant);
      return std::string(toReturn.begin(), toReturn.end());
   }
   
   std::string getNameDefault(ERole role, EDataFlow flow)
   {
      IMMDeviceEnumeratorPtr enumerator = InjectionFramework::getEnumerator();
      IMMDevicePtr device;
      enumerator->GetDefaultAudioEndpoint(flow, role, (IMMDevice **)device.getPointerToPointerToNull());

      return getName(device);
   }
   std::vector<std::string> getAllNames(EDataFlow flow, DWORD state)
   {
      std::vector<std::string> toReturn;
      
      auto devices = getVecOfDevices(flow, state);

      for (auto device : devices)
      {
         auto name = getName(device);
         if (name.size())
         {
            toReturn.push_back(name);
         }
      }

      return toReturn;
   }

   IMMDevicePtr getDevice(std::wstring id)
   {
      IMMDeviceEnumeratorPtr enumerator = InjectionFramework::getEnumerator();
      IMMDevicePtr device;
      enumerator->GetDevice(id.c_str(), (IMMDevice **)device.getPointerToPointerToNull());

      return device;
   }
   IMMDevicePtr getDevice(std::string uniqueNameIdentifier, EDataFlow flow, DWORD state)
   {
      if (uniqueNameIdentifier.size() == 0)
      {
         return IMMDevicePtr();
      }
      auto devices = getVecOfDevices(flow, state);
      IMMDevicePtr toReturn;
      for (auto device : devices)
      {
         auto deviceName = toLower(getName(device));
         if (deviceName.find(toLower(uniqueNameIdentifier)) != std::string::npos)
         {
            toReturn = device;
            break;
         }
      }

      return toReturn;
   }

   IAudioEndpointVolumePtr getEndpointVolume(IMMDevicePtr device)
   {
      IAudioEndpointVolumePtr toReturn;
      device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void **)toReturn.getPointerToPointerToNull());
      return toReturn;
   }

   IMMDeviceEnumerator * InjectionFramework::getEnumerator(DWORD dwClsContext)
   {
      if (runningTests)
      {
         if (pEnum)
         {
            return pEnum;
         }
         else
         {
            throw "Test enumerator has not ben initialized.";
         }
      }
      else
      {
         IMMDeviceEnumerator *toReturn;
         HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, dwClsContext, __uuidof(IMMDeviceEnumerator), (void **)&toReturn);
         return SUCCEEDED(hr) ? toReturn : nullptr;
      }
   }
}



//#undef WinAPISafeRelease
//
//EDataFlow TestWinAPIWrap::getFlow(std::wstring id)
//{
//   EDataFlow toReturn = EDataFlow_enum_count;
//   for (auto mic : mics)
//   {
//      if (mic.id == id)
//      {
//         toReturn = eCapture;
//         break;
//      }
//   }
//   if (toReturn == EDataFlow_enum_count)
//   {
//      for (auto speaker : speakers)
//      {
//         if (speaker.id == id)
//         {
//            toReturn = eRender;
//            break;
//         }
//      }
//   }
//
//   return toReturn;
//}
//
//int TestWinAPIWrap::getIndex(std::string name, EDataFlow flow, DWORD state)
//{
//   int i = 0;
//
//   if (devices(flow).size() == 0)
//   {
//      return DEVICE_NOT_FOUND;
//   }
//
//   for (i; i < devices(flow).size(); i++)
//   {
//      if (devices(flow).at(i).name.find(name) != std::string::npos && devices(flow).at(i).isSubsetOf(state))
//      {
//         break;
//      }
//   }
//
//   return i < devices(flow).size() ? i : DEVICE_NOT_FOUND;
//}
//
//int TestWinAPIWrap::getFirstActiveIndex(EDataFlow flow)
//{
//   return getIndex("", flow, DEVICE_STATE_ACTIVE);
//}
//
//void TestWinAPIWrap::t_addDevice(std::string name, EDataFlow flow, DWORD state)
//{
//   std::wstringstream wss;
//
//   wss << deviceCount++;
//   switch (flow)
//   {
//   case eRender:
//      speakers.push_back(Device(flow, name, wss.str(), state));
//      break;
//   case eCapture:
//      mics.push_back(Device(flow, name, wss.str(), state));
//      break;
//   default:
//      throw "invalid flow while adding devices";
//   }
//
//   //if possible, there should always be a default device.
//   if (state == DEVICE_STATE_ACTIVE && defaultIndex(flow) == DEVICE_NOT_FOUND)
//   {
//      defaultIndex(flow) = 0;
//      defaultComIndex(flow) = 0;
//   }
//}
//
//void TestWinAPIWrap::t_removeDevice(std::string name, EDataFlow flow)
//{
//   int index = getIndex(name, flow);
//   if (index == DEVICE_NOT_FOUND)
//   {
//      throw "trying to remove device that is not there";
//   }
//
//   devices(flow).erase(devices(flow).begin() + index);
//
//
//   //update default device indexes
//   if (index == defaultIndex(flow))
//   {
//      defaultIndex(flow) = getFirstActiveIndex(flow);
//   }
//   else if (defaultIndex(flow) > index)
//   {
//      defaultIndex(flow)--;
//   }
//
//   if (index == defaultComIndex(flow))
//   {
//      defaultComIndex(flow) = getFirstActiveIndex(flow);
//   }
//   else if (defaultComIndex(flow) > index)
//   {
//      defaultComIndex(flow)--;
//   }
//}
//
//void TestWinAPIWrap::t_changeProperty(std::string name, EDataFlow flow, PROPERTYKEY newProperties)
//{
//   if (getIndex(name, flow) == DEVICE_NOT_FOUND)
//   {
//      throw "trying to change properties of device that does not exist";
//   }
//   devices(flow).at(getIndex(name, flow)).properties = newProperties;
//}
//
//HRESULT TestWinAPIWrap::t_callChangeProperties(std::string name, EDataFlow flow)
//{
//   if (notifier == nullptr)
//   {
//      throw "trying to call \'callChangeProperties\" when notifier is not registered";
//   }
//
//   int index = getIndex(name, flow);
//
//   if (index == DEVICE_NOT_FOUND)
//   {
//      throw "trying to call \'callChangeProperties\" for device that does not exist";
//   }
//
//   return notifier->OnPropertyValueChanged(devices(flow).at(index).id.c_str(), devices(flow).at(index).properties);
//}
//
//void TestWinAPIWrap::t_changeState(std::string name, EDataFlow flow, DWORD newState)
//{
//   int index = getIndex(name, flow);
//   if (index == DEVICE_NOT_FOUND)
//   {
//      throw "Trying to change state of device that does not exist";
//   }
//
//   devices(flow).at(index).state = newState;
//   return;
//}
//
//HRESULT TestWinAPIWrap::t_callChangeState(std::string name, EDataFlow flow)
//{
//   if (notifier == nullptr)
//   {
//      throw "trying to call \"callChangeState\" without setting IMMNotificationClient.";
//   }
//
//   int index;
//   if (DEVICE_NOT_FOUND == (index = getIndex(name, flow)))
//   {
//      throw "trying to call \"callChangeState\" for device that does not exist";
//   }
//
//   return notifier->OnDeviceStateChanged(devices(flow).at(index).id.c_str(), devices(flow).at(index).state);
//}
//
//void TestWinAPIWrap::t_changeDefault(std::string name, EDataFlow flow, ERole role)
//{
//   int index;
//   if (DEVICE_NOT_FOUND == (index = getIndex(name, flow, DEVICE_STATE_ACTIVE)))
//   {
//      throw "Trying to change default device to a device that either does not exist, or is not currently active.";
//   }
//
//   switch (role)
//   {
//   case eMultimedia:
//   case eConsole:
//      defaultIndex(flow) = index;
//      break;
//   case eCommunications:
//      defaultComIndex(flow) = index;
//   default:
//      defaultIndex(flow) = index;
//      defaultComIndex(flow) = index;
//      break;
//   }
//
//   return;
//}
