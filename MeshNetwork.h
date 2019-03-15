#ifndef MESH_NETWORK_H
#define MESH_NETWORK_H

#include <ESP8266WiFi.h>
#include <stdio.h>

class MeshNetwork{
    private:
        String parentMAC;
        String parentName;
        int * possibleParents;
        int numberOfPossibleParent;
        bool checkPrefix(const char *text, const char *prefix);
        const char * generatePassword(const char * bssid);
        int selectSignal(int * signals);

      public:
        ~MeshNetwork();
        bool searchParent();
        bool connectParent();
        bool createAP();
        bool init();

};

#endif