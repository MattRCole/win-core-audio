#pragma once
#include "TestComTest.h"

//We are testing
#include "../src/ComTestInstanciations.h"

//Test Dependencies
#include "../src/WinAPIWrap.h"

class SimpleControlChangeNotify : public IAudioEndpointVolumeCallback {
public:
   int timesCalled;
   SimpleControlChangeNotify() : timesCalled(0) {}
   ULONG AddRef() { return -1; }
   ULONG Release() { return -1; }
   HRESULT QueryInterface(const IID &riid, void ** ppvObject) { *ppvObject = NULL; return CO_E_NOTINITIALIZED; }
   HRESULT OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify) { timesCalled++; return S_OK; }
};

SCENARIO("TestAudioEndpoint should act as closely to IAudioEndpointVolume as we currently need")
{
   //pre-setup
   WinAPIWrap::InjectionFramework::initTests(NULL);
   GIVEN("A TestAudioEndpoint when CoInitialize has not been called")
   {
      ComTest::TestAudioEndpointVolume endpoint;
      //We will treat the test endpoint as a pointer since it is technically supposed to be an interface.
      ComTest::TestAudioEndpointVolume * testEndpoint = &endpoint;
      WHEN("WinAPIWrap::InjectionFramework::ComInitialize has not been called")
      {
         THEN("All (instanciated) calls will return an HRESULT of CO_E_NOTINITIALIZED and will not succeed")
         {
            HRESULT hr = testEndpoint->RegisterControlChangeNotify(NULL);
            REQUIRE(hr == CO_E_NOTINITIALIZED);
            hr = testEndpoint->UnregisterControlChangeNotify(NULL);
            REQUIRE(hr == CO_E_NOTINITIALIZED);
            hr = testEndpoint->SetMasterVolumeLevelScalar(0, NULL);
            REQUIRE(hr == CO_E_NOTINITIALIZED);
            hr = testEndpoint->GetMasterVolumeLevelScalar(NULL);
            REQUIRE(hr == CO_E_NOTINITIALIZED);
            hr = testEndpoint->SetMute(FALSE, NULL);
            REQUIRE(hr == CO_E_NOTINITIALIZED);
            hr = testEndpoint->GetMute(NULL);
            REQUIRE(hr == CO_E_NOTINITIALIZED);
         }
      }
         
   }

   WinAPIWrap::InjectionFramework::ComInitialize();

   GIVEN("A Test AudioEndpoint when CoInitialize has been called")
   {
      ComTest::TestAudioEndpointVolume endpoint;
      ComTest::TestAudioEndpointVolume * testEndpoint = &endpoint;
      WHEN("RegisterContrlChangeNotify is called with an null pointer")
      {
         HRESULT hr = testEndpoint->RegisterControlChangeNotify(NULL);
         THEN("The return value will be E_POINTER")
         {
            REQUIRE(hr == E_POINTER);
         }
      }
      WHEN("RegisterControlChangeNotify is called with a non-null pointer")
      {
         SimpleControlChangeNotify ccNotify;
         HRESULT hr = testEndpoint->RegisterControlChangeNotify(&ccNotify);
         THEN("The return value will be S_OK")
         {
            REQUIRE(hr == S_OK);
         }
      }

      WHEN("SetMasterVolumeLevelScalar is called and passed a value between 1 and 0 (includes)")
      {
         HRESULT hr = testEndpoint->SetMasterVolumeLevelScalar(0.5f, NULL);
         THEN("The return value is S_OK")
         {
            REQUIRE(hr == S_OK);
         }
      }

      WHEN("SetMasterVolumeLevelScalar is called and passed a value that is less than zero")
      {
         HRESULT hr = testEndpoint->SetMasterVolumeLevelScalar(-0.01f, NULL);
         THEN("The return value is E_INVALIDARG")
         {
            REQUIRE(hr == E_INVALIDARG);
         }
      }

      WHEN("SetMasterVolumeLevelScalar is called and passed a value that is greater than zero")
      {
         HRESULT hr = testEndpoint->SetMasterVolumeLevelScalar(1.1f, NULL);
         THEN("The return value is E_INVALIDARG")
         {
            REQUIRE(hr == E_INVALIDARG);
         }
      }

      WHEN("TestAudioEndpointVolume is passed an IAudioEndpointVolumeCallback and SetMasterVolumeLevelScalar is called")
      {
         SimpleControlChangeNotify ccNotify;
         auto hr = testEndpoint->RegisterControlChangeNotify(&ccNotify);
         THEN("If the float parameter is between 1 and 0, IAudioEndpointVolumeCallback registers one callback")
         {
            hr = testEndpoint->SetMasterVolumeLevelScalar(0.5f, NULL);
            REQUIRE(ccNotify.timesCalled == 1);
         }

         THEN("If the float parameter is lower than 0, IAudioEndpointVolumeCallback will not register a callback")
         {
            hr = testEndpoint->SetMasterVolumeLevelScalar(-0.5f, NULL);
            REQUIRE(ccNotify.timesCalled == 0);
         }

         THEN("If the float parameter is greater than 1, IAudioEndpointVolumeCallback will not register a callback")
         {
            hr = testEndpoint->SetMasterVolumeLevelScalar(1.5f, NULL);
            REQUIRE(ccNotify.timesCalled == 0);
         }
      }

      WHEN("GetMasterVolumeLevelScalar is called with NULL as its parameter")
      {
         auto hr = testEndpoint->GetMasterVolumeLevelScalar(NULL);
         THEN("The return value is E_POINTER")
         {
            REQUIRE(hr == E_POINTER);
         }
      }

      WHEN("GetMasterVolumeLevelScalar is called with a valid pointer to a float as its parameter")
      {
         float floatArg;
         auto hr = testEndpoint->GetMasterVolumeLevelScalar(&floatArg);
         THEN("The return value is S_OK")
         {
            REQUIRE(hr == S_OK);
         }
         const float NEW_VOLUME = 0.66f;
         testEndpoint->SetMasterVolumeLevelScalar(NEW_VOLUME, NULL);
         hr = testEndpoint->GetMasterVolumeLevelScalar(&floatArg);

         THEN("The return value contains the current volume")
         {
            REQUIRE(floatArg == NEW_VOLUME);
         }
      }

      WHEN("SetMute is called")
      {
         auto hr = testEndpoint->SetMute(FALSE, NULL);
         THEN("The return value is S_OK")
         {
            REQUIRE(hr == S_OK);
         }
         hr = testEndpoint->SetMute(TRUE, NULL);
         THEN("The return value is S_OK")
         {
            REQUIRE(hr == S_OK);
         }
      }

      WHEN("SetMute is called and an IAudioEndpointVolumeCallback is registered")
      {
         SimpleControlChangeNotify ccNotify;
         testEndpoint->RegisterControlChangeNotify(&ccNotify);
         auto hr = testEndpoint->SetMute(FALSE, NULL);
         THEN("IAudioEndpointVolumeCallback registers one callback")
         {
            REQUIRE(ccNotify.timesCalled == 1);
         }
         hr = testEndpoint->SetMute(TRUE, NULL);
         THEN("IAudioEndpointVolumeCallback registers one more callback")
         {
            REQUIRE(ccNotify.timesCalled == 2); 
         }
      }

      WHEN("GetMute is called with a null pointer as the BOOL parameter")
      {
         auto hr = testEndpoint->GetMute(NULL);
         THEN("The return value is E_POINTER")
         {
            REQUIRE(hr == E_POINTER);
         }
      }

      WHEN("GetMute is called with a pointer to a BOOL data type")
      {
         BOOL isMuted;
         auto hr = testEndpoint->GetMute(&isMuted);
         THEN("The return value is S_OK")
         {
            REQUIRE(hr == S_OK);
         }

         testEndpoint->SetMute(TRUE, NULL);
         testEndpoint->GetMute(&isMuted);

         THEN("The parameter contains the current mute status")
         {
            REQUIRE(isMuted == TRUE);
         }

         testEndpoint->SetMute(FALSE, NULL);
         testEndpoint->GetMute(&isMuted);

         THEN("The parameter contains the current mute status")
         {
            REQUIRE(isMuted == FALSE);
         }
      }

      //TODO: write tests for UnregisterControlChangeNotify
      //WHEN UnregisterControlChangeNotify is passed a null pointer
      WHEN("UnregisterControlChangeNotify is passed a null pointer")
      {
         auto hr = testEndpoint->UnregisterControlChangeNotify(nullptr);
            //THEN the return value is E_POINTER
         THEN("the return value is E_POINTER")
         {
            REQUIRE(hr == E_POINTER);
         }
      }

      //When UnregisterControlChangeNotify is passed a pointer to an object that is not registered
      WHEN("UnregisterControlChangeNotify is passed a pointer to an object that is not registered")
      {
         SimpleControlChangeNotify ccNotify;
         auto hr = testEndpoint->UnregisterControlChangeNotify(&ccNotify);
         //THEN the return value is E_INVALIDARG
         THEN("the return value is E_INVALIDARG")
         {
            REQUIRE(hr == E_INVALIDARG);
         }
      }

      //When UnregisterControlChangeNotify is passed a pointer to a registered object
      WHEN("UnregisterControlChangeNotify is passed a pointer to a registered object")
      {
         SimpleControlChangeNotify ccNotify;
         auto hr = testEndpoint->RegisterControlChangeNotify(&ccNotify);
         hr = testEndpoint->UnregisterControlChangeNotify(&ccNotify);
         //THEN the return value is S_OK
         THEN("the return value is S_OK")
         {
            REQUIRE(hr == S_OK);
         }
         //THEN no errors are thrown when a different function of TestAudioEndpointVolume is called(This means the object has been unregistered)
         THEN("no errors are thrown when a different function of TestAudioEndpointVolume is called(This means the object has been unregistered)")
         {
            REQUIRE_NOTHROW(testEndpoint->SetMute(FALSE, NULL));
            REQUIRE_NOTHROW(testEndpoint->SetMasterVolumeLevelScalar(1.0f, NULL));
         }
      }
   }
}

