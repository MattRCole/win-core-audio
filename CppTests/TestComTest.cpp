#pragma once
#include "TestComTest.h"

//We are testing
#include "../src/ComTestInstanciations.h"

//Test Dependencies
#include "../src/WinAPIWrap.h"

SCENARIO("TestAudioEndpoint should act as closely to IAudioEndpointVolume as we currently need")
{
   //pre-setup
   WinAPIWrap::InjectionFramework::initTests(NULL);
   GIVEN("A TestAudioEndpoint")
   {
      ComTest::TestAudioEndpointVolume testEndpoint;

      WHEN("WinAPIWrap::InjectionFramework::ComInitialize has not been called")
      {
         THEN("All (instanciated) calls will return an HRESULT of CO_E_NOTINITIALIZED and will not succeed")
         {
            HRESULT hr = testEndpoint.RegisterControlChangeNotify(NULL);
            REQUIRE(hr == CO_E_NOTINITIALIZED);
            hr = testEndpoint.UnregisterControlChangeNotify(NULL);
            REQUIRE(hr == CO_E_NOTINITIALIZED);
            hr = testEndpoint.SetMasterVolumeLevelScalar(0, NULL);
            REQUIRE(hr == CO_E_NOTINITIALIZED);
            hr = testEndpoint.GetMasterVolumeLevelScalar(NULL);
            REQUIRE(hr == CO_E_NOTINITIALIZED);
            hr = testEndpoint.SetMute(FALSE, NULL);
            REQUIRE(hr == CO_E_NOTINITIALIZED);
            hr = testEndpoint.GetMute(NULL);
            REQUIRE(hr == CO_E_NOTINITIALIZED);
         }
      }
         
   }
}