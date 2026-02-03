#include "ViessmannClient.h"
#include "ViessmannApiSelection.h"
#include "config.h"
#include "ArduinoJson.h"

typedef int t_httpCode;

// Constructor
ViessmannClient::ViessmannClient(ViessmannApiAccount * account, const char * caCert, HTTPClient * httpClient, WiFiClient * wifiClient, uint8_t * bufferStorePtr)
{  
    _viessmannAccountPtr = account;
    _viessmannCaCert = (char *)caCert;
    _viessmannWifiClient = wifiClient;  
    _viessmannHttpPtr = httpClient;   
    _viessmannHttpPtr -> setReuse(false);
    
}

#pragma region ViessmannClient::GetFeatures(...)
t_httpCode ViessmannClient::GetFeatures(uint8_t* responseBuffer, const uint16_t reponseBufferLength, const uint32_t data_0_id, const char * gateways_0_serial, const char * gateways_0_devices_0_id, ViessmannApiSelection * apiSelectionPtr)
{
    Serial.println("Viessmann Client start getting features\n");
    //Serial.printf("In ViessmannClient. LastReadTimeSeconds: %u Interval: %u \n ", (uint32_t)apiSelectionPtr ->lastReadTimeSeconds, apiSelectionPtr ->readIntervalSeconds);
    char InstallationId[20] = {0};
    sprintf(InstallationId, "%d", data_0_id);

    char GatewaySerial[30] = {0};
    
    String addendum = "features/installations/" + (String)InstallationId + "/gateways/" + (String(gateways_0_serial) + "/devices/" + String(gateways_0_devices_0_id) + "/features"); 
    String Url = _viessmannAccountPtr -> UriEndPointIot + addendum;
    String authorizationHeader = "Bearer " + _viessmannAccountPtr ->AccessToken;
    //Serial.println(F("Loading Viessmann features from Cloud"));
    
    const int nameLen = VI_FEATURENAMELENGTH;
    const int stampLen = VI_FEATURESTAMPLENGTH;
    const int valLen = VI_FEATUREVALUELENGTH;

    //#if SERIAL_PRINT == 1
    //Serial.printf("VI-WiFiClient Address: %d\n\n", &_viessmannWifiClient);
    //#endif
    
    #if SERIAL_PRINT == 1
        Serial.println(Url);
    #endif

    //https://arduinojson.org/v7/how-to/use-arduinojson-with-httpclient/
    
    
    _viessmannHttpPtr ->useHTTP10(true);   // Must be reset to false for Azure requests
                                            // Is needed to load the long features JSON string

    //Serial.println(F("Set httpClient to true"));
    #if SERIAL_PRINT == 1
    Serial.printf("Free heapsize: %d Minimum: %d\n", esp_get_free_heap_size(), esp_get_minimum_free_heap_size());
    #endif
     // Setting of timeout possibly needed to avoid 'IncompleteInput' errors (not sure)
           
     _viessmannWifiClient ->setTimeout(10);  //default of WiFiClientSecure is 5000 ms
                                            // here it is set to 7000 ms
                                            // later (7000 not sufficient) set to 10000 ms
     
     uint32_t wiFiSecureTimeOut = _viessmannWifiClient ->getTimeout();
     #if SERIAL_PRINT == 1
        Serial.printf("WiFiClientSecure TimeOut = %d\n", wiFiSecureTimeOut);
     #endif

     _viessmannHttpPtr ->begin(*_viessmannWifiClient, Url);
    
    _viessmannHttpPtr ->addHeader("Authorization", authorizationHeader);
             
    t_httpCode httpResponseCode = _viessmannHttpPtr ->GET();
                
    if (httpResponseCode > 0) 
    { 
       #if SERIAL_PRINT == 1
            Serial.println(F("Viessmann Received ResponseCode > 0"));
       #endif

        if (httpResponseCode == HTTP_CODE_OK)
        {     
            #if SERIAL_PRINT == 1
              Serial.println(F("Viessmann Received ResponseCode OK"));
            #endif
             
            JsonDocument doc;
            StaticJsonDocument<64> filter;           
            filter["data"][0]["feature"] = true,
            filter["data"][0]["timestamp"] = true,
            filter["data"][0]["properties"] = true 
            ;

            DeserializationError error; 
            
            WiFiClient* stream = _viessmannHttpPtr->getStreamPtr();
            // Setting of timeout seems to be needed to avoid 'IncompleteInput' errors
            // Not sure if the argument plays a role, seems to work with '1' as well
            stream->setTimeout(2);  
                          
            #if SERIAL_PRINT == 1
                ReadLoggingStream loggingStream(*stream, Serial);  //use this to print the JSON string
            #else
               NullPrint nullPrint;
               ReadLoggingStream loggingStream(*stream, nullPrint);
            #endif
                
            //error = deserializeJson(doc, _viessmannHttpPtr->getStream(), DeserializationOption::Filter(filter));
            error = deserializeJson(doc, loggingStream, DeserializationOption::Filter(filter));
            // RoSchmi                                           
            if (error == DeserializationError::Ok) // Ok; EmptyInput; IncompleteInput; InvalidInput; NoMemory                       
            {
            #pragma region if(DeserializationError::Ok)    
               #if SERIAL_PRINT == 1
                Serial.println(F("JsonDoc is deserialized"));
               #endif

                char tempVal[valLen] = {'\0'};
            
                if (!doc.overflowed())
                {
                   #if SERIAL_PRINT == 1
                    Serial.printf("Number of elements = %d\n", doc.size());
                   #endif
                    // From the long Features JSON string get some chosen entities
                    
                    
                    apiSelectionPtr -> extractFeatures(doc, vi_features, ViessmannApiSelection::NUM_INTERESTING_PROPERTIES);
                    
                    #if SERIAL_PRINT == 1
                    Serial.printf("\r\nKey and Value of %s is '%s' : %s\n", vi_features[0].name, vi_features[0].values[0].key, vi_features[0].values[0].value);
                    Serial.printf("\r\nKey and Value of %s is '%s' : %s\n", vi_features[1].name, vi_features[1].values[0].key, vi_features[1].values[0].value);
                    Serial.printf("\r\nKey and Value of %s is '%s' : %s\n", vi_features[2].name, vi_features[2].values[0].key, vi_features[2].values[0].value);
                    Serial.printf("\r\nKey and Value of %s is '%s' : %s\n", vi_features[3].name, vi_features[3].values[0].key, vi_features[3].values[0].value);
                    Serial.printf("\r\nKey and Value of %s is '%s' : %s\n", vi_features[4].name, vi_features[4].values[0].key, vi_features[4].values[0].value);
                    Serial.printf("\r\nKey and Value of %s is '%s' : %s\n", vi_features[5].name, vi_features[5].values[0].key, vi_features[5].values[0].value);
                    Serial.printf("\r\nKey and Value of %s is '%s' : %s\n", vi_features[6].name, vi_features[6].values[0].key, vi_features[6].values[0].value);
                    Serial.printf("\r\nKey and Value of %s is '%s' : %s\n", vi_features[6].name, vi_features[6].values[1].key, vi_features[6].values[1].value);                    
                    Serial.printf("\r\nKey and Value of %s is '%s' : %s\n", vi_features[7].name, vi_features[7].values[0].key, vi_features[7].values[0].value);
                    Serial.printf("\r\nKey and Value of %s is '%s' : %s\n", vi_features[8].name, vi_features[8].values[0].key, vi_features[8].values[0].value);
                    Serial.printf("\r\nKey and Value of %s is '%s' : %s\n", vi_features[9].name, vi_features[9].values[0].key, vi_features[9].values[0].value);
                    Serial.printf("\r\nKey and Value of %s is '%s' : %s\n", vi_features[10].name, vi_features[10].values[0].key, vi_features[10].values[0].value);
                    Serial.printf("\r\nKey and Value of %s is '%s' : %s\n", vi_features[11].name, vi_features[11].values[0].key, vi_features[11].values[0].value);                                
                    #endif
                }
                else
                {
                    Serial.println(F("Deserialization doc was overflowed"));
                }
            #pragma endregion
            }
            else
            {
            #pragma region else (DeserializationError::Not Ok)
                Serial.printf("\nDeserializeJson() failed: %s\n\n", (const char *)error.c_str());
                Serial.println("Handled the same way as a -1 response");   
                
                
                // Set httpResponseCode to -1
                // so it is handled as a bad request/response
                //RoSchmi
                //httpResponseCode = -1;
            #pragma endregion           
            }
            doc.clear();   
        }
        else
        {
            // If request doesn't return with ok, we give back 
            // the begin of the response string in responseBuffer        
            Serial.printf("Viessmann Received ResponseCode %d\n",httpResponseCode);
            WiFiClient *stream = _viessmannHttpPtr ->getStreamPtr();                  
            uint32_t bytesRead = 0;
            uint32_t totalBytesRead = 0;
            uint32_t chunkSize = 1000;                  
            char chunkBuffer[chunkSize +1] = {0};
            
            while (stream->connected() || stream->available()) 
            {
                if (stream->available()) 
                {
                  // Read data in chunks
                  size_t bytesRead = stream->readBytes(chunkBuffer, chunkSize);
                  
                  // Write data to buffer
                  for (size_t i = 0; i < bytesRead; i++) 
                  {
                    if (totalBytesRead < reponseBufferLength) 
                    {
                      responseBuffer[totalBytesRead++] = chunkBuffer[i];
                    } 
                    else 
                    {
                      Serial.println("responseBuffer is full!");
                      break;
                    }
                  }
                  #if SERIAL_PRINT == 1                                 
                  Serial.write(chunkBuffer, bytesRead);
                  Serial.println();
                  #endif
                }
              }
              responseBuffer[reponseBufferLength -1] = '\0';                        
        }                      
    } 
    else 
    {    
        Serial.printf("Vi-Features: Error performing the request, HTTP-Code: %d\n", httpResponseCode);
    }
    
    _viessmannHttpPtr ->useHTTP10(false);   
    _viessmannHttpPtr->end();    
    return httpResponseCode;
}
#pragma endregion

