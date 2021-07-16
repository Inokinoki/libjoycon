/* Copyright (c) Mark J. Kilgard, 1997. */

/* This program is freely distributable without licensing fees 
   and is provided without guarantee or warrantee expressed or 
   implied. This program is -not- in the public domain. */

/* This program was requested by Patrick Earl; hopefully someone else
   will write the equivalent Direct3D immediate mode program. */

#include <GL/glut.h>

GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};  /* Red diffuse light. */
GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0}; /* Infinite light location. */
GLfloat n[6][3] = {                              /* Normals for the 6 faces of a cube. */
                   {-1.0, 0.0, 0.0},
                   {0.0, 1.0, 0.0},
                   {1.0, 0.0, 0.0},
                   {0.0, -1.0, 0.0},
                   {0.0, 0.0, 1.0},
                   {0.0, 0.0, -1.0}};
GLint faces[6][4] = {/* Vertex indices for the 6 faces of a cube. */
                     {0, 1, 2, 3},
                     {3, 2, 6, 7},
                     {7, 6, 5, 4},
                     {4, 5, 1, 0},
                     {5, 6, 2, 1},
                     {7, 4, 0, 3}};
GLfloat v[8][3]; /* Will be filled in with X,Y,Z vertexes. */

GLfloat angles[3];

void drawBox(void)
{
    int i;

    glPushMatrix();
    glRotatef(angles[0], 1.0, 0.0, 0.0);
    glRotatef(angles[1], 0.0, 1.0, 0.0);
    glRotatef(angles[2], 0.0, 0.0, 1.0);
    for (i = 0; i < 6; i++)
    {
        glBegin(GL_QUADS);
        glNormal3fv(&n[i][0]);
        glVertex3fv(&v[faces[i][0]][0]);
        glVertex3fv(&v[faces[i][1]][0]);
        glVertex3fv(&v[faces[i][2]][0]);
        glVertex3fv(&v[faces[i][3]][0]);
        glEnd();
    }
    glPopMatrix();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawBox();
    glutSwapBuffers();
}

#include <stdio.h>
#include <stdio.h>
#include <hidapi.h>
#include <string.h>
#include <stdint.h>
#include "../include/libjoycon.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

#include <unistd.h>
#define Sleep(x) usleep(x * 1000)

typedef enum
{
    NO_ERR,
    ERR_NUL,
    ERR_CON
} joycon_status;

static int handle_ok;
static int check_connection_ok;

static uint8_t timming_byte;

hid_device *handle;
hid_device *handle_l;

