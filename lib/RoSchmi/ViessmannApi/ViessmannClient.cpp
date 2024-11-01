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
t_httpCode ViessmannClient::GetEquipment(uint8_t* responseBuffer, uint16_t reponseBufferLength)
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
        
    return httpResponseCode;
} 

t_httpCode ViessmannClient::GetUser(uint8_t* responseBuffer, uint16_t reponseBufferLength)
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
        
    return httpResponseCode; 
}

ViessmannClient::~ViessmannClient()
{};