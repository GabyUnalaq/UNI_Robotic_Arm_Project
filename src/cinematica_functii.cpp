#include <iostream>
#include <math.h>
#include <cmath>
#include <stdlib.h>

#define pi 3.14159265358979323846

float angle_check(float angle);

float to_robot(float angle);
float to_program(float angle);

float degree_to_rad(float angle);
float rad_to_degree(float angle);

float** calc_matrix(float angle, float dist);
float** multiply_matrix(float ** a, float ** b);
float* multiply_matrix(float ** mat, float * vec);

void afisare_matrice(float** matrix);

/// CINEMATICA DIRECTA
void cinematic(){
    std::cout << std::endl;
    std::cout << "========== Cinematica directa ==========" << std::endl << std::endl;
    float angle_1 = 150;
    float angle_2 = 150;
    float angle_3 = 150;

    float dist_0 = 4.5;
    float dist_1 = 9.5;
    float dist_2 = 9.8;
    float dist_3 = 6.0;

    angle_1 = angle_check(angle_1);
    angle_2 = angle_check(angle_2);
    angle_3 = angle_check(angle_3);

    angle_1 = to_program(angle_1);
    angle_2 = to_program(angle_2);
    angle_3 = to_program(angle_3);

    float ** matrix_0 = calc_matrix(0, dist_0);
    float ** matrix_1 = calc_matrix(degree_to_rad(angle_1), dist_1);
    float ** matrix_2 = calc_matrix(degree_to_rad(angle_2), dist_2);
    float ** matrix_3 = calc_matrix(degree_to_rad(angle_3), dist_3);

    float ** final_matrix = multiply_matrix(matrix_0, matrix_1);
    final_matrix = multiply_matrix(final_matrix, matrix_2);
    final_matrix = multiply_matrix(final_matrix, matrix_3);

    std::cout << "Matrice finala: " << std::endl;
    afisare_matrice(final_matrix);
}

/// CINEMATICA INVERSA
void rev_cinematic(){
    std::cout << std::endl;
    std::cout << "========== Cinematica inversa ==========" << std::endl << std::endl;
    /// Input:
    float efx = 8; // effector x pos
    float efy = 19; // effector y pos
    float efa = 0; // effector angle with Ox

    float d0 = 4.5; // distance from base to first joint
    float d1 = 9.5; // distance from first to second joint
    float d2 = 9.8; // distance from second to third joint
    float d3 = 6.0; // distance from third to effector joint

    /// Other variables
    float x0 = 0, y0 = 0; // coords of the base
    float x1, y1; // coords of the first joint
    float x2, y2; // coords of the second joint
    float x3, y3; // coords of the third joint

    float dist; // distance to the effector
    float a0, a1, a2, a3; // angles of the robot

    /// Calculations:
    x1 = x0;
    y1 = y0 + d0;

    x3 = -1 * d3 * cos(efa) + efx;
    y3 = -1 * d3 * sin(efa) + efy;

    dist = sqrt(pow(x1 - x3, 2) + pow(y1 - y3, 2));

    a0 = rad_to_degree(atan((y1 - y3)/(x1 - x3)));
    a1 = acos((d1*d1 + dist*dist - d2*d2)/(2 * d1 * dist)) + degree_to_rad(a0);
    if (x3 <= 0)
        a1 += 180;
    a1 = rad_to_degree(a1);

    a2 = rad_to_degree(acos((d1*d1 + d2*d2 - dist*dist)/(2 * d1 * d2))) + 180;

    if ((a1 + a2 - efa) >= 360)
        a3 = 360.0 - (a1 + a2 - efa - 360.0);
    else
        a3 = 360.0 - (a1 + a2 - efa);

    x2 = d1 * cos(degree_to_rad(a1)) + x1;
    y2 = d1 * sin(degree_to_rad(a1)) + y1;

    /// TODO transformare

    std::cout << "Pentru efectorul ce are: " << std::endl;
    std::cout << "x = " << efx << ", y = " << efy << std::endl;
    std::cout << "si orientarea " << efa << std::endl;
    std::cout << std::endl;
    std::cout << "Unghiurile cuplelor sunt:" << std::endl;
    std::cout << "a1 = " << a1 << std::endl;
    std::cout << "a2 = " << a2 << std::endl;
    std::cout << "a3 = " << a3 << std::endl;
    std::cout << std::endl;
    std::cout << "Pozitiile cuplelor sunt:" << std::endl;
    std::cout << "J1: ("  << x1 << ';' << y1 << ')'<< std::endl;
    std::cout << "J2: ("  << x2 << ';' << y2 << ')'<< std::endl;
    std::cout << "J3: ("  << x3 << ';' << y3 << ')'<< std::endl;
    std::cout << std::endl;

}