SCENARIO("Given a TestPropertyStore, it should act close enough to IPropertyStore for testing purposes")
{
   WinAPIWrap::InjectionFramework::initTests(NULL);
   std::wstring name = L"Test Device";
   ComTest::TestPropertyStore store(name);
   ComTest::TestPropertyStore * testStore = &store;
   GIVEN("A state where WinAPIWrap::InjectionFramework::ComInitialize() has not been called")
   {
      WHEN("GetValue is called")
      {
         auto hr = testStore->GetValue(PKEY_Device_FriendlyName, NULL);
         THEN("The return value is CO_E_NOTINITIALIZED")
         {
            REQUIRE(hr == CO_E_NOTINITIALIZED);
         }
      }
   }

   WinAPIWrap::InjectionFramework::ComInitialize();
   GIVEN("A state where WinAPIWrap::InjectionFramework::ComInitialize() has been called")
   {
      WHEN("GetValue is called with a value that is not PKEY_Device_FriendlyName")
      {
         auto hr = testStore->GetValue(PKEY_Device_Numa_Node, NULL);
         THEN("The return value is going to be E_NOTIMPL")
         {
            REQUIRE(hr == E_NOTIMPL);
         }
      }

      WHEN("GetValue is called with a valid parameter for REFPROPTERYKEY and a nullpointer as the PROPVARIANT parameter")
      {
         auto hr = testStore->GetValue(PKEY_Device_FriendlyName, NULL);
         THEN("the return value will be E_POINTER")
         {
            REQUIRE(hr == E_POINTER);
         }
      }

      WHEN("GetValue is called with a valid parameter for REFPROPERTYKEY and a valid PROPVARIANT Pointer")
      {
         PROPVARIANT variant;
         PropVariantInit(&variant);
         testStore->GetValue(PKEY_Device_FriendlyName, &variant);
         THEN("PROPVARIANT::pwszVal will contain a pointer to a string equaling the initialized name of TestProptertyStore")
         {
            REQUIRE(name == std::wstring(variant.pwszVal));
         }
         PropVariantClear(&variant);
      }
   }
}

SCENARIO("TestDevice will behave enough like IMMDevice for testing purposes")
{
   WinAPIWrap::InjectionFramework::initTests(NULL);
   ComTest::TestDevice device;
   auto * testDevice = &device;

   GIVEN("That WinAPIWrap::InjectionFramework::ComInitialize() has not been called")
   {
      WHEN("An instanciated function of TestDevice is called")
      {
         THEN("The return value is CO_E_NOTINITIALIZED")
         {
            REQUIRE(testDevice->GetState(NULL) == CO_E_NOTINITIALIZED);
         }
      }
   }

   WinAPIWrap::InjectionFramework::ComInitialize();
   GIVEN("That WinAPIWrap::InjectionFramework::ComInitialize() has been called")
   {
      WHEN("GetState is called with nullpointer")
      {
         auto hr = testDevice->GetState(NULL);
         THEN("return value is E_POINTER")
         {
            REQUIRE(hr == E_POINTER);
         }
      }
   }
}