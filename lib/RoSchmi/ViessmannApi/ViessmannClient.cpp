#include "ViessmannClient.h"

WiFiClient * _viessmannWifiClient;

ViessmannApiAccount  * _viessmannAccountPtr;
HTTPClient * _viessmannHttpPtr;

const char * _viessmannCaCert;

// Constructor
ViessmannClient::ViessmannClient(ViessmannApiAccount * account, const char * caCert, HTTPClient * httpClient, WiFiClient * wifiClient, uint8_t * bufferStorePtr)
{
    
    _viessmannAccountPtr = account;
    _viessmannCaCert = caCert;
    _viessmannHttpPtr = httpClient;
    _viessmannWifiClient = wifiClient;

    uint8_t * _requestPtr;
    uint8_t * _propertiesPtr;
    uint8_t * _responsePtr;
    uint8_t * _authorizationHeaderBufferPtr;

    
    // Some buffers located in memory segment .dram0.bss are injected to achieve lesser stack consumption
    /*
    _requestPtr = bufferStorePtr;
    _propertiesPtr = bufferStorePtr + REQUEST_BODY_BUFFER_LENGTH;
    _authorizationHeaderBufferPtr = bufferStorePtr + REQUEST_BODY_BUFFER_LENGTH + PROPERTIES_BUFFER_LENGTH;
    _responsePtr = bufferStorePtr + REQUEST_BODY_BUFFER_LENGTH + PROPERTIES_BUFFER_LENGTH + AUTH_HEADER_BUFFER_LENGTH;
    */
}


az_http_status_code ViessmannClient::GetUserName()
{
    String encodedUrl = _viessmannAccountPtr -> UriEndPointUser + "?sections=identity"; 

    String authorizationHeader = "Bearer " + _viessmannAccountPtr ->AccessToken;
        Serial.println(encodedUrl);
        _viessmannHttpPtr ->begin(encodedUrl);
        _viessmannHttpPtr ->addHeader("Authorization", authorizationHeader);
        int httpResponseCode = _viessmannHttpPtr ->GET();
        if (httpResponseCode > 0) 
        {
            String payload = _viessmannHttpPtr ->getString();
            Serial.println(payload);
        } 
        else 
        {
            Serial.printf("Fehler bei der Anfrage, HTTP-Code: %d\n", httpResponseCode);
        }
        
        return AZ_HTTP_STATUS_CODE_ACCEPTED;
}

ViessmannClient::~ViessmannClient()
{};