#include <hidapi.h>

int handle_ok;
static int check_connection_ok;

#include <unistd.h>
#define Sleep(x) usleep(x * 1000)

typedef enum {
    NO_ERR,
    ERR_NUL,
    ERR_CON
} joycon_status;

hid_device *handle;

typedef enum {
    JOYCON_L,
    JOYCON_R,
    PROCON,
    OTHER
} joycon_type;

typedef struct
{
    hid_device *handle;
    joycon_type type;
} joycon;

joycon_status init_joycon(joycon *con, joycon_type type);

joycon_status release_joycon(joycon *con);

int device_connection(){
    joycon con;
    if (check_connection_ok) {
        handle_ok = 0;
        // Try Joy-Con (L)
        if (init_joycon(&con, JOYCON_L) == NO_ERR) {
            handle_ok = 1;
            handle = con.handle;
            return handle_ok;
        }
        // Joy-Con (R)
        if (init_joycon(&con, JOYCON_R) == NO_ERR) {
            handle_ok = 2;
            handle = con.handle;
            return handle_ok;
        }
        // Pro Controller
        if (init_joycon(&con, PROCON) == NO_ERR) {
            handle_ok = 3;
            handle = con.handle;
            return handle_ok;
        }
        // Nothing found
        else {
            return 0;
        }
    }
    return handle_ok;
}

joycon_status init_joycon(joycon *con, joycon_type type)
{
    if (!con) {
        return ERR_NUL;
    }

    // Init controller
    switch (type)
    {
    case JOYCON_L:
        con->handle = hid_open(0x57e, 0x2006, NULL);
        break;
    case JOYCON_R:
        con->handle = hid_open(0x57e, 0x2007, NULL);
        break;
    case PROCON:
        con->handle = hid_open(0x57e, 0x2009, NULL);
        break;
    default:
        break;
    }

    // Set controller type
    if (con->handle) {
        con->type = type;
        return NO_ERR;
    }

    return ERR_CON;
}

joycon_status release_joycon(joycon *con)
{
    if (con && con->handle)
    {
        hid_close(con->handle);
        con->handle = NULL;
    }
    return NO_ERR;
}
