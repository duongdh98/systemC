#ifndef __PWM_H
#define __PWM_H

#include <systemc.h>

class PWM : public sc_module
{
private:
    unsigned int count;
    unsigned int m_read_value;
    unsigned int n_read_value;
    bool current_flag;
public:

    SC_HAS_PROCESS(PWM);
    PWM(sc_module_name name);
    ~PWM();

    sc_in_clk           pwm_clk;
    sc_in <bool>        pwm_reset;
    sc_in <bool>        pwm_wr_enable;
    sc_in < sc_uint <8> > m_val;
    sc_in < sc_uint <8> > n_val;
    sc_out <bool>       pwm_out;
    sc_out <bool>       pwm_flag;
    sc_event            clock_event;
    sc_event            flag_event;

    void read_values_method();
    void handleClock(void);
    void process_thread_pwm_out();
    void handleFlagThread(void);
};

#endif /* __PWM_H */