typedef enum
{
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

joycon_status get_joycon_info(joycon *con, u8 *buffer);

joycon_status get_joycon_spi_data(joycon *con, u32 offset, const u16 read_len, u8 *buffer);

int device_connection()
{
    joycon con;
    if (check_connection_ok)
    {
        handle_ok = 0;
        // Try Joy-Con (L)
        if (init_joycon(&con, JOYCON_L) == NO_ERR)
        {
            handle_ok = 1;
            handle = con.handle;
            return handle_ok;
        }
        // Joy-Con (R)
        if (init_joycon(&con, JOYCON_R) == NO_ERR)
        {
            handle_ok = 2;
            handle = con.handle;
            return handle_ok;
        }
        // Pro Controller
        if (init_joycon(&con, PROCON) == NO_ERR)
        {
            handle_ok = 3;
            handle = con.handle;
            return handle_ok;
        }
        // Nothing found
        else
        {
            return 0;
        }
    }
    return handle_ok;
}

joycon_status init_joycon(joycon *con, joycon_type type)
{
    if (!con)
    {
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
    if (con->handle)
    {
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

void init(void)
{
    fprintf(stderr, "Test\n");
    check_connection_ok = 1;
    if (!device_connection())
    {
        fprintf(stderr, "No connected device\n");
        return;
    }
    fprintf(stderr, "Connected device\n");

    uint8_t buf[64];
    uint8_t buf_read[64];
    memset(buf, 0, sizeof(buf));
    struct Header *hdr = (struct Header *)buf;
    struct SubcommandBody *pkt = (struct SubcommandBody *)(hdr + 1);
    hdr->command = Subcommand;
    hdr->counter = timming_byte & 0xF;
    timming_byte++;
    pkt->subcommand = EnableIMU;
    pkt->args.arg1 = IMU_ENABLE;
    hid_write(handle, buf, sizeof(buf));

    hdr->command = Subcommand;
    hdr->counter = timming_byte & 0xF;
    timming_byte++;
    pkt->subcommand = SetInputReportMode;
    pkt->args.arg1 = SIRFullSpeed;
    hid_write(handle, buf, sizeof(buf));

    /* Setup cube vertex data. */
    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;

    /* Enable a single OpenGL light. */
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    /* Use depth buffering for hidden surface elimination. */
    glEnable(GL_DEPTH_TEST);

    /* Setup the view of the cube. */
    glMatrixMode(GL_PROJECTION);
    gluPerspective(/* field of view in degree */ 40.0,
                   /* aspect ratio */ 1.0,
                   /* Z near */ 1.0, /* Z far */ 10.0);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(0.0, 0.0, 5.0, /* eye is at (0,0,5) */
              0.0, 0.0, 0.0, /* center is at (0,0,0) */
              0.0, 1.0, 0.); /* up is in positive Y direction */

    /* Adjust cube position to be asthetic angle. */
    glTranslatef(0.0, 0.0, -1.0);
    // glRotatef(60, 1.0, 0.0, 0.0);
    // glRotatef(-20, 0.0, 0.0, 1.0);
}

float joycon_gyro_decode(int16_t data, enum IMUSensitivity sensitivity)
{
    switch (sensitivity)
    {
    case Sensitivity0:
        return SENSITIVITY_GYROSCOPE_245 * data;
    case Sensitivity1:
        return SENSITIVITY_GYROSCOPE_500 * data;
    case Sensitivity3:
    default:
        return SENSITIVITY_GYROSCOPE_2000 * data;
    }
}

void read_imu_data(int t)
{
    // Update angle
    uint8_t buf_read[64];
    int res = hid_read_timeout(handle, buf_read, sizeof(buf_read), 200);
    struct InputReportHeader *header = (struct InputReportHeader *)buf_read;
    switch (header->id)
    {
    case SIRFullSpeed:
        fprintf(stderr, "Received SIR:\t0x%02X, LEN: %d\n", header->id, res);
        struct IMUPackedDataReply *reply = (struct IMUPackedDataReply *)(header + 1);

        angles[0] += 0.005 * joycon_gyro_decode((reply->data[0].gyro_1.le_bytes.high << 8) | reply->data[0].gyro_1.le_bytes.low, Sensitivity3);
        angles[0] += 0.005 * joycon_gyro_decode((reply->data[1].gyro_1.le_bytes.high << 8) | reply->data[1].gyro_1.le_bytes.low, Sensitivity3);
        angles[0] += 0.005 * joycon_gyro_decode((reply->data[2].gyro_1.le_bytes.high << 8) | reply->data[2].gyro_1.le_bytes.low, Sensitivity3);
        angles[1] += 0.005 * joycon_gyro_decode((reply->data[0].gyro_2.le_bytes.high << 8) | reply->data[0].gyro_2.le_bytes.low, Sensitivity3);
        angles[1] += 0.005 * joycon_gyro_decode((reply->data[1].gyro_2.le_bytes.high << 8) | reply->data[1].gyro_2.le_bytes.low, Sensitivity3);
        angles[1] += 0.005 * joycon_gyro_decode((reply->data[2].gyro_2.le_bytes.high << 8) | reply->data[2].gyro_2.le_bytes.low, Sensitivity3);
        angles[2] += 0.005 * joycon_gyro_decode((reply->data[0].gyro_3.le_bytes.high << 8) | reply->data[0].gyro_3.le_bytes.low, Sensitivity3);
        angles[2] += 0.005 * joycon_gyro_decode((reply->data[1].gyro_3.le_bytes.high << 8) | reply->data[1].gyro_3.le_bytes.low, Sensitivity3);
        angles[2] += 0.005 * joycon_gyro_decode((reply->data[2].gyro_3.le_bytes.high << 8) | reply->data[2].gyro_3.le_bytes.low, Sensitivity3);

        // Refresh it
        glutPostRedisplay();
        break;
    default:
        fprintf(stderr, "Received Other:\t0x%02X\n", header->id);
        break;
    }

    // Reset timer
    glutTimerFunc(0, read_imu_data, 0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("red 3D lighted cube");
    glutDisplayFunc(display);
    init();
    glutTimerFunc(15, read_imu_data, 0);
    glutMainLoop();

    uint8_t buf[64];
    uint8_t buf_read[64];
    memset(buf, 0, sizeof(buf));
    struct Header *hdr = (struct Header *)buf;
    struct SubcommandBody *pkt = (struct SubcommandBody *)(hdr + 1);
    hdr->counter = timming_byte & 0xF;
    timming_byte++;
    pkt->subcommand = EnableIMU;
    pkt->args.arg1 = IMU_DISABLE;
    hid_write(handle, buf, sizeof(buf));

    hdr->counter = timming_byte & 0xF;
    timming_byte++;
    pkt->subcommand = SetInputReportMode;
    pkt->args.arg1 = ButtonTrigged;
    hid_write(handle, buf, sizeof(buf));
    return 0; /* ANSI C requires main to return int. */
}
