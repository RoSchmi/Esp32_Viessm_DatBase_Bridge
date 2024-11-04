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



t_httpCode ViessmannClient::GetFeatures(uint8_t* responseBuffer, const uint16_t reponseBufferLength, const uint32_t data_0_id, const char * gateways_0_serial, const char * gateways_0_devices_0_id)
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
        
        JsonDocument doc;
        
         deserializeJson(doc, _viessmannHttpPtr ->getStream());
         
        //JsonArray data = doc["data"];
        //const char * tempOutside = data[95]["feature"];
        //const char * tempOutside = doc["features"]["data"][95]["feature"];
        const char * data_0_feature = doc["data"][0]["feature"];
        const char * data_1_feature = doc["data"][1]["feature"];
        const char * data_3_feature = doc["data"][3]["feature"];
        const char * data_20_feature = doc["data"][20]["feature"];
        const char * data_40_feature = doc["data"][40]["feature"];
        const char * data_50_feature = doc["data"][50]["feature"];
        const char * data_60_feature = doc["data"][60]["feature"];
        const char * data_70_feature = doc["data"][70]["feature"];
        const char * data_77_feature = doc["data"][77]["feature"];
        const char * data_95_feature = doc["data"][95]["feature"];
        //const char * tempOutside = doc["data"][95]["properties"]["value"]["unit"];
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
        Serial.println("data_50_feature:");
        Serial.println(data_50_feature);
        Serial.println("data_60_feature:");
        Serial.println(data_60_feature);
        Serial.println("data_70_feature:");
        Serial.println(data_70_feature);
        Serial.println("data_77_feature:");
        Serial.println(data_77_feature);
        Serial.println("data_95_feature:");
        Serial.println(data_95_feature);
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