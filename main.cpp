#include <iostream>
#include <vector>
#include "fir1.h"
#include "audioio.h"
#include <algorithm>
#include <stdexcept>

std::vector<double> trumpet_values;
std::vector<double> vibraphone_values;

AudioReader arT("Trumpet.wav");
AudioReader arV("Vibraphone.wav");

int sr = 96000;
int trial = 2*sr;
double fl = 0.3;
double ss = 4.9;
double lrate = 0.002;
int nc = int(fl*sr);
Fir1 fir(nc);
fir.setLearningRate(lrate);
int ns = int(sr*ss);

int fnum= 1;

int main(){

//fill vectors 
while (!arT.eof())
    trumpet_values.push_back(arT.get());
while (!arV.eof())
    vibraphone_values.push_back(arV.get());

std::vector<double> training = trumpet_values[ns:]; //start at later value
std::vector<double> src = vibraphone_values[ns:];

std::transform(src.begin(),src.end(),src.begin(),[](double c){return c*c;}); //square each element

double sum = std::accumulate(src.begin(), src.end(), 0.0);
double msi = sum/src.size();

if (training.size()!=src.size()){
throw std::invalid_argument( "interference and source samples are different lengths" );
}
std::cout << "Pre-training..." << std::endl;
int length;

if (trial < src.size()){
        length = trial;
}else{
        length = src.size();
}
for (int i = 0;i<length;i++){
        fir.lms_update(src[i]-fir.filter(training[i]));
