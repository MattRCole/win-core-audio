#include "WinAPIWrap.h"
#include "common.h"
#include <string>
#include <sstream>
#include <mmdeviceapi.h>
#include <vector>
#include <functiondiscoverykeys_devpkey.h>
#include <endpointvolume.h>
#include <locale>

namespace WinAPIWrap
{
   IMMDeviceEnumerator *InjectionFramework::pEnum = nullptr;
   bool InjectionFramework::runningTests = false;
   int InjectionFramework::initCount = int(0);

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

   std::wstring getId(IMMDevicePtr device)
   {
      LPWSTR pId = NULL;
      device->GetId(&pId);
      std::wstring id;
      if (pId)
      {
         id = std::wstring(pId);
      }
      return id;
   }

   std::vector<std::wstring> getAllIds(EDataFlow flow, DWORD state)
   {
      auto devices = getVecOfDevices(flow, state);
      std::vector<std::wstring> ids;
      for(auto device : devices)
      {
         auto id = getId(device);
         if(id.size())
         {
            ids.push_back(id);
         }
      }
      return ids;
   }

   IAudioEndpointVolumePtr getEndpointVolume(IMMDevicePtr device)
   {
      IAudioEndpointVolumePtr toReturn;
      device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void **)toReturn.getPointerToPointerToNull());
      return toReturn;
   }

   IAudioEndpointVolumePtr getEndpointVolume(std::wstring id)
   {
      return getEndpointVolume(getDevice(id));
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