#pragma region ViessmannClient::RefreshAccessToken(...)
t_httpCode ViessmannClient::RefreshAccessToken(uint8_t* responseBuffer, const uint16_t reponseBufferLength, const char * refreshToken)
{ 
        String body = "grant_type=refresh_token&client_id=" + (String)_viessmannAccountPtr ->ClientId + "&refresh_token=" + (String)refreshToken; 
           
        String Url = _viessmannAccountPtr -> UriEndPointToken;

        //Url = "https://iam.viessmann.com/idp/v3/token";
    
        #if SERIAL_PRINT == 1
            Serial.println(Url);
            Serial.println(body);
        #endif

        _viessmannHttpPtr ->begin(*_viessmannWifiClient, Url);

        _viessmannHttpPtr ->addHeader("Content-Type", "application/x-www-form-urlencoded; charset=utf-8");
    
        t_httpCode httpResponseCode =_viessmannHttpPtr ->POST((String)body);
        if (httpResponseCode > 0) 
        {
            #if SERIAL_PRINT == 1
            Serial.printf("Refresh Token: ResponseCode is: %d\n", httpResponseCode);
            #endif

            String payload = _viessmannHttpPtr ->getString();
            
            #if SERIAL_PRINT == 1
                Serial.println("Got payload:\n");
                Serial.println(payload);
            #endif

            int charsToCopy = payload.length() < reponseBufferLength ? payload.length() : reponseBufferLength;
            for (int i = 0; i < charsToCopy; i++)
            {
                responseBuffer[i] = payload[i];
            } 
        } 
        else 
        {
            Serial.printf("Refresh token: Error performing the request, HTTP-Code: %d\n", httpResponseCode);
            if (httpResponseCode == HTTPC_ERROR_CONNECTION_REFUSED)
            {
                Serial.println(F("Viessmann Server: Connection refused"));
                delay(500);          
            }
        }
        _viessmannHttpPtr->end();  
    
    return httpResponseCode;
}
#pragma endregion

