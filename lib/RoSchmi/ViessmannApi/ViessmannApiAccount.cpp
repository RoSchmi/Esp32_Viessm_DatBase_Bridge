#include <ViessmannApiAccount.h>

/**
 * @brief 
 * 
 * @param clientId 
 * @param accessToken 
 * @param apiIot 
 * @param apiUser 
 * @param useHttps 
 * @return * constructor 
 */
ViessmannApiAccount::ViessmannApiAccount(String clientId, String accessToken, String apiIot, String apiUser, bool useHttps )
{
    ChangeAccountParams(clientId, accessToken, apiIot, apiUser, useHttps);   
}

void ViessmannApiAccount::ChangeAccountParams(String clientId, String accessToken, String apiIot, String apiUser, bool useHttps)
{
    ClientId = clientId;
    ClientId = (clientId.length() <= MAX_CLIENT_ID_LENGTH) ? clientId : clientId.substring(0, MAX_CLIENT_ID_LENGTH);
    AccessToken = accessToken;
    ApiIotUrl = apiIot;
    ApiUserUrl = apiIot;
    
    const char * insert = (char *)useHttps ? "s" : "";
    int baseLength = apiIot.length() > apiUser.length() ? apiIot.length() : apiUser.length();
    char strData[baseLength + 30];
    
    sprintf(strData, "http%s://%s", insert, apiIot.c_str());
    UriEndPointIot = String(strData);
    
    sprintf(strData, "http%s://%s", insert, apiUser.c_str());
    UriEndPointUser = String(strData);


    /*
    AccountName = (accountName.length() <= MAX_ACCOUNTNAME_LENGTH) ? accountName : accountName.substring(0, MAX_ACCOUNTNAME_LENGTH);
    AccountKey = accountKey;
    char strData[accountName.length() + 30];
    const char * insert = (char *)useHttps ? "s" : "";
    sprintf(strData, "http%s://%s.table.core.windows.net", insert, accountName.c_str());
    UriEndPointTable = String(strData);
    
    sprintf(strData, "%s.table.core.windows.net", accountName.c_str());
    HostNameTable = String(strData);
    */
}



/**
 * destructor
 */
ViessmannApiAccount::~ViessmannApiAccount()
{}