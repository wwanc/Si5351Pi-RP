#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include "si5351.h"
using namespace std; 
  
int main(int argc, char** argv) 
{ 
    Si5351 si5351("/dev/i2c-1");
    if(si5351.init(SI5351_CRYSTAL_LOAD_10PF, 0, 0) == false){
        cout<<"Failed to initialize Si5351.";
        return 1;
    }

    if(argc > 4) argc = 4;
    try {
        for (int i = 1; i < argc; ++i){
            double freq_kHz = stod(argv[i]);
            cout << "CLK"<<i-1<<": "<<freq_kHz << " kHz\n";
            if(i == 1){
                si5351.set_freq(freq_kHz*100000, SI5351_CLK0);
            }
            if(i == 2){
                si5351.set_freq(freq_kHz*100000, SI5351_CLK1);
            }
            if(i == 3){
                si5351.set_freq(freq_kHz*100000, SI5351_CLK2);
            }
        }
        si5351.update_status();
        usleep(500000);
        cout<<"SYS_INIT: "<<int(si5351.dev_status.SYS_INIT)<<endl;
        cout<<"  LOL_A: "<<int(si5351.dev_status.LOL_A)<<endl;
        cout<<"  LOL_B: "<<int(si5351.dev_status.LOL_B)<<endl;
        cout<<"  LOS: "<<int(si5351.dev_status.LOS)<<endl;
        cout<<"  REVID: "<<int(si5351.dev_status.REVID)<<endl;
    } catch (const std::invalid_argument& ia) {
            std::cerr << "Invalid argument: " << ia.what() << '\n';
    }
  
    return 0; 
} 
