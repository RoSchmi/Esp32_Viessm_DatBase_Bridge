#include "ViessmannClient.h"

WiFiClient * _viessmannWifiClient;

ViessmannApiAccount  * _viessmannAccountPtr;
HTTPClient * _viessmannHttpPtr;

const char * _viessmannCaCert;

typedef int t_httpCode;

char initName[FEATURENAMELENGTH] {0};
char initTimestamp[FEATURESTAMPLENGTH] {0};
char initValue[FEATUREVALUELENGTH] {0};

ViessmannApiSelection::Feature tempFeature; 

//= {.initName, initTimestamp, initValue};
//strcpy(tempFeature.name, "");

 


/*
char name[FEATURENAMELENGTH] = {0};
        char timestamp[FEATURESTAMPLENGTH] = {0};     
        char value[FEATUREVALUELENGTH] = {0};
*/    

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

t_httpCode ViessmannClient::GetFeatures(uint8_t* responseBuffer, const uint16_t reponseBufferLength, const uint32_t data_0_id, const char * gateways_0_serial, const char * gateways_0_devices_0_id, ViessmannApiSelection * apiSelectionPtr)
{
    char InstallationId[20] = {0};
    sprintf(InstallationId, "%d", data_0_id);

    char GatewaySerial[30] = {0};

    String addendum = "features/installations/" + (String)InstallationId + "/gateways/" + (String(gateways_0_serial) + "/devices/" + String(gateways_0_devices_0_id) + "/features"); 
    String encodedUrl = _viessmannAccountPtr -> UriEndPointIot + addendum;
    String authorizationHeader = "Bearer " + _viessmannAccountPtr ->AccessToken;
    Serial.println(encodedUrl);

    //https://arduinojson.org/v7/how-to/use-arduinojson-with-httpclient/

    _viessmannHttpPtr ->useHTTP10(true);   // Must be reset to false for Azure requests
                                           // Is needed to load the long features JSON string 
    _viessmannHttpPtr ->begin(encodedUrl);
    _viessmannHttpPtr ->addHeader("Authorization", authorizationHeader);
    t_httpCode httpResponseCode = _viessmannHttpPtr ->GET();   
    if (httpResponseCode > 0) 
    { 
        if (httpResponseCode == HTTP_CODE_OK)
        {
            //Serial.println("Received ResponsCode > 0"); 
            JsonDocument doc;
            StaticJsonDocument<64> filter;
            filter["data"][0]["feature"] = true,
            filter["data"][0]["timestamp"] = true,
            filter["data"][0]["properties"] = true 
            ;
            deserializeJson(doc, _viessmannHttpPtr ->getStream(),DeserializationOption::Filter(filter));
    
            int nameLen = apiSelectionPtr ->nameLenght;
            int stampLen = apiSelectionPtr -> stampLength;
            int valLen = apiSelectionPtr -> valueLength;
            
            
            // From the long Features JSON string get the selected entities
        
            // in the first 'block' we write in a temporary feature object and copy the
            // object, but writing directly to the members seems to work as well
            // see the following features
            tempFeature.idx = 3;
            strncpy(tempFeature.name, doc["data"][3]["feature"], nameLen - 1);
            strncpy(tempFeature.timestamp, doc["data"][3]["timestamp"], stampLen - 1);
            snprintf(tempFeature.value, valLen - 1, "%.1f", (float)doc["data"][3]["properties"]["value"]["value"]);
            apiSelectionPtr -> _3_temperature_main = tempFeature;
        
            apiSelectionPtr -> _5_boiler_temperature.idx = 5;
            strncpy(apiSelectionPtr -> _5_boiler_temperature.name, doc["data"][5]["feature"], nameLen - 1);
            strncpy(apiSelectionPtr-> _5_boiler_temperature.timestamp, doc["data"][5]["timestamp"], stampLen - 1);
            snprintf(apiSelectionPtr -> _5_boiler_temperature.value, valLen - 1, "%.1f", (float)doc["data"][5]["properties"]["value"]["value"]);
        
            apiSelectionPtr -> _7_burner_modulation.idx = 7;
            strncpy(apiSelectionPtr -> _7_burner_modulation.name, doc["data"][7]["feature"], nameLen - 1);
            strncpy(apiSelectionPtr-> _7_burner_modulation.timestamp, doc["data"][7]["timestamp"], stampLen - 1);
            snprintf(apiSelectionPtr -> _7_burner_modulation.value, valLen - 1, "%.0f", (float)doc["data"][7]["properties"]["value"]["value"]);
            
            apiSelectionPtr -> _8_burner_hours.idx = 8;
            strncpy(apiSelectionPtr -> _8_burner_hours.name, doc["data"][8]["feature"], nameLen - 1);
            strncpy(apiSelectionPtr-> _8_burner_hours.timestamp, doc["data"][8]["timestamp"], stampLen - 1);
            snprintf(apiSelectionPtr -> _8_burner_hours.value, valLen - 1, "%.2f", (float)doc["data"][8]["properties"]["hours"]["value"]);
        
            apiSelectionPtr -> _8_burner_starts.idx = 8;
            strncpy(apiSelectionPtr -> _8_burner_starts.name, doc["data"][8]["feature"], nameLen - 1);
            strncpy(apiSelectionPtr-> _8_burner_starts.timestamp, doc["data"][8]["timestamp"], stampLen - 1);
            snprintf(apiSelectionPtr -> _8_burner_starts.value, valLen - 1, "%.0f", (float)doc["data"][8]["properties"]["starts"]["value"]);
            
            apiSelectionPtr -> _9_burner_is_active.idx = 9;
            strncpy(apiSelectionPtr -> _9_burner_is_active.name, doc["data"][9]["feature"], nameLen - 1);
            strncpy(apiSelectionPtr-> _9_burner_is_active.timestamp, doc["data"][9]["timestamp"], stampLen - 1);
            strcpy(apiSelectionPtr -> _9_burner_is_active.value, (boolean)doc["data"][9]["properties"]["active"]["value"] ? "true" : "false");
            
            apiSelectionPtr -> _11_circulation_pump_status.idx = 11;
            strncpy(apiSelectionPtr -> _11_circulation_pump_status.name, doc["data"][11]["feature"], nameLen - 1);
            strncpy(apiSelectionPtr -> _11_circulation_pump_status.timestamp, doc["data"][11]["timestamp"], stampLen - 1);
            strncpy(apiSelectionPtr -> _11_circulation_pump_status.value, doc["data"][11]["properties"]["status"]["value"], valLen -1);
        
            apiSelectionPtr -> _23_heating_curve_shift.idx = 23;
            strncpy(apiSelectionPtr -> _23_heating_curve_shift.name, doc["data"][23]["feature"], nameLen - 1);
            strncpy(apiSelectionPtr-> _23_heating_curve_shift.timestamp, doc["data"][23]["timestamp"], stampLen - 1);
            snprintf(apiSelectionPtr -> _23_heating_curve_shift.value, valLen - 1, "%.1f", (float)doc["data"][23]["properties"]["shift"]["value"]);
        
            apiSelectionPtr -> _23_heating_curve_slope.idx = 23;
            strncpy(apiSelectionPtr -> _23_heating_curve_slope.name, doc["data"][23]["feature"], nameLen - 1);
            strncpy(apiSelectionPtr-> _23_heating_curve_slope.timestamp, doc["data"][23]["timestamp"], stampLen - 1);
            snprintf(apiSelectionPtr -> _23_heating_curve_slope.value, valLen - 1, "%.1f", (float)doc["data"][23]["properties"]["slope"]["value"]);
        
            apiSelectionPtr -> _77_temperature_supply.idx = 77;
            strncpy(apiSelectionPtr -> _77_temperature_supply.name, doc["data"][77]["feature"], nameLen - 1);
            strncpy(apiSelectionPtr-> _77_temperature_supply.timestamp, doc["data"][77]["timestamp"], stampLen - 1);
            snprintf(apiSelectionPtr -> _77_temperature_supply.value, valLen - 1, "%.1f", (float)doc["data"][77]["properties"]["value"]["value"]);
        
            apiSelectionPtr -> _85_heating_dhw_charging.idx = 85;
            strncpy(apiSelectionPtr -> _85_heating_dhw_charging.name, doc["data"][85]["feature"], nameLen - 1);
            strncpy(apiSelectionPtr-> _85_heating_dhw_charging.timestamp, doc["data"][85]["timestamp"], stampLen - 1);
            strcpy(apiSelectionPtr -> _85_heating_dhw_charging.value, (boolean)doc["data"][85]["properties"]["active"]["value"] ? "true" : "false");
        
            apiSelectionPtr -> _86_heating_dhw_pump_status.idx = 86;
            strncpy(apiSelectionPtr -> _86_heating_dhw_pump_status.name, doc["data"][86]["feature"], nameLen - 1);
            strncpy(apiSelectionPtr -> _86_heating_dhw_pump_status.timestamp, doc["data"][86]["timestamp"], stampLen - 1);
            strncpy(apiSelectionPtr -> _86_heating_dhw_pump_status.value, doc["data"][86]["properties"]["status"]["value"], valLen -1);
        
            apiSelectionPtr -> _88_heating_dhw_pump_primary_status.idx = 88;
            strncpy(apiSelectionPtr -> _88_heating_dhw_pump_primary_status.name, doc["data"][88]["feature"], nameLen - 1);
            strncpy(apiSelectionPtr -> _88_heating_dhw_pump_primary_status.timestamp, doc["data"][88]["timestamp"], stampLen - 1);
            strncpy(apiSelectionPtr -> _88_heating_dhw_pump_primary_status.value, doc["data"][88]["properties"]["status"]["value"], valLen -1);
        
            apiSelectionPtr -> _90_heating_dhw_cylinder_temperature.idx = 90;
            strncpy(apiSelectionPtr -> _90_heating_dhw_cylinder_temperature.name, doc["data"][90]["feature"], nameLen - 1);
            strncpy(apiSelectionPtr-> _90_heating_dhw_cylinder_temperature.timestamp, doc["data"][90]["timestamp"], stampLen - 1);
            snprintf(apiSelectionPtr -> _90_heating_dhw_cylinder_temperature.value, valLen - 1, "%.1f", (float)doc["data"][90]["properties"]["value"]["value"]);
        
            apiSelectionPtr -> _92_heating_dhw_outlet_temperature.idx = 92;
            strncpy(apiSelectionPtr -> _92_heating_dhw_outlet_temperature.name, doc["data"][92]["feature"], nameLen - 1);
            strncpy(apiSelectionPtr-> _92_heating_dhw_outlet_temperature.timestamp, doc["data"][92]["timestamp"], stampLen - 1);
            snprintf(apiSelectionPtr -> _92_heating_dhw_outlet_temperature.value, valLen - 1, "%.1f", (float)doc["data"][92]["properties"]["value"]["value"]);
        
            apiSelectionPtr -> _93_heating_dhw_main_temperature.idx = 93;
            strncpy(apiSelectionPtr -> _93_heating_dhw_main_temperature.name, doc["data"][93]["feature"], nameLen - 1);
            strncpy(apiSelectionPtr-> _93_heating_dhw_main_temperature.timestamp, doc["data"][93]["timestamp"], stampLen - 1);
            snprintf(apiSelectionPtr -> _93_heating_dhw_main_temperature.value, valLen - 1, "%.1f", (float)doc["data"][93]["properties"]["value"]["value"]);
        
            apiSelectionPtr -> _95_heating_temperature_outside.idx = 95;
            strncpy(apiSelectionPtr -> _95_heating_temperature_outside.name, doc["data"][95]["feature"], nameLen - 1);
            strncpy(apiSelectionPtr-> _95_heating_temperature_outside.timestamp, doc["data"][95]["timestamp"], stampLen - 1);
            snprintf(apiSelectionPtr -> _95_heating_temperature_outside.value, valLen - 1, "%.1f", (float)doc["data"][95]["properties"]["value"]["value"]);   
        }
        else
        {
            // If request doesn't return with ok, we printout the begin of the response string
            WiFiClient *stream = _viessmannHttpPtr ->getStreamPtr();        
            uint32_t bytesRead = 0;
            uint32_t chunkSize = 1000;       
            uint16_t maxRounds = 10;
            uint16_t rounds = 0;
            char chunkBuffer[chunkSize +1] = {0};

            while(rounds < maxRounds)
            {
            bytesRead += stream ->readBytes(chunkBuffer, chunkSize);
            chunkBuffer[chunkSize] = '\0';
            //Serial.println(chunkBuffer);
            strcat((char *)responseBuffer, chunkBuffer);
            responseBuffer += chunkSize;
            //Serial.printf("\r\n%d\r\n", bytesRead);
            }
        }                      
    } 
    else 
    {
        Serial.printf("Fehler bei der Anfrage, HTTP-Code: %d\n", httpResponseCode);
    }
    _viessmannHttpPtr ->useHTTP10(false);
    _viessmannHttpPtr->end();
    return httpResponseCode;
}