/// MAIN
int main(){
    cinematic();
    rev_cinematic();

    return 0;
}

/// Verifica daca unghiurile sunt in raza de miscare e robotului
float angle_check(float angle){
    if (angle < 0)
        return 0.0;
    else if (angle > 300)
        return 300.0;
    return angle;
}

/// Transforma din unghiul acceptat de program in cel in care prelucram
float to_program(float angle){
    return angle - 150;
}

/// Transforma din unghiul de prelucrare in cel acceptat de robot
float to_robot(float angle){
    return angle + 150;
}

/// Returneaza o matrice de transformare construita cu unghiul si distanta date ca parametrii
float** calc_matrix(float angle, float dist){
    //float matrix[3][3] = {{cos(angle), -sin(angle), 0},
    //                      {sin(angle), cos(angle), dist},
    //                      {0, 0, 1}};

    float** matrix = 0;
    matrix = new float*[3];

    for (int i = 0; i < 3; i++){
        matrix[i] = new float[3];
    }

    float cosa = cos(angle);
    float sina = sin(angle);

    //cout << "cos = " << cosa << endl;
    //cout << "sin = " << sina << endl;

    matrix[0][0] = cosa;
    matrix[0][1] = -sina;
    matrix[0][2] = 0;
    matrix[1][0] = sina;
    matrix[1][1] = cosa;
    matrix[1][2] = dist;
    matrix[2][0] = 0;
    matrix[2][1] = 0;
    matrix[2][2] = 1;

    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++){
            if (abs(matrix[i][j]) < 0.001)
                matrix[i][j] = 0;
        }

    return matrix;
}

/// Face transformare din radiani in grade
float rad_to_degree(float angle){
    return (angle * (180/pi));
}

/// Face transformare din grade in radiani
float degree_to_rad(float angle){
    return (angle * (pi/180));
}

/// Face inmultirea a doua matrici de 3/3, returnand rezultatul ca o matrice de 3/3
float** multiply_matrix(float ** a, float ** b){
    float** mul = 0;
    mul = new float*[3];

    for (int i = 0; i < 3; i++){
        mul[i] = new float[3];
        for(int j=0; j<3; j++)
            mul[i][j] = 0;
    }

    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            mul[i][j] = 0;
                for(int k=0; k<3; k++){
                    mul[i][j] += a[i][k] * b[k][j];
                }
        }
    }
    return mul;
}

/// Face inmultirea a unei matrici de 3/3 cu una 3/1, returnand rezultatul ca o matrice de 3/1
float* multiply_matrix(float ** mat, float * vec){
    float* vecpy = new float[3];
    for (int i=0; i<3; i++)
        vecpy[i] = vec[i];
    for (int i=0; i<3; i++)
        vec[i] = vecpy[0] * mat[i][0] + vecpy[1] * mat[i][1] + vecpy[2] * mat[i][2];
    return vec;
}

/// Afiseaza o matrice de 3/3
void afisare_matrice(float** matrix){
    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++)
            std::cout << matrix[i][j] << ' ';
        std::cout << std::endl;
    }
}
