#ifndef __COUNTER_H
#define __COUNTER_H

#include "include/global.hpp"

SC_MODULE(counter) {

    sc_in_clk               clock;          // Clock input of the design
    sc_in<bool>             reset;          // active high, synchronous Reset input
    sc_in<bool>             enable;         // Active high enable signal for counter
    sc_out< sc_uint<8> >    counter_out;    // 8 bit vector output of the counter

    //------------Local Variables Here---------------------
    sc_uint<8>              count;

    //------------Code Starts Here-------------------------
    // Below function implements actual counter logic
    void incr_count ();

    // Constructor for the counter
    // Since this counter is a positive edge trigged one,
    // We trigger the below block with respect to positive
    // edge of the clock and also when ever reset changes state
    SC_CTOR(counter) {
        cout<<"Executing new"<<endl;
        SC_METHOD(incr_count);
        sensitive << reset;
        sensitive << clock.pos();
    }
    // End of Constructor
};

#endif /* __COUNTER_H */