t_httpCode ViessmannClient::RefreshAccessToken(uint8_t* responseBuffer, const uint16_t reponseBufferLength, const char * refreshToken)
{
    String body = "grant_type=refresh_token&client_id=" + (String)_viessmannAccountPtr ->ClientId + "&refresh_token=" + (String)refreshToken; 
    String encodedUrl = _viessmannAccountPtr -> UriEndPointToken;

   //encodedUrl = "https://iam.viessmann.com/idp/v3/token";
    
    Serial.println(encodedUrl);
    Serial.println(body);
    
    _viessmannHttpPtr ->begin(encodedUrl);
    
    _viessmannHttpPtr ->addHeader("Content-Type", "application/x-www-form-urlencoded; charset=utf-8");
    
    t_httpCode httpResponseCode =_viessmannHttpPtr ->POST((String)body);
    if (httpResponseCode > 0) 
    {
        Serial.println("Responsecode is > 0");
        String payload = _viessmannHttpPtr ->getString();
        Serial.println("Got payload");
        Serial.println(payload);
        int charsToCopy = payload.length() < reponseBufferLength ? payload.length() : reponseBufferLength;
        for (int i = 0; i < charsToCopy; i++)
        {
            responseBuffer[i] = payload[i];
        }               
    } 
    else 
    {
        Serial.printf("Error perfrming the request, HTTP-Code: %d\n", httpResponseCode);
    }
    _viessmannHttpPtr->end();
    
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
    _viessmannHttpPtr->end();
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
    _viessmannHttpPtr->end();  
    return httpResponseCode; 
}

ViessmannClient::~ViessmannClient()
{};