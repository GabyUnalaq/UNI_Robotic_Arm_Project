#include <iostream>
#include <math.h>
#include <stdlib.h>

#define pi 3.14159265358979323846

using namespace std;

float angle_check(float angle);

float to_robot(float angle);
float to_program(float angle);

float degree_to_rad(float angle);
float rad_to_degree(float angle);

float** calc_matrix(float angle, float dist);
float** multiply_matrix(float ** a, float ** b);

void afisare_matrice(float** matrix);

int main(){
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

    cout << "Matrice finala: " << endl;
    afisare_matrice(final_matrix);

    return 0;
}

float angle_check(float angle){
    if (angle < 0)
        return 0.0;
    else if (angle > 300)
        return 300.0;
    return angle;
}

float to_program(float angle){
    return angle - 150;
}

float to_robot(float angle){
    return angle + 150;
}

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

float rad_to_degree(float angle){
    return (angle * (180/pi));
}

float degree_to_rad(float angle){
    return (angle * (pi/180));
}

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

void afisare_matrice(float** matrix){
    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++)
            cout << matrix[i][j] << ' ';
        cout << endl;
    }
}
