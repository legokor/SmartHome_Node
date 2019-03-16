#include "MeshNetwork.h"
#include "config.h"


MeshNetwork::~MeshNetwork(){
    delete this->possibleParents;
}

const char * MeshNetwork::generatePassword(const char * bssid){
  return bssid;
}

String MeshNetwork::generateSoftAPName(){
    int parentID = this->parentName.substring(strlen(config::AP_PREFIX)).toInt();
    return config::AP_PREFIX+String((parentID+1));
}

IPAddress MeshNetwork::generateSoftAPIP()
{
    IPAddress parentIP(WiFi.gatewayIP());
    return IPAddress(
        parentIP[0],
        parentIP[1],
        parentIP[2] + 1,
        1);
}

bool MeshNetwork::checkPrefix(const char *text, const char *prefix)
{
    return 0 == strncmp(text,prefix,strlen(prefix));
}
int MeshNetwork::selectSignal(int *signals)
{
    int value = -100;
    int index = -1;

    for (int k = 0; k < this->numberOfPossibleParent; k++)
    {
        if (signals[k] > value)
        {
            value = signals[k];
            index = k;
        }
    }
    return index;
}
bool MeshNetwork::searchParent(){
    int numberOfNetworks = WiFi.scanNetworks();
    this->possibleParents = new int[numberOfNetworks];
    this->numberOfPossibleParent = numberOfNetworks;

    bool hasPossibleParent = false;
    //search for possible parents
    for (int i = 0; i < numberOfNetworks; i++)
    {
        if (this->checkPrefix(WiFi.SSID(i).c_str(), config::AP_PREFIX)){
            this->possibleParents[i] = WiFi.RSSI(i);
            hasPossibleParent = true;
        }
        else this->possibleParents[i] = -100;
    }

    return hasPossibleParent;
}

bool MeshNetwork::connectParent(){
    for (int tries = 0; tries < config::AP_PARENT_CONNECTION_TRIES && WiFi.status() != WL_CONNECTED; tries++)
    {
        int selectedID = this->selectSignal(this->possibleParents);
        if(selectedID == -1) break;
        this->parentName = WiFi.SSID(selectedID);
        String password = this->generatePassword(WiFi.BSSIDstr(selectedID).c_str());
        this->parentMAC = WiFi.BSSIDstr(selectedID);
        WiFi.begin(this->parentName, password);

        int waiting = 0;
        while (WiFi.status() != WL_CONNECTED && waiting < 10)
        {
            delay(1000);
            waiting++;
        }
    }
    return WiFi.status() == WL_CONNECTED;
}

bool MeshNetwork::createAP(){
    // IPAddress localIP = this->generateSoftAPIP();
    // IPAddress gateway = WiFi.localIP();
    IPAddress localIP = WiFi.localIP();
    IPAddress gateway = WiFi.gatewayIP();
    IPAddress subnet(255, 255, 255, 0);

    WiFi.softAPConfig(localIP, gateway, subnet);

    return WiFi.softAP(
        this->generateSoftAPName(), 
        this->generatePassword(WiFi.softAPmacAddress().c_str())
    );
}

bool MeshNetwork::init(){
    if(!this->searchParent()){
        Serial.print("Can't find parents");
        return false;
    }
    if (!this->connectParent())
    {
        Serial.print("Can't connect to parents");
        return false;
    }
    if (!this->createAP())
    {
        Serial.print("Can't create to AP");
        return false;
    }

    Serial.print("Parent address: ");
    Serial.println(WiFi.gatewayIP());

    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());

  

    Serial.print("Soft-AP IP address = ");
    Serial.println(WiFi.softAPIP());
    Serial.print("Password: ");
    Serial.println(this->generatePassword(WiFi.softAPmacAddress().c_str()));

    digitalWrite(board::STATUS_LED, HIGH);
}