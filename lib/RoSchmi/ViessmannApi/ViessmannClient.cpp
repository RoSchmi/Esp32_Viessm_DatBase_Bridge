#include "ViessmannClient.h"

WiFiClient * _viessmannWifiClient;

ViessmannApiAccount  * _viessmannAccountPtr;
HTTPClient * _viessmannHttpPtr;

const char * _viessmannCaCert;

typedef int t_httpCode;
/*
uint8_t * _requestPtr;
uint8_t * _propertiesPtr;
uint8_t * _responsePtr;
uint8_t * _authorizationHeaderBufferPtr;
*/

// Constructor
ViessmannClient::ViessmannClient(ViessmannApiAccount * account, const char * caCert, HTTPClient * httpClient, WiFiClient * wifiClient, uint8_t * bufferStorePtr)
{
    
    _viessmannAccountPtr = account;
    _viessmannCaCert = caCert;
    _viessmannHttpPtr = httpClient;
    _viessmannWifiClient = wifiClient;

    // Some buffers located in memory segment .dram0.bss are injected to achieve lesser stack consumption
    
    /*
    _requestPtr = bufferStorePtr;
    _propertiesPtr = bufferStorePtr + REQUEST_BODY_BUFFER_LENGTH;
    _authorizationHeaderBufferPtr = bufferStorePtr + REQUEST_BODY_BUFFER_LENGTH + PROPERTIES_BUFFER_LENGTH;
    _responsePtr = bufferStorePtr + REQUEST_BODY_BUFFER_LENGTH + PROPERTIES_BUFFER_LENGTH + AUTH_HEADER_BUFFER_LENGTH;
    */

    /*
   _accountPtr = account;
    _caCert = caCert;
    _httpPtr = httpClient;
    _wifiClient = wifiClient;
    */
    /*
       char responseString[RESPONSE_BUFFER_LENGTH];
       memset(&(responseString[0]), 0, RESPONSE_BUFFER_LENGTH);
    */
}



