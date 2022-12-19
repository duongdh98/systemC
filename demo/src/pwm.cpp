#include "../include/pwm.hpp"

PWM::PWM(sc_module_name name)
{
    m_read_value = 0;
    n_read_value = 0;
    current_flag = false;
    count = 0;

    pwm_out.initialize(false);
    pwm_flag.initialize(false);

    SC_METHOD(read_values_method);
    sensitive << pwm_wr_enable.pos();
    dont_initialize();

    SC_METHOD(handleClock);
    sensitive << pwm_clk.pos();
    dont_initialize();

    SC_THREAD(process_thread_pwm_out);
    sensitive << pwm_clk.pos();
    dont_initialize();

    SC_THREAD(handleFlagThread);
    sensitive << pwm_clk.pos();
    dont_initialize();
}

PWM::~PWM()
{
}


void PWM::handleFlagThread() {
    while (true) {
        wait(flag_event);
        if (false == current_flag) {
            pwm_flag.write(true);
            current_flag = true;
        }
        else {
            pwm_flag.write(false);
            current_flag = false;
        }
    }
}

void PWM::read_values_method() {
    m_read_value = m_val.read();
    n_read_value = n_val.read();
}

void PWM::handleClock() {
    count++;
    if (count > m_read_value) {
        count = 0;
        flag_event.notify();
    }
    clock_event.notify();
}

void PWM::process_thread_pwm_out() {
    while (true) {
        wait(clock_event);
        if (count < n_read_value) {
            pwm_out.write(1);
        }
        else if (count < m_read_value) {
            pwm_out.write(0);
        }
    }
}