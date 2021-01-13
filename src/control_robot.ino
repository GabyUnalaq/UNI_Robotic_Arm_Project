#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include "Arduino.h"
#include "AX12A.h"

#define pi 3.14159265358979323846
#define DIRECTION_PIN (10u)
#define BAUD_RATE (1000000ul)
#define CUPLA_1 (4u)
#define CUPLA_2 (5u)
#define CUPLA_3 (3u)


/// Lungimi cuple
float d0 = 4.5;
float d1 = 9.5;
float d2 = 9.8;
float d3 = 6.0;

/// Viteza simulare
float rot_step = 2;
float mov_step = 0.2;

/// Valori sticla
float st_d = 10;
float st_h = 19.8;

/// Valori calculate
float a0 = 0; // tangenta pantei distantei
float a1 = 0, a2 = 0, a3 = 0; // unghiuri
float ox = 0, oy = 0; // coord origine
float c1x, c1y, c2x, c2y, c3x, c3y; // coord cuple

/// Functii
void calc_angles(float efx, float efy, float efa);
float rad_to_degree(float angle);
float degree_to_rad(float angle);
void angle_to_servo(float &u1, float &u2, float &u3);
void angle_writing();
void reinstate_arm(float &efx, float &efy, float &efa);

void setup(){
    ax12a.begin(BAUD_RATE, DIRECTION_PIN, &Serial);
}

void loop(){
    float efx, efy, efa;

    /// Pozitia initiala
    efa = 90.0;
    efx = 0.0;
    efy = 29.8;
    calc_angles(efx, efy, efa);
    float aux_mov;
    angle_writing();
    delay(10000);

    /// Miscarea 0 - 1: miscare pe axa y
    aux_mov = efy;
    for (efy = aux_mov; efy - d3 >= st_h - 0.2; efy-=mov_step) {
        calc_angles(efx, efy, efa);
        angle_writing();
    }
   
    delay(3000);

    /// Miscare 1 - 2: rotatie
    aux_mov = efa;
    for (efa = aux_mov; efa>=0; efa-=rot_step){
        efx = c3x + cos(degree_to_rad(efa)) * d3;
        efy = c3y + sin(degree_to_rad(efa)) * d3;
        calc_angles(efx, efy, efa);
        angle_writing();
    }
   
    delay(3000);

    /// Miscare 2 - 3: miscare pe axa x
    aux_mov = efx;
    for (efx = aux_mov; efx<=(st_d - 1); efx+=mov_step){
        calc_angles(efx, efy, efa);
        angle_writing();
    }
    
    delay(3000);

    /// Miscare 3 - 4: rotatie
    aux_mov = efa;
    for (efa = aux_mov; efa>=-35; efa-=rot_step){
        calc_angles(efx, efy, efa);
        angle_writing();
    }
    
    delay(3000);

    /// Miscare 4 - 5: rotatie
    aux_mov = efa;
    for (efa = aux_mov; efa<=90; efa+=rot_step){
        efx = c3x + cos(degree_to_rad(efa)) * d3;
        efy = c3y + sin(degree_to_rad(efa)) * d3;
        calc_angles(efx, efy, efa);
        angle_writing();
    }
    
    delay(3000);

    /// Miscare 5 - 0: intoarcere
    efa = 90.0;
    efx = 0.0;
    efy = 29.8;
    //reinstate_arm(efx, efy, efa);
    calc_angles(efx, efy, efa);
    angle_writing();

}


/// Calculeaza folosind cinematica inversa
void calc_angles(float efx, float efy, float efa){
    float dist;

    c1x = ox;
    c1y = oy + d0;

    c3x = -1 * d3 * cos(degree_to_rad(efa)) + efx;
    c3y = -1 * d3 * sin(degree_to_rad(efa)) + efy;

    dist = sqrt(pow(c1x - c3x, 2) + pow(c1y - c3y, 2));

    a0 = rad_to_degree(atan((c1y - c3y)/(c1x - c3x)));
    a1 = rad_to_degree(acos((d1*d1 + dist*dist - d2*d2)/(2 * d1 * dist)) + degree_to_rad(a0));
    if (c3x <= 0)
        a1 += 180;

    a2 = rad_to_degree(acos((d1*d1 + d2*d2 - dist*dist)/(2 * d1 * d2))) + 180;

    if ((a1 + a2 - efa) >= 360)
        a3 = 360.0 - (a1 + a2 - efa - 360.0);
    else
        a3 = 360.0 - (a1 + a2 - efa);

    c2x = d1 * cos(degree_to_rad(a1)) + c1x;
    c2y = d1 * sin(degree_to_rad(a1)) + c1y;
}

/// Transforma unghiurile din sistemul programului de coordonate in cele ale cuplelor
void angle_to_servo(float &u1, float &u2, float &u3){
    u1 += 60;
    u2 -= 210;
    u3 -= 210;

    while (u1 < 0)
        u1 += 360;
    while (u2 < 0)
        u2 += 360;
    while (u3 < 0)
        u3 += 360;

    while (u1 > 360)
        u1 -= 360;
    while (u2 > 360)
        u2 -= 360;
    while (u3 > 360)
        u3 -= 360;
}

/// Trimite unghiurile la arduino
void angle_writing(){
    float u1, u2, u3;
    u1 = a1;
    u2 = a2;
    u3 = a3;
    angle_to_servo(u1, u2, u3);
    
    int c1 = int((512 * u1) / 150);
    int c2 = int((512 * u2) / 150);
    int c3 = int((512 * u3) / 150);
    
    ax12a.move(CUPLA_1, c1);
    ax12a.move(CUPLA_2, c2);
    ax12a.move(CUPLA_3, c3);
    /*
    ax12a.moveSpeed(CUPLA_1, c1, 50);
    ax12a.moveSpeed(CUPLA_2, c2, 50);
    ax12a.moveSpeed(CUPLA_3, c3, 50);
    */
    delay(1000);
}

void reinstate_arm(float &efx, float &efy, float &efa){ /// TODO
    /// Unghi 1
    for (float i = a1; i<90; i+=rot_step){
        a1 = i;
        angle_writing();
    }
    for (float i = a1; i>90; i-=rot_step){
        a1 = i;
        angle_writing();
    }

    ///Unghi 2
    for (float i = a1; i<360; i+=rot_step){
        a2 = i;
        angle_writing();
    }

    ///Unghi 3
    if ((a3 != 0) || (a3 != 360)){
        if (a3 < 150)
            for(float i = a3; i >= 0; i-=rot_step){
                a3 = i;
                angle_writing();
            }
        else
            for(float i = a3; i <= 360; i+=rot_step){
                a3 = i;
                angle_writing();
            }

    }
    efx = 0.0;
    efy = 29.8;
    efa = 90;

}

/// Face transformare din radiani in grade
float rad_to_degree(float angle){
    return (angle * (180/pi));
}

/// Face transformare din grade in radiani
float degree_to_rad(float angle){
    return (angle * (pi/180));
}
