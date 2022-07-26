float cos_interpolate(float a,float b,float x){
  float ft = x * 3.1415927;
  float f = (1 - cosf(ft)) * 0.5;
  return a*(1-f) + b*f;
}
float one_interpolate(float x1,float x2,float v1,float v2,float x){
return  v1+( v2 - v1 )*(x - x1)/(x2 - x1);
}
float two_interpolate(float x1,float y1,float x2,float y2,float x,float y,float fx1y1,float fx2y1,float fx1y2,float fx2y2){
float fxy1=fx1y1+(x-x1)*((fx2y1-fx1y1)/(x2-x1));
float fxy2=fx1y2+(x-x1)*((fx2y2-fx1y2)/(x2-x1));
//printf("\nfx1y1: %f fx2y1 %f fx1y2 %f fx2y2 %f fxy1 %f fxy2 %f",fx1y1,fx2y1,fx1y2,fx2y2,fxy1,fxy2);
if((y-y1==0||y2-y1==0)&&((x-x1)==0||(x2-x1)==0))
    return 0;
if(x-x1==0|x2-x1==0)
    return 0;
if(y-y1==0||y2-y1==0)
return fxy1;
else
return fxy1+(y-y1)*((fxy2-fxy1)/(y2-y1));
}