t_httpCode ViessmannClient::GetFeatures(const uint32_t data_0_id, const char * gateways_0_serial, const char * gateways_0_devices_0_id, ViessmannApiSelection * apiSelectionPtr)
{
    char InstallationId[20] = {0};
    sprintf(InstallationId, "%d", data_0_id);

    char GatewaySerial[30] = {0};


    String addendum = "features/installations/" + (String)InstallationId + "/gateways/" + (String(gateways_0_serial) + "/devices/" + String(gateways_0_devices_0_id) + "/features"); 
    String encodedUrl = _viessmannAccountPtr -> UriEndPointIot + addendum;
    String authorizationHeader = "Bearer " + _viessmannAccountPtr ->AccessToken;
    Serial.println(encodedUrl);

    //https://arduinojson.org/v7/how-to/use-arduinojson-with-httpclient/

    _viessmannHttpPtr ->useHTTP10(true);
    _viessmannHttpPtr ->begin(encodedUrl);
    _viessmannHttpPtr ->addHeader("Authorization", authorizationHeader);
    t_httpCode httpResponseCode = _viessmannHttpPtr ->GET();   
    if (httpResponseCode > 0) 
    { 
        //char newStr[50] = {0};
        JsonDocument doc;
        //deserializeJson(doc, String("Hello" ));
        //deserializeJson(doc, newStr );

        StaticJsonDocument<64> filter;
        filter["data"][0]["feature"] = true,
        filter["data"][0]["timestamp"] = true,
        filter["data"][0]["properties"] = true 
        ;
        deserializeJson(doc, _viessmannHttpPtr ->getStream(),DeserializationOption::Filter(filter));
         
        
        
        //const char * data_0_feature = doc["data"][0]["feature"];
        //const char * data_1_feature = doc["data"][1]["feature"];
        //const char * data_3_feature = doc["data"][3]["feature"];
        //const char * data_3_timestamp = doc["data"][3]["timestamp"];
        //const char * data_20_feature = doc["data"][20]["feature"];
        //const char * data_40_feature = doc["data"][40]["feature"];
        //const char * data_50_feature = doc["data"][50]["feature"];
        //const char * data_60_feature = doc["data"][60]["feature"];
        //const char * data_70_feature = doc["data"][70]["feature"];
        //const char * data_77_feature = doc["data"][77]["feature"];
        const char * data_91_feature = doc["data"][91]["feature"];        
        const char * data_91_unit    = doc["data"][91]["properties"]["value"]["unit"];
        float data_91_value    = doc["data"][91]["properties"]["value"]["value"];
        const char * data_95_feature = doc["data"][95]["feature"];        
        const char * data_95_unit    = doc["data"][95]["properties"]["value"]["unit"];
        float data_95_value    = doc["data"][95]["properties"]["value"]["value"];


     
        /*
        Serial.println("data_0_feature:");
        Serial.println(data_0_feature);
        Serial.println("data_1_feature:");
        Serial.println(data_1_feature);
        Serial.println("data_3_feature:");
        Serial.println(data_3_feature);
        Serial.println("data_20_feature:");
        Serial.println(data_20_feature);
        Serial.println("data_40_feature:");
        Serial.println(data_40_feature);
        */
        
        /*
        Serial.println("data_50_feature:");
        Serial.println(data_50_feature);
        Serial.println("data_60_feature:");
        Serial.println(data_60_feature);
        Serial.println("data_70_feature:");
        Serial.println(data_70_feature);
        Serial.println("data_77_feature:");
        Serial.println(data_77_feature);
        */
        Serial.println("data_91_feature:");
        Serial.println(data_91_feature);
        Serial.println("data_91_unit:");
        Serial.println(data_91_unit);
        Serial.println("data_91_value:");
        Serial.println(data_91_value);
        Serial.println("data_95_feature:");
        Serial.println(data_95_feature);
        Serial.println("data_95_unit:");
        Serial.println(data_95_unit);
        Serial.println("data_95_value:");
        Serial.println(data_95_value);
        
        
        int nameLen = apiSelectionPtr ->nameLenght;
        int stampLen = apiSelectionPtr -> stampLength;
        int valLen = apiSelectionPtr -> valueLength;
        strncpy(apiSelectionPtr -> _3_temperature_main.name, doc["data"][3]["feature"], nameLen - 1);
        strncpy(apiSelectionPtr-> _3_temperature_main.timestamp, doc["data"][3]["timestamp"], stampLen - 1);
        snprintf(apiSelectionPtr -> _3_temperature_main.value, valLen - 1, "%.1f", (float)doc["data"][3]["properties"]["value"]["value"]);
        
        strncpy(apiSelectionPtr -> _5_boiler_temperature.name, doc["data"][5]["feature"], nameLen - 1);
        strncpy(apiSelectionPtr-> _5_boiler_temperature.timestamp, doc["data"][5]["timestamp"], stampLen - 1);
        snprintf(apiSelectionPtr -> _5_boiler_temperature.value, valLen - 1, "%.1f", (float)doc["data"][5]["properties"]["value"]["value"]);
        
        strncpy(apiSelectionPtr -> _8_burner_hours.name, doc["data"][8]["feature"], nameLen - 1);
        strncpy(apiSelectionPtr-> _8_burner_hours.timestamp, doc["data"][8]["timestamp"], stampLen - 1);
        snprintf(apiSelectionPtr -> _8_burner_hours.value, valLen - 1, "%.0f", (float)doc["data"][8]["properties"]["hours"]["value"]);
        
        strncpy(apiSelectionPtr -> _8_burner_starts.name, doc["data"][8]["feature"], nameLen - 1);
        strncpy(apiSelectionPtr-> _8_burner_starts.timestamp, doc["data"][8]["timestamp"], stampLen - 1);
        snprintf(apiSelectionPtr -> _8_burner_starts.value, valLen - 1, "%.0f", (float)doc["data"][8]["properties"]["starts"]["value"]);
        
        strncpy(apiSelectionPtr -> _9_burner_is_active.name, doc["data"][9]["feature"], nameLen - 1);
        strncpy(apiSelectionPtr-> _9_burner_is_active.timestamp, doc["data"][9]["timestamp"], stampLen - 1);
        strcpy(apiSelectionPtr -> _9_burner_is_active.value, (boolean)doc["data"][9]["properties"]["active"]["value"] ? "true" : "false");
        
        strncpy(apiSelectionPtr -> _11_circulation_pump_status.name, doc["data"][11]["feature"], nameLen - 1);
        strncpy(apiSelectionPtr -> _11_circulation_pump_status.timestamp, doc["data"][11]["timestamp"], stampLen - 1);
        strncpy(apiSelectionPtr -> _11_circulation_pump_status.value, doc["data"][11]["properties"]["status"]["value"], valLen -1);
        
        strncpy(apiSelectionPtr -> _23_heating_curve_shift.name, doc["data"][23]["feature"], nameLen - 1);
        strncpy(apiSelectionPtr-> _23_heating_curve_shift.timestamp, doc["data"][23]["timestamp"], stampLen - 1);
        snprintf(apiSelectionPtr -> _23_heating_curve_shift.value, valLen - 1, "%.1f", (float)doc["data"][23]["properties"]["shift"]["value"]);
        
        strncpy(apiSelectionPtr -> _23_heating_curve_slope.name, doc["data"][23]["feature"], nameLen - 1);
        strncpy(apiSelectionPtr-> _23_heating_curve_slope.timestamp, doc["data"][23]["timestamp"], stampLen - 1);
        snprintf(apiSelectionPtr -> _23_heating_curve_slope.value, valLen - 1, "%.1f", (float)doc["data"][23]["properties"]["slope"]["value"]);
        


        //int ret = snprintf(buffer, sizeof buffer, "%f", myFloat);
        /*
        strncpy(apiSelection._5_Boiler_temperature.name, doc["data"][5]["feature"], nameLen);
        strncpy(apiSelection._5_Boiler_temperature.timestamp, doc["data"][5]["timestamp"], stampLen);
        strncpy(apiSelection._5_Boiler_temperature.value, doc["data"][5]["value"], valLen);
        */

        
        Serial.println("Print in ViessmannClient again:");
        Serial.println(apiSelectionPtr -> _3_temperature_main.name);
        Serial.println(apiSelectionPtr -> _3_temperature_main.timestamp);
        Serial.println(apiSelectionPtr -> _3_temperature_main.value);
        

        /*
        WiFiClient *stream = _viessmannHttpPtr ->getStreamPtr();        
        uint32_t bytesRead = 0;
        uint32_t chunkSize = 1000;
        uint32_t maxBytesRead = 90000;
        uint16_t maxRounds = 73;
        uint16_t rounds = 0;
        char chunkBuffer[chunkSize +1] = {0};

        
        //while(_viessmannHttpPtr ->connected() && stream->available())
        while(rounds < maxRounds)
        {
        
           bytesRead += stream ->readBytes(chunkBuffer, chunkSize);
           chunkBuffer[chunkSize] = '\0';
           Serial.println(chunkBuffer);

           //strcat((char *)responseBuffer, chunkBuffer);

           //responseBuffer += chunkSize;
           Serial.printf("\r\n%d\r\n", bytesRead);
        }
        */
        
        
        
        /*
        String payload = _viessmannHttpPtr ->getString();      
        int charsToCopy = payload.length() < reponseBufferLength ? payload.length() : reponseBufferLength;
        Serial.printf("\r\nPayloadLength = %d\r\n", payload.length());
        Serial.printf("\r\nCharsToCopyLength = %d\r\n", charsToCopy);
        int payloadLength = payload.length();
        int toCopy = 800;
        for (int i = 0; i < toCopy; i++)
        //for (int i = 0; i < charsToCopy; i++)
        {
            responseBuffer[i] = payload[payloadLength - toCopy + i];
        }
         */              
    } 
    else 
    {
        Serial.printf("Fehler bei der Anfrage, HTTP-Code: %d\n", httpResponseCode);
    }
    //_viessmannHttpPtr->end();
    return httpResponseCode;
}
    

t_httpCode ViessmannClient::GetEquipment(uint8_t* responseBuffer, const uint16_t reponseBufferLength)
{
    String encodedUrl = _viessmannAccountPtr -> UriEndPointIot + "equipment/installations" + "?includeGateways=true"; 
    String authorizationHeader = "Bearer " + _viessmannAccountPtr ->AccessToken;
    Serial.println(encodedUrl);
    _viessmannHttpPtr ->begin(encodedUrl);
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
    //_viessmannHttpPtr->end();
    return httpResponseCode;
} 

t_httpCode ViessmannClient::GetUser(uint8_t* responseBuffer, const uint16_t reponseBufferLength)
{
    String encodedUrl = _viessmannAccountPtr -> UriEndPointUser + "?sections=identity"; 

    String authorizationHeader = "Bearer " + _viessmannAccountPtr ->AccessToken;
    Serial.println(encodedUrl);

    _viessmannHttpPtr ->begin(encodedUrl);
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
    //_viessmannHttpPtr->end();  
    return httpResponseCode; 
}

ViessmannClient::~ViessmannClient()
{};