#pragma region ViessmannClient::GetEquipment(...)
t_httpCode ViessmannClient::GetEquipment(uint8_t* responseBuffer, const uint16_t reponseBufferLength)
{
    String Url = _viessmannAccountPtr -> UriEndPointIot + "equipment/installations" + "?includeGateways=true"; 
    String authorizationHeader = "Bearer " + _viessmannAccountPtr ->AccessToken;
    Serial.println(Url);
    _viessmannHttpPtr ->begin(*_viessmannWifiClient, Url);
    _viessmannHttpPtr ->addHeader("Authorization", authorizationHeader);
    t_httpCode httpResponseCode = _viessmannHttpPtr ->GET();   
    if (httpResponseCode > 0) 
    {
        String payload = _viessmannHttpPtr ->getString();      
        int charsToCopy = payload.length() < reponseBufferLength ? payload.length() : reponseBufferLength;
        for (int i = 0; i < charsToCopy; i++)
        {
            responseBuffer[i] = payload[i];
        }               
    } 
    else 
    {
        Serial.printf("Eqipment: Error performing the request, HTTP-Code: %d\n", httpResponseCode);
    }
    _viessmannHttpPtr->end();
    return httpResponseCode;
}
#pragma endregion 

#pragma region ViessmannClient::GetUser(...)
t_httpCode ViessmannClient::GetUser(uint8_t* responseBuffer, const uint16_t reponseBufferLength)
{
    String Url = _viessmannAccountPtr -> UriEndPointUser + "?sections=identity"; 

    String authorizationHeader = "Bearer " + _viessmannAccountPtr ->AccessToken;
    Serial.println(Url);

    _viessmannHttpPtr ->begin(*_viessmannWifiClient,Url);
    _viessmannHttpPtr ->addHeader("Authorization", authorizationHeader);
    t_httpCode httpResponseCode = _viessmannHttpPtr ->GET();   
    if (httpResponseCode > 0) 
    {
        String payload = _viessmannHttpPtr ->getString();      
        int charsToCopy = payload.length() < reponseBufferLength ? payload.length() : reponseBufferLength;
        for (int i = 0; i < charsToCopy; i++)
        {
            responseBuffer[i] = payload[i];
        }               
    } 
    else 
    {
        Serial.printf("Fehler bei der Anfrage, HTTP-Code: %d\n", httpResponseCode);
    }
    _viessmannHttpPtr->end();  
    return httpResponseCode; 
}
#pragma endregion
