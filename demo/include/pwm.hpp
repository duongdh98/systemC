#ifndef __PWM_H
#define __PWM_H

#include "../include/global.hpp"

class PWM : public sc_module
{
private:
    /* data */
public:

    SC_HAS_PROCESS(PWM);
    PWM(sc_module_name name);
    ~PWM();

    int m_read_value;
    int n_read_value;

    sc_event            pwm_event;
    sc_in_clk           pwm_clk;
    sc_in <bool>        pwm_reset;
    sc_in <bool>        pwm_wr_enable;
    sc_in < sc_uint <8> > pwm_wr_m_value;
    sc_in < sc_uint <8> > pwm_wr_n_value;
    sc_out <bool>       pwm_out;
    sc_out <bool>       pwm_time;

    void pwm_init();
    void read_values_method();
    void process_thread_pwm_out();
    void pwm_gen_output(int m_val, int n_val);
    void pwm_reset_out();
    void pwm_reset_wr();
};

#endif /* __PWM_H */