#ifndef CONFIG_H
#define CONFIG_H

namespace config
{
    extern const char *AP_PREFIX;
    extern int AP_PARENT_CONNECTION_TRIES;
    extern int WEB_SERVER_PORT;
} 

namespace board
{
    extern int STATUS_LED;
}
#endif