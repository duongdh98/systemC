#include "include/custom_gen.hpp"
#include "include/pwm.hpp"

// sc_main in top level function like in C++ main
int sc_main(int argc, char* argv[]) {

    cout << "\n>>>=============================================================> \n \n";

    sc_signal<bool>         signal_clk;
    sc_signal< sc_uint<8> > m_value;
    sc_signal< sc_uint<8> > n_value;
    sc_signal<bool>         reset;
    sc_signal<bool>         wr_enable;
    sc_signal<bool>         pwm_out;
    sc_signal<bool>         pwm_flag;

    CUSTOM_GEN custom_gen("custom_gen", 100);
    custom_gen.clk(signal_clk);

    PWM pwm("pwm");
    pwm.pwm_clk(signal_clk);
    pwm.pwm_reset(reset);
    pwm.pwm_wr_enable(wr_enable);
    pwm.m_val(m_value);
    pwm.n_val(n_value);
    pwm.pwm_out(pwm_out);
    pwm.pwm_flag(pwm_flag);

    sc_start(1, SC_NS);
    // Open VCD file
    sc_trace_file* wf = sc_create_vcd_trace_file("zWave");

    /* m = 0 , n = 0 */
#if 0
    wr_enable = true;
    m_value = 0;
    n_value = 0;
#endif

    /* m = 10 , n = 3 */
#if 0
    wr_enable = true;
    m_value = 10;
    n_value = 3;
#endif

    /* m = 10 , n = 5 */
#if 1
    wr_enable = true;
    m_value = 10;
    n_value = 5;
#endif
    
    /* m = 10 , n = 7 */
#if 0
    wr_enable = true;
    m_value = 10;
    n_value = 7;
#endif
    
    // Dump the desired signals
    sc_trace(wf, signal_clk, "signal_clk");
    sc_trace(wf, reset, "reset");
    sc_trace(wf, wr_enable, "wr_enable");
    sc_trace(wf, m_value, "m_value");
    sc_trace(wf, n_value, "n_value");
    sc_trace(wf, pwm_out, "pwm_out");
    sc_trace(wf, pwm_flag, "pwm_flag");

    for (int i = 0; i < custom_gen.ns; i++) {
        signal_clk = 0;
        sc_start(1, SC_NS);
        signal_clk = 1;
        sc_start(1, SC_NS);
    }

    sc_close_vcd_trace_file(wf);

    cout << "\n\n=== End Program of systemC    ==========================================================";
    return(0);
return(0);
}