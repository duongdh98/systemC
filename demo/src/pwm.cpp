#include "../include/pwm.hpp"

PWM::PWM(sc_module_name name)
{
    pwm_init();
}

PWM::~PWM()
{
}

void PWM::pwm_init() {
    
    cout << __FUNCTION__ << ": init ... \n";
    pwm_out.initialize(false);
    pwm_time.initialize(false);

    SC_METHOD(read_values_method);
    sensitive << pwm_clk.pos();
    sensitive << pwm_reset;
    dont_initialize();

    SC_THREAD(process_thread_pwm_out);
}

void PWM::read_values_method() {
    if(pwm_wr_enable == 1) 
    {
        m_read_value = pwm_wr_m_value.read();
        n_read_value = pwm_wr_n_value.read();
        pwm_event.notify();
    }
}

void PWM::process_thread_pwm_out() {

    int current_m;
    int current_n;
    while (true)
    {
        cout << __FUNCTION__ << endl;
        wait(pwm_event);
        if(m_read_value != 0 && n_read_value != 0)
        {
            current_m = m_read_value;
            current_n = n_read_value;
            cout << "Current values m: " << current_m << " n: " << current_n << endl;
            int buff_out = 0;
            int buff_time = 0;
            while(true) {

                if(buff_out >= current_m){
                    buff_out = 0;
                }
                if(buff_time >= (current_m * 2)) {
                    buff_time = 0;
                }
                buff_out++;
                buff_time++;

                /* Pwm out */
                if(buff_out < current_n) {
                    pwm_out.write(1);
                } else {
                    pwm_out.write(0);
                }

                /* PWM time */
                if(buff_time <= current_m) {
                    pwm_time.write(1);
                } else {
                    pwm_time.write(0);
                }
                wait(1, SC_NS);
            }
        }
    }
}

void PWM::pwm_gen_output(int m_val, int n_val) {

    if(m_val != 0 && m_val >= n_val ) {
        pwm_out.write(1);
    }
    else if(m_val != 0 && m_val < n_val) {
        pwm_out.write(0);
    }
}

void PWM::pwm_reset_out() {
    pwm_out.write(0);
    pwm_time.write(0);
}

void PWM::pwm_reset_wr() {
}
