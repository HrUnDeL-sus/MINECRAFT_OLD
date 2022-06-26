#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define M_PI 3.14159265358979323846

float deegres_to_radiar(float myDegrees){
return myDegrees * M_PI  / 180;

}
float radiar_to_deegres(float myRadiar){
return myRadiar * 180  / M_PI;